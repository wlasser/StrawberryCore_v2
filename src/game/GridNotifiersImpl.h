/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef STRAWBERRY_GRIDNOTIFIERSIMPL_H
#define STRAWBERRY_GRIDNOTIFIERSIMPL_H

#include "GridNotifiers.h"
#include "WorldPacket.h"
#include "Corpse.h"
#include "Player.h"
#include "UpdateData.h"
#include "CreatureAI.h"
#include "SpellAuras.h"
#include "DBCEnums.h"

template<class T>
inline void Strawberry::VisibleNotifier::Visit(GridRefManager<T> &m)
{
    for(typename GridRefManager<T>::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        i_camera.UpdateVisibilityOf(iter->getSource(), i_data, i_visibleNow);
        i_clientGUIDs.erase(iter->getSource()->GetObjectGuid());
    }
}

inline void Strawberry::ObjectUpdater::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        WorldObject::UpdateHelper helper(iter->getSource());
        helper.Update(i_timeDiff);
    }
}

inline void PlayerCreatureRelocationWorker(Player* pl, Creature* c)
{
    // Creature AI reaction
    if (!c->hasUnitState(UNIT_STAT_LOST_CONTROL))
    {
        if (c->AI() && c->AI()->IsVisible(pl) && !c->IsInEvadeMode())
            c->AI()->MoveInLineOfSight(pl);
    }
}

inline void CreatureCreatureRelocationWorker(Creature* c1, Creature* c2)
{
    if (!c1->hasUnitState(UNIT_STAT_LOST_CONTROL))
    {
        if (c1->AI() && c1->AI()->IsVisible(c2) && !c1->IsInEvadeMode())
            c1->AI()->MoveInLineOfSight(c2);
    }

    if (!c2->hasUnitState(UNIT_STAT_LOST_CONTROL))
    {
        if (c2->AI() && c2->AI()->IsVisible(c1) && !c2->IsInEvadeMode())
            c2->AI()->MoveInLineOfSight(c1);
    }
}

inline void Strawberry::PlayerRelocationNotifier::Visit(CreatureMapType &m)
{
    if (!i_player.isAlive() || i_player.IsTaxiFlying())
        return;

    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Creature* c = iter->getSource();
        if (c->isAlive())
            PlayerCreatureRelocationWorker(&i_player, c);
    }
}

template<>
inline void Strawberry::CreatureRelocationNotifier::Visit(PlayerMapType &m)
{
    if (!i_creature.isAlive())
        return;

    for(PlayerMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        Player* player = iter->getSource();
        if (player->isAlive() && !player->IsTaxiFlying())
            PlayerCreatureRelocationWorker(player, &i_creature);
    }
}

template<>
inline void Strawberry::CreatureRelocationNotifier::Visit(CreatureMapType &m)
{
    if (!i_creature.isAlive())
        return;

    for(CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        Creature* c = iter->getSource();
        if (c != &i_creature && c->isAlive())
            CreatureCreatureRelocationWorker(c, &i_creature);
    }
}

inline void Strawberry::DynamicObjectUpdater::VisitHelper(Unit* target)
{
    if (!target->isAlive() || target->IsTaxiFlying() )
        return;

    if (target->GetTypeId() == TYPEID_UNIT && ((Creature*)target)->IsTotem())
        return;

    if (!i_dynobject.IsWithinDistInMap(target, i_dynobject.GetRadius()))
        return;

    //Check targets for not_selectable unit flag and remove
    if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE))
        return;

    // Evade target
    if (target->GetTypeId()==TYPEID_UNIT && ((Creature*)target)->IsInEvadeMode())
        return;

    //Check player targets and remove if in GM mode or GM invisibility (for not self casting case)
    if (target->GetTypeId() == TYPEID_PLAYER && target != i_check && (((Player*)target)->isGameMaster() || ((Player*)target)->GetVisibility() == VISIBILITY_OFF))
        return;

    // for player casts use less strict negative and more stricted positive targeting
    if (i_check->GetTypeId() == TYPEID_PLAYER )
    {
        if (i_check->IsFriendlyTo( target ) != i_positive)
                return;
    }
    else
    {
        if (i_check->IsHostileTo( target ) == i_positive)
            return;
    }

    if (i_dynobject.IsAffecting(target))
        return;

    SpellEntry const *spellInfo = sSpellStore.LookupEntry(i_dynobject.GetSpellId());
    SpellEffectIndex eff_index  = i_dynobject.GetEffIndex();

    // Check target immune to spell or aura
    if (target->IsImmuneToSpell(spellInfo) || target->IsImmuneToSpellEffect(spellInfo, eff_index))
        return;

    // Apply PersistentAreaAura on target
    // in case 2 dynobject overlap areas for same spell, same holder is selected, so dynobjects share holder
    SpellAuraHolder *holder = target->GetSpellAuraHolder(spellInfo->Id, i_dynobject.GetCasterGuid());

    if (holder)
    {
        if (!holder->GetAuraByEffectIndex(eff_index))
        {
            PersistentAreaAura* Aur = new PersistentAreaAura(spellInfo, eff_index, NULL, holder, target, i_dynobject.GetCaster());
            holder->AddAura(Aur, eff_index);
            target->AddAuraToModList(Aur);
            holder->SetInUse(true);
            Aur->ApplyModifier(true,true);
            holder->SetInUse(false);
        }
        else if (holder->GetAuraDuration() >= 0 && uint32(holder->GetAuraDuration()) < i_dynobject.GetDuration())
        {
            holder->SetAuraDuration(i_dynobject.GetDuration());
            holder->SendAuraUpdate(false);
        }
    }
    else
    {
        holder = CreateSpellAuraHolder(spellInfo, target, i_dynobject.GetCaster());
        PersistentAreaAura* Aur = new PersistentAreaAura(spellInfo, eff_index, NULL, holder, target, i_dynobject.GetCaster());
        holder->AddAura(Aur, eff_index);
        target->AddSpellAuraHolder(holder);
    }

    i_dynobject.AddAffected(target);
}

template<>
inline void Strawberry::DynamicObjectUpdater::Visit(CreatureMapType  &m)
{
    for(CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        VisitHelper(itr->getSource());
}

template<>
inline void Strawberry::DynamicObjectUpdater::Visit(PlayerMapType  &m)
{
    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        VisitHelper(itr->getSource());
}

// SEARCHERS & LIST SEARCHERS & WORKERS

// WorldObject searchers & workers

template<class Check>
void Strawberry::WorldObjectSearcher<Check>::Visit(GameObjectMapType &m)
{
    // already found
    if (i_object)
        return;

    for(GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::WorldObjectSearcher<Check>::Visit(PlayerMapType &m)
{
    // already found
    if (i_object)
        return;

    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::WorldObjectSearcher<Check>::Visit(CreatureMapType &m)
{
    // already found
    if (i_object)
        return;

    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::WorldObjectSearcher<Check>::Visit(CorpseMapType &m)
{
    // already found
    if (i_object)
        return;

    for(CorpseMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::WorldObjectSearcher<Check>::Visit(DynamicObjectMapType &m)
{
    // already found
    if (i_object)
        return;

    for(DynamicObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::WorldObjectListSearcher<Check>::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::WorldObjectListSearcher<Check>::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::WorldObjectListSearcher<Check>::Visit(CorpseMapType &m)
{
    for(CorpseMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::WorldObjectListSearcher<Check>::Visit(GameObjectMapType &m)
{
    for(GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::WorldObjectListSearcher<Check>::Visit(DynamicObjectMapType &m)
{
    for(DynamicObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

// Gameobject searchers

template<class Check>
void Strawberry::GameObjectSearcher<Check>::Visit(GameObjectMapType &m)
{
    // already found
    if (i_object)
        return;

    for(GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::GameObjectLastSearcher<Check>::Visit(GameObjectMapType &m)
{
    for(GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
            i_object = itr->getSource();
    }
}

template<class Check>
void Strawberry::GameObjectListSearcher<Check>::Visit(GameObjectMapType &m)
{
    for(GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

// Unit searchers

template<class Check>
void Strawberry::UnitSearcher<Check>::Visit(CreatureMapType &m)
{
    // already found
    if (i_object)
        return;

    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::UnitSearcher<Check>::Visit(PlayerMapType &m)
{
    // already found
    if (i_object)
        return;

    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::UnitLastSearcher<Check>::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
            i_object = itr->getSource();
    }
}

template<class Check>
void Strawberry::UnitLastSearcher<Check>::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
            i_object = itr->getSource();
    }
}

template<class Check>
void Strawberry::UnitListSearcher<Check>::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::UnitListSearcher<Check>::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

// Creature searchers

template<class Check>
void Strawberry::CreatureSearcher<Check>::Visit(CreatureMapType &m)
{
    // already found
    if (i_object)
        return;

    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::CreatureLastSearcher<Check>::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
            i_object = itr->getSource();
    }
}

template<class Check>
void Strawberry::CreatureListSearcher<Check>::Visit(CreatureMapType &m)
{
    for(CreatureMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Check>
void Strawberry::PlayerSearcher<Check>::Visit(PlayerMapType &m)
{
    // already found
    if (i_object)
        return;

    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!itr->getSource()->InSamePhase(i_phaseMask))
            continue;

        if (i_check(itr->getSource()))
        {
            i_object = itr->getSource();
            return;
        }
    }
}

template<class Check>
void Strawberry::PlayerListSearcher<Check>::Visit(PlayerMapType &m)
{
    for(PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (itr->getSource()->InSamePhase(i_phaseMask))
            if (i_check(itr->getSource()))
                i_objects.push_back(itr->getSource());
}

template<class Builder>
void Strawberry::LocalizedPacketDo<Builder>::operator()( Player* p )
{
    int32 loc_idx = p->GetSession()->GetSessionDbLocaleIndex();
    uint32 cache_idx = loc_idx+1;
    WorldPacket* data;

    // create if not cached yet
    if (i_data_cache.size() < cache_idx+1 || !i_data_cache[cache_idx])
    {
        if (i_data_cache.size() < cache_idx+1)
            i_data_cache.resize(cache_idx+1);

        data = new WorldPacket(SMSG_MESSAGE_CHAT, 200);

        i_builder(*data, loc_idx);

        i_data_cache[cache_idx] = data;
    }
    else
        data = i_data_cache[cache_idx];

    p->SendDirectMessage(data);
}

template<class Builder>
void Strawberry::LocalizedPacketListDo<Builder>::operator()( Player* p )
{
    int32 loc_idx = p->GetSession()->GetSessionDbLocaleIndex();
    uint32 cache_idx = loc_idx+1;
    WorldPacketList* data_list;

    // create if not cached yet
    if (i_data_cache.size() < cache_idx+1 || i_data_cache[cache_idx].empty())
    {
        if (i_data_cache.size() < cache_idx+1)
            i_data_cache.resize(cache_idx+1);

        data_list = &i_data_cache[cache_idx];

        i_builder(*data_list, loc_idx);
    }
    else
        data_list = &i_data_cache[cache_idx];

    for(size_t i = 0; i < data_list->size(); ++i)
        p->SendDirectMessage((*data_list)[i]);
}

#endif                                                      // STRAWBERRY_GRIDNOTIFIERSIMPL_H

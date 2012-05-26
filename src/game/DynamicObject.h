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

#ifndef STRAWBERRYSERVER_DYNAMICOBJECT_H
#define STRAWBERRYSERVER_DYNAMICOBJECT_H

#include "Object.h"
#include "DBCEnums.h"
#include "Unit.h"

enum DynamicObjectType
{
    DYNAMIC_OBJECT_PORTAL           = 0x0,      // unused
    DYNAMIC_OBJECT_AREA_SPELL       = 0x1,
    DYNAMIC_OBJECT_FARSIGHT_FOCUS   = 0x2,
    DYNAMIC_OBJECT_RAID_MARKER      = 0x3,
};

struct SpellEntry;

class DynamicObject : public WorldObject
{
    public:
        typedef std::set<ObjectGuid> AffectedSet;
        explicit DynamicObject();

        void AddToWorld();
        void RemoveFromWorld();

        bool Create(uint32 guidlow, Unit *caster, uint32 spellId, SpellEffectIndex effIndex, float x, float y, float z, int32 duration, float radius, DynamicObjectType type);
        void Update(uint32 update_diff, uint32 p_time) override;
        void Delete();
        uint32 GetSpellId() const { return m_spellId; }
        SpellEffectIndex GetEffIndex() const { return m_effIndex; }
        uint32 GetDuration() const { return m_aliveDuration; }
        ObjectGuid const& GetCasterGuid() const { return GetGuidValue(DYNAMICOBJECT_CASTER); }
        Unit* GetCaster() const;
        float GetRadius() const { return m_radius; }
        DynamicObjectType GetType() const { return (DynamicObjectType)GetByteValue(DYNAMICOBJECT_BYTES,0); }
        AffectedSet GetAffected() const { return m_affected; };
        bool IsAffecting(Unit *unit) const { return m_affected.find(unit->GetObjectGuid()) != m_affected.end(); }
        void AddAffected(Unit *unit) { m_affected.insert(unit->GetObjectGuid()); }
        void RemoveAffected(Unit *unit) { m_affected.erase(unit->GetObjectGuid()); }
        void Delay(int32 delaytime);

        bool IsHostileTo(Unit const* unit) const;
        bool IsFriendlyTo(Unit const* unit) const;

        float GetObjectBoundingRadius() const               // overwrite WorldObject version
        {
            return 0.0f;                                    // dynamic object not have real interact size
        }

        bool isVisibleForInState(Player const* u, WorldObject const* viewPoint, bool inVisibleList) const;

        GridReference<DynamicObject> &GetGridRef() { return m_gridRef; }

    protected:
        uint32 m_spellId;
        SpellEffectIndex m_effIndex;
        int32 m_aliveDuration;
        float m_radius;                                     // radius apply persistent effect, 0 = no persistent effect
        bool m_positive;
        AffectedSet m_affected;
    private:
        GridReference<DynamicObject> m_gridRef;
};
#endif

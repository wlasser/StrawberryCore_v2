/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
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

/* ScriptData
SDName: instance_dragon_soul
SD%Complete: 80
SDComment: Does not support the Quests.
SDCategory: Caverns of Time, Dragon Soul
EndScriptData */

#include "pchdef.h"
#include "instance_dragon_soul.h"

instance_dragon_soul::instance_dragon_soul(Map *pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_dragon_soul::Initialize()
{
    memset(&m_uiEncounter, 0, sizeof(m_uiEncounter));
}

void instance_dragon_soul::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_uiEncounter[0] >> m_uiEncounter[1] >> m_uiEncounter[2] >> m_uiEncounter[3] >> m_uiEncounter[4];
    
    for(uint8 i = 0;i < MAX_ENCOUNTER;++i)
        if(m_uiEncounter[i] == IN_PROGRESS)
            m_uiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_dragon_soul::SetData(uint32 uiType, uint32 uiValue)
{
    m_uiEncounter[uiType] = uiValue;

    if(uiValue == DONE)
    {
        OUT_SAVE_INST_DATA; 

        std::ostringstream saveStream;
        saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " " << m_uiEncounter[3] << " " << m_uiEncounter[4];
        m_chrInStr = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_dragon_soul::GetData(uint32 uiType)
{
    return m_uiEncounter[uiType];
}

void instance_dragon_soul::OnCreatureCreate(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_MORCHOK:                    m_mNpcEntryGuidStore[BOSS_MORCHOK] = creature->GetObjectGuid();                    break;            
        case BOSS_WARLORD_ZONOZZ:            m_mNpcEntryGuidStore[BOSS_WARLORD_ZONOZZ] = creature->GetObjectGuid();            break;
        case BOSS_YORSAHJ_THE_UNSLEEPING:    m_mNpcEntryGuidStore[BOSS_YORSAHJ_THE_UNSLEEPING] = creature->GetObjectGuid();  break;
        case BOSS_HAGARA_THE_STORMBINDER:    m_mNpcEntryGuidStore[BOSS_HAGARA_THE_STORMBINDER] = creature->GetObjectGuid();  break;
        case BOSS_ULTRAXION:                m_mNpcEntryGuidStore[BOSS_ULTRAXION] = creature->GetObjectGuid();                break;
        case BOSS_WARMASTER_BLACKHORN:        m_mNpcEntryGuidStore[BOSS_WARMASTER_BLACKHORN] = creature->GetObjectGuid();     break;
        case BOSS_SPLINE_OF_DEATHWING:        m_mNpcEntryGuidStore[BOSS_SPLINE_OF_DEATHWING] = creature->GetObjectGuid();     break;
        case BOSS_MADNESS_OF_DEATHWING:        m_mNpcEntryGuidStore[BOSS_MADNESS_OF_DEATHWING] = creature->GetObjectGuid();    break;
    }
}

void instance_dragon_soul::OnCreatureEnterCombat(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_MORCHOK:                    SetData(DATA_MORCHOK_EVENT,IN_PROGRESS);                    break;                
        case BOSS_WARLORD_ZONOZZ:            SetData(DATA_WARLORD_ZONOZZ_EVENT,IN_PROGRESS);                break;
        case BOSS_YORSAHJ_THE_UNSLEEPING:    SetData(DATA_YORSAHJ_THE_UNSLEEPING_EVENT,IN_PROGRESS);     break;
        case BOSS_HAGARA_THE_STORMBINDER:    SetData(DATA_HAGARA_THE_STORMBINDER_EVENT,IN_PROGRESS);     break;
        case BOSS_ULTRAXION:                SetData(DATA_ULTRAXION_EVENT,IN_PROGRESS);                break;
        case BOSS_WARMASTER_BLACKHORN:        SetData(DATA_WARMASTER_BLACKHORN_EVENT,IN_PROGRESS);        break;
        case BOSS_SPLINE_OF_DEATHWING:        SetData(DATA_SPLINE_OF_DEATHWING_EVENT,IN_PROGRESS);        break;
        case BOSS_MADNESS_OF_DEATHWING:        SetData(DATA_MADNESS_OF_DEATHWING_EVENT,IN_PROGRESS);        break;
    }
}

void instance_dragon_soul::OnCreatureDeath(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_MORCHOK:                    SetData(DATA_MORCHOK_EVENT,DONE);                    break;                
        case BOSS_WARLORD_ZONOZZ:            SetData(DATA_WARLORD_ZONOZZ_EVENT,DONE);            break;
        case BOSS_YORSAHJ_THE_UNSLEEPING:    SetData(DATA_YORSAHJ_THE_UNSLEEPING_EVENT,DONE);    break;
        case BOSS_HAGARA_THE_STORMBINDER:    SetData(DATA_HAGARA_THE_STORMBINDER_EVENT,DONE);    break;
        case BOSS_ULTRAXION:                SetData(DATA_ULTRAXION_EVENT,DONE);                    break;
        case BOSS_WARMASTER_BLACKHORN:        SetData(DATA_WARMASTER_BLACKHORN_EVENT,DONE);        break;
        case BOSS_SPLINE_OF_DEATHWING:        SetData(DATA_SPLINE_OF_DEATHWING_EVENT,DONE);        break;
        case BOSS_MADNESS_OF_DEATHWING:        SetData(DATA_MADNESS_OF_DEATHWING_EVENT,DONE);        break;
    }
}

void instance_dragon_soul::OnCreatureEvade(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_MORCHOK:                    SetData(DATA_MORCHOK_EVENT,FAIL);                    break;                
        case BOSS_WARLORD_ZONOZZ:            SetData(DATA_WARLORD_ZONOZZ_EVENT,FAIL);            break;
        case BOSS_YORSAHJ_THE_UNSLEEPING:    SetData(DATA_YORSAHJ_THE_UNSLEEPING_EVENT,FAIL);    break;
        case BOSS_HAGARA_THE_STORMBINDER:    SetData(DATA_HAGARA_THE_STORMBINDER_EVENT,FAIL);    break;
        case BOSS_ULTRAXION:                SetData(DATA_ULTRAXION_EVENT,FAIL);                    break;
        case BOSS_WARMASTER_BLACKHORN:        SetData(DATA_WARMASTER_BLACKHORN_EVENT,FAIL);        break;
        case BOSS_SPLINE_OF_DEATHWING:        SetData(DATA_SPLINE_OF_DEATHWING_EVENT,FAIL);        break;
        case BOSS_MADNESS_OF_DEATHWING:        SetData(DATA_MADNESS_OF_DEATHWING_EVENT,FAIL);        break;
    }
}

InstanceData * GetInstanceData_instance_dragon_soul(Map* pMap)
{
    return new instance_dragon_soul(pMap);
}

void AddSC_instance_dragon_soul()
{
    Script * pScript;

    pScript = new Script;
    pScript->Name = "instance_dragon_soul";
    pScript->GetInstanceData = &GetInstanceData_instance_dragon_soul;
    pScript->RegisterSelf();
}
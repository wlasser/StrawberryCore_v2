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
SDName: instance_hour_of_twilight
SD%Complete: 80
SDComment: Does not support the Quests.
SDCategory: Caverns of Time, Hour of Twilight
EndScriptData */

#include "pchdef.h"
#include "instance_hour_of_twilight.h"

instance_hour_of_twilight::instance_hour_of_twilight(Map *pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_hour_of_twilight::Initialize()
{
    memset(&m_uiEncounter, 0, sizeof(m_uiEncounter));
}

void instance_hour_of_twilight::Load(const char* chrIn)
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

void instance_hour_of_twilight::SetData(uint32 uiType, uint32 uiValue)
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

uint32 instance_hour_of_twilight::GetData(uint32 uiType)
{
    return m_uiEncounter[uiType];
}

void instance_hour_of_twilight::OnCreatureCreate(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_ARCURION:		m_mNpcEntryGuidStore[BOSS_ARCURION] = creature->GetObjectGuid();       break;
        case BOSS_ASIRA_DAWNSLAYER:        m_mNpcEntryGuidStore[BOSS_ASIRA_DAWNSLAYER] = creature->GetObjectGuid();       break;
        case BOSS_ARCHBISHOP_BENEDICTUS:     m_mNpcEntryGuidStore[BOSS_ARCHBISHOP_BENEDICTUS] = creature->GetObjectGuid();    break;
    }
}

void instance_hour_of_twilight::OnCreatureEnterCombat(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_ARCURION:        SetData(DATA_ARCURION_EVENT,IN_PROGRESS);     break;
        case BOSS_ASIRA_DAWNSLAYER:        SetData(DATA_ASIRA_DAWNSLAYER_EVENT,IN_PROGRESS);     break;
        case BOSS_ARCHBISHOP_BENEDICTUS:     SetData(DATA_ARCHBISHOP_BENEDICTUS_EVENT,IN_PROGRESS);       break;
    }
}

void instance_hour_of_twilight::OnCreatureDeath(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_ARCURION:        SetData(DATA_ARCURION_EVENT,DONE);            break;
        case BOSS_ASIRA_DAWNSLAYER:        SetData(DATA_ASIRA_DAWNSLAYER_EVENT,DONE);            break;
        case BOSS_ARCHBISHOP_BENEDICTUS:     SetData(DATA_ARCHBISHOP_BENEDICTUS_EVENT,DONE);              break;
    }
}

void instance_hour_of_twilight::OnCreatureEvade(Creature * creature)
{
    switch(creature->GetEntry())
	{
        case BOSS_ARCURION:        SetData(DATA_ARCURION_EVENT,FAIL);            break;
        case BOSS_ASIRA_DAWNSLAYER:        SetData(DATA_ASIRA_DAWNSLAYER_EVENT,FAIL);            break;
        case BOSS_ARCHBISHOP_BENEDICTUS:     SetData(DATA_ARCHBISHOP_BENEDICTUS_EVENT,FAIL);            break;
    }

}

InstanceData * GetInstanceData_instance_hour_of_twilight(Map* pMap)
{
    return new instance_hour_of_twilight(pMap);
}

void AddSC_instance_hour_of_twilight()
{
    Script * pScript;

    pScript = new Script;
    pScript->Name = "instance_hour_of_twilight";
    pScript->GetInstanceData = &GetInstanceData_instance_hour_of_twilight;
    pScript->RegisterSelf();
}
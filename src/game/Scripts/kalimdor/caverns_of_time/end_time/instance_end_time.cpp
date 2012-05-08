/* 
Starwberry Scripts
*/
#include "pchdef.h"
#include "instance_end_time.h"

instance_end_time::instance_end_time(Map *pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_end_time::Initialize()
{
    memset(&m_uiEncounter, 0, sizeof(m_uiEncounter));
}

void instance_end_time::Load(const char* chrIn)
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

void instance_end_time::SetData(uint32 uiType, uint32 uiValue)
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

uint32 instance_end_time::GetData(uint32 uiType)
{
    return m_uiEncounter[uiType];
}

void instance_end_time::OnCreatureCreate(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_BAINE:		m_mNpcEntryGuidStore[BOSS_BAINE] = creature->GetObjectGuid();       break;
        case BOSS_JAINA:        m_mNpcEntryGuidStore[BOSS_JAINA] = creature->GetObjectGuid();       break;
        case BOSS_SYLVANAS:     m_mNpcEntryGuidStore[BOSS_SYLVANAS] = creature->GetObjectGuid();    break;
        case BOSS_TYRANDE:      m_mNpcEntryGuidStore[BOSS_TYRANDE] = creature->GetObjectGuid();     break;
        case BOSS_MUROZOND:     m_mNpcEntryGuidStore[BOSS_MUROZOND] = creature->GetObjectGuid();    break;
    }
}

void instance_end_time::OnCreatureEnterCombat(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_BAINE:        SetData(DATA_ECHO_BAINE_EVENT,IN_PROGRESS);     break;
        case BOSS_JAINA:        SetData(DATA_ECHO_JAINA_EVENT,IN_PROGRESS);     break;
        case BOSS_SYLVANAS:     SetData(DATA_ECHO_SYLVANAS_EVENT,IN_PROGRESS);  break;
        case BOSS_TYRANDE:      SetData(DATA_ECHO_TYRANDE_EVENT,IN_PROGRESS);   break;
        case BOSS_MUROZOND:     SetData(DATA_MUROZOND_EVENT,IN_PROGRESS);       break;
    }
}

void instance_end_time::OnCreatureDeath(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_BAINE:        SetData(DATA_ECHO_BAINE_EVENT,DONE);            break;
        case BOSS_JAINA:        SetData(DATA_ECHO_JAINA_EVENT,DONE);            break;
        case BOSS_SYLVANAS:     SetData(DATA_ECHO_SYLVANAS_EVENT,DONE);         break;
        case BOSS_TYRANDE:      SetData(DATA_ECHO_TYRANDE_EVENT,DONE);          break;
        case BOSS_MUROZOND:     SetData(DATA_MUROZOND_EVENT,DONE);              break;
    }
}

void instance_end_time::OnCreatureEvade(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_BAINE:        SetData(DATA_ECHO_BAINE_EVENT,FAIL);            break;
        case BOSS_JAINA:        SetData(DATA_ECHO_JAINA_EVENT,FAIL);            break;
        case BOSS_SYLVANAS:     SetData(DATA_ECHO_SYLVANAS_EVENT,FAIL);         break;
        case BOSS_TYRANDE:      SetData(DATA_ECHO_TYRANDE_EVENT,FAIL);          break;
        case BOSS_MUROZOND:     SetData(DATA_MUROZOND_EVENT,FAIL);              break;
    }
}

InstanceData * GetInstanceData_instance_end_of_time(Map* pMap)
{
    return new instance_end_time(pMap);
}

void AddSC_instance_end_of_time()
{
    Script * pScript;

    pScript = new Script;
    pScript->Name = "instance_end_of_time";
    pScript->GetInstanceData = &GetInstanceData_instance_end_of_time;
    pScript->RegisterSelf();
}
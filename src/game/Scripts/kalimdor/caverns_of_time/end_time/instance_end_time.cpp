/****


**/
#include "instance_end_time.h"
#include "precompiled.h"

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
    
    for(uint8 = 0;i < MAX_ENCOUNTER;++i)
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
        saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " " << m_uiEncounter[3] << " " << m_uiEncounter[4]
        m_m_chrInStr = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_end_time::SetData64(uint32 uiType, uint64 uiValue)
{
    switch(uiType)
    {
        case DATA_ECHO_BAINE:       m_mNpcEntryGuidStore[BOSS_BAINE] = uiValue; break;
        case DATA_ECHO_JAINA:       m_mNpcEntryGuidStore[BOSS_JAINA] = uiValue; break;
        case DATA_ECHO_SYLVANAS:    m_mNpcEntryGuidStore[BOSS_SYLVANAS] = uiValue; break;
        case DATA_ECHO_TYRANDE:     m_mNpcEntryGuidStore[BOSS_TYRANDE] = uiValue; break;
        case DATA_MUROZOND:         m_mNpcEntryGuidStore[BOSS_MUROZOND] = uiValue; break;
    }
}

uint32 instance_end_time::GetData(uint32 uiType)
{
    return m_uiEncounter[uiType];
}

uint64 instance_end_time::GetData64(uint32 uiType)
{
    switch(uiType)
    {
        case DATA_ECHO_BAINE:       return m_mNpcEntryGuidStore[BOSS_BAINE]; break;
        case DATA_ECHO_JAINA:       return m_mNpcEntryGuidStore[BOSS_JAINA]; break;
        case DATA_ECHO_SYLVANAS:    return m_mNpcEntryGuidStore[BOSS_SYLVANAS]; break;
        case DATA_ECHO_TYRANDE:     return m_mNpcEntryGuidStore[BOSS_TYRANDE]; break;
        case DATA_MUROZOND:         return m_mNpcEntryGuidStore[BOSS_MUROZOND]; break;
    }
}
void instance_end_time::OnCreatureCreate(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_BAINE:        SetData64(BOSS_BAINE,creature->GetObjectGuid());       break;
        case BOSS_JAINA:        SetData64(BOSS_JAINA],creature->GetObjectGuid());       break;
        case BOSS_SYLVANAS:     SetData64(BOSS_SYLVANAS],reature->GetObjectGuid());    break;
        case BOSS_TYRANDE:      SetData64(BOSS_TYRANDE],creature->GetObjectGuid());     break;
        case BOSS_MUROZOND:     SetData64(BOSS_MUROZOND],creature->GetObjectGuid());    break;
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
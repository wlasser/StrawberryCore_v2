/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ENDTIME_H
#define DEF_ENDTIME_H

#define MAX_ENCOUNTER 5

enum Data64
{
    DATA_ECHO_BAINE,
    DATA_ECHO_JAINA,
    DATA_ECHO_SYLVANAS,
    DATA_ECHO_TYRANDE,
    DATA_MUROZOND,
};

enum DATA
{
    DATA_ECHO_BAINE_EVENT,
    DATA_ECHO_JAINA_EVENT,
    DATA_ECHO_SYLVANAS_EVENT,
    DATA_ECHO_TYRANDE_EVENT,
    DATA_MUROZOND_EVENT,
};

enum CreatureIds
{
    // Creatures
    BOSS_BAINE            = 54431,
    BOSS_JAINA            = 54445,
    BOSS_SYLVANAS        = 54123,
    BOSS_TYRANDE        = 54544,
    BOSS_MUROZOND        = 54432,
};

class instance_end_time : public ScriptedInstance
{
    public:
        instance_end_time(Map* pMap);
        ~instance_end_time() {}

        void Initialize();
        void Load(const char* chrIn);
        const char* Save() { return m_chrInStr.c_str(); }
        void OnCreatureCreate(Creature * creature);
        void OnCreatureEnterCombat(Creature * creature);
        void OnCreatureEvade(Creature * creature);
        void OnCreatureDeath(Creature * creature);
        uint32 GetData(uint32 uiType);
        void SetData(uint32 uiType, uint32 uiValue);

    private:
        std::string m_chrInStr;
        uint32 m_uiEncounter[MAX_ENCOUNTER];
};

#endif
/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ENDTIME_H
#define DEF_ENDTIME_H

#define MAX_ENCOUNTER 5

enum Data64
{
    DATA_ARCURION,
    DATA_ASIRA_DAWNSLAYER,
    DATA_ARCHBISHOP_BENEDICTUS,
};

enum DATA
{
    DATA_ARCURION_EVENT,
    DATA_ASIRA_DAWNSLAYER_EVENT,
    DATA_ARCHBISHOP_BENEDICTUS_EVENT,
};

enum CreatureIds
{
	// Creatures
	BOSS_ARCURION				= 54590,
	BOSS_ASIRA_DAWNSLAYER		= 54968,
	BOSS_ARCHBISHOP_BENEDICTUS	= 54938,
};

class instance_hour_of_twilight : public ScriptedInstance
{
    public:
        instance_hour_of_twilight(Map* pMap);
        ~instance_hour_of_twilight() {}

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
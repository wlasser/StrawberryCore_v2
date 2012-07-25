/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ENDTIME_H
#define DEF_ENDTIME_H

#define MAX_ENCOUNTER 5

enum Data64
{
    DATA_MORCHOK,
    DATA_WARLORD_ZONOZZ,
    DATA_YORSAHJ_THE_UNSLEEPING,
    DATA_HAGARA_THE_STORMBINDER,
    DATA_ULTRAXION,
	DATA_WARMASTER_BLACKHORN,
	DATA_SPLINE_OF_DEATHWING,
	DATA_MADNESS_OF_DEATHWING,
};

enum DATA
{
    DATA_MORCHOK_EVENT,
    DATA_WARLORD_ZONOZZ_EVENT,
    DATA_YORSAHJ_THE_UNSLEEPING_EVENT,
    DATA_HAGARA_THE_STORMBINDER_EVENT,
    DATA_ULTRAXION_EVENT,
	DATA_WARMASTER_BLACKHORN_EVENT,
	DATA_SPLINE_OF_DEATHWING_EVENT,
	DATA_MADNESS_OF_DEATHWING_EVENT,
};

enum CreatureIds
{
	// Creatures
	BOSS_MORCHOK				=55265,
    BOSS_WARLORD_ZONOZZ			=55308,
    BOSS_YORSAHJ_THE_UNSLEEPING	=55312,
    BOSS_HAGARA_THE_STORMBINDER	=55689,
    BOSS_ULTRAXION				=55294,
	BOSS_WARMASTER_BLACKHORN	=56427,
	BOSS_SPLINE_OF_DEATHWING	=53879,
	BOSS_MADNESS_OF_DEATHWING	=56173,
};

class instance_dragon_soul : public ScriptedInstance
{
    public:
        instance_dragon_soul(Map* pMap);
        ~instance_dragon_soul() {}

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
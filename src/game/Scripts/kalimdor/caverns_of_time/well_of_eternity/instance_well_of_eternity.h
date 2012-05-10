/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ENDTIME_H
#define DEF_ENDTIME_H

#define MAX_ENCOUNTER 5

enum Data64
{
    DATA_PEROTH_ARN,
    DATA_QUEEN_AZSHARA,
    DATA_CAPTAIN_VAROTHEN,
    DATA_MANNOROTH,
};

enum DATA
{
    DATA_PEROTH_ARN_EVENT,
    DATA_QUEEN_AZSHARA_EVENT,
    DATA_CAPTAIN_VAROTHEN_EVENT,
    DATA_MANNOROTH_EVENT,
};

enum CreatureIds
{
	// Creatures
	BOSS_PEROTH_ARN			= 55085,
	BOSS_QUEEN_AZSHARA		= 54853,
	BOSS_CAPTAIN_VAROTHEN	= 55419,
	BOSS_MANNOROTH			= 54969,
};

class instance_well_of_eternity : public ScriptedInstance
{
    public:
        instance_well_of_eternity(Map* pMap);
        ~instance_well_of_eternity() {}

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
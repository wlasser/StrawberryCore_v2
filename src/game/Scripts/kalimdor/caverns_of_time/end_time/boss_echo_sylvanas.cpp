/* 
Starwberry Scripts
*/

// Includes
#include instance_end_time.h
#include "precompiled.h"

// UPDATE `creature_template` SET ScriptName='npc_boss_echo_sylvanas' WHERE `entry`=54123;

struct MANGOS_DLL_DECL boss_echo_sylvanasAI : public ScriptedAI
{
	boss_echo_sylvanasAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 spell1_phase1_Timer;
	uint32 spell2_phase1_Timer;
	uint32 spell3_phase1_Timer;
	uint32 phase1_Percent;
	uint32 phase2_Percent;
	uint32 phase;

	void Aggro(Unit* pWho)
	{
		m_creature->MonsterYell("Another band of Deathwing's converts? I'll be sure your death is especially painful.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature, 10789);
	}

	void KilledUnit(Unit* pVictim)
	{
		m_creature->MonsterYell("And so ends your story.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature, 10789);
	}

	void JustDied(Unit* pKiller)
	{
		m_creature->MonsterYell("This ... isn't how it's supposed to ... end.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature, 10789);
	}

	void Reset()
	{
		spell1_phase1_Timer = 1000+rand()%19000;
		spell2_phase1_Timer = 1000+rand()%9000;
		spell3_phase1_Timer = 1000+rand()%29000;
		phase1_Percent = 100;
		phase2_Percent = 0;
		phase = 0;
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 100) && phase == 0)
		{
			phase = 1;
			m_creature->MonsterYell("Another band of Deathwing's converts? I'll be sure your death is especially painful.", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 0) && phase == 1)
		{
			phase = 2;
			m_creature->MonsterYell("This ... isn't how it's supposed to ... end.", LANG_UNIVERSAL, NULL);
		}

		if (phase == 1)
		{

			if (spell1_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10141);
				spell1_phase1_Timer = 1000+rand()%19000;
			} else spell1_phase1_Timer -= diff;

			if (spell2_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), 10141);
				spell2_phase1_Timer = 1000+rand()%9000;
			} else spell2_phase1_Timer -= diff;

			if (spell3_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10140);
				spell3_phase1_Timer = 1000+rand()%29000;
			} else spell3_phase1_Timer -= diff;
		}

		if (phase == 2)
		{
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAIboss_echo_sylvanas(Creature* pCreature)
{
	return new boss_echo_sylvanasAI (pCreature);
}

void AddSC_boss_echo_sylvanas()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "npc_boss_echo_sylvanas";
	newscript->GetAI = &GetAIboss_echo_sylvanas;
	newscript->RegisterSelf();
}
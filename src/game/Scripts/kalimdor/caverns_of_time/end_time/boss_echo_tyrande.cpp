/* 
Starwberry Scripts
*/

#include "instance_end_time.h"
#include "pchdef.h"

// UPDATE `creature_template` SET ScriptName='npc_boss_echo_tyrande' WHERE `entry`=54544;

struct boss_echo_tyrandeAI : public ScriptedAI
{
	boss_echo_tyrandeAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 spell1_phase1_Timer;
	uint32 spell2_phase1_Timer;
	uint32 spell3_phase1_Timer;
	uint32 spell4_phase1_Timer;
	uint32 spell5_phase1_Timer;
	uint32 phase1_Percent;
	uint32 phase2_Percent;
	uint32 phase;

	void Aggro(Unit* pWho)
	{
	}

	void KilledUnit(Unit* pVictim)
	{
		m_creature->MonsterYell("Elune guide you through the night.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature->getVictim(), 10789);
	}

	void JustDied(Unit* pKiller)
	{
	}

	void Reset()
	{
		spell1_phase1_Timer = 1000+rand()%9000;
		spell2_phase1_Timer = 1000+rand()%29000;
		spell3_phase1_Timer = 1000+rand()%19000;
		spell4_phase1_Timer = 1000+rand()%49000;
		spell5_phase1_Timer = 1000+rand()%14000;
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
			m_creature->MonsterYell("Let the peaceful light of Elune soothe your souls in this dark time.", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 0) && phase == 1)
		{
			phase = 2;
			m_creature->MonsterYell("I can...see the light of the moon...so clearly now. It is...beautiful...", LANG_UNIVERSAL, NULL);
		}

		if (phase == 1)
		{

			if (spell1_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10219);
				spell1_phase1_Timer = 1000+rand()%9000;
			} else spell1_phase1_Timer -= diff;

			if (spell2_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10241);
				spell2_phase1_Timer = 1000+rand()%29000;
			} else spell2_phase1_Timer -= diff;

			if (spell3_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10217);
				spell3_phase1_Timer = 1000+rand()%19000;
			} else spell3_phase1_Timer -= diff;

			if (spell4_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10214);
				spell4_phase1_Timer = 1000+rand()%49000;
			} else spell4_phase1_Timer -= diff;

			if (spell5_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), 10219);
				spell5_phase1_Timer = 1000+rand()%14000;
			} else spell5_phase1_Timer -= diff;
		}

		if (phase == 2)
		{
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAIboss_echo_tyrande(Creature* pCreature)
{
	return new boss_echo_tyrandeAI (pCreature);
}

void AddSC_boss_echo_tyrande()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "npc_boss_echo_tyrande";
	newscript->GetAI = &GetAIboss_echo_tyrande;
	newscript->RegisterSelf();
}
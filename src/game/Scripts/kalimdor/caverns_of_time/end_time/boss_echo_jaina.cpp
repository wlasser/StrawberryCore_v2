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

// Includes
#include "instance_end_time.h"
#include "pchdef.h"

// UPDATE `creature_template` SET ScriptName='npc_boss_echo_jaina' WHERE `entry`=54445;

struct boss_echo_jainaAI : public ScriptedAI
{
	boss_echo_jainaAI(Creature* pCreature) : ScriptedAI(pCreature)
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

	bool enraged;

	void Aggro(Unit* pWho)
	{
		m_creature->MonsterYell("You asked for it.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature->getVictim(), 10180);
	}

	void KilledUnit(Unit* pVictim)
	{
		m_creature->MonsterYell("I didn't want to do that.", LANG_UNIVERSAL, NULL);
	}

	void JustDied(Unit* pKiller)
	{
		m_creature->MonsterYell("I understand, now. Farewell, and good luck.", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature->getVictim(), 10181);
	}

	void Reset()
	{
		spell1_phase1_Timer = 1000+rand()%14000;
		spell2_phase1_Timer = 1000+rand()%29000;
		spell3_phase1_Timer = 1000+rand()%30000;
		spell4_phase1_Timer = 1000+rand()%31000;
		spell5_phase1_Timer = 1000+rand()%24000;
		phase1_Percent = 100;
		phase2_Percent = 0;
		phase = 0;
		enraged = false;
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 90) && !enraged)
		{
			enraged = true;
			DoCastSpellIfCan(m_creature->getVictim(), 10789);
			m_creature->MonsterYell("I hate resorting to violence.", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 100) && phase == 0)
		{
			phase = 1;
			m_creature->MonsterYell("I don't know who you are, but I'll defend this shrine with my life.", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 0) && phase == 1)
		{
			phase = 2;
			m_creature->MonsterYell("I understand, now. Farewell, and good luck.", LANG_UNIVERSAL, NULL);
		}

		if (phase == 1)
		{

			if (spell1_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), 10180);
				spell1_phase1_Timer = 1000+rand()%14000;
			} else spell1_phase1_Timer -= diff;

			if (spell2_phase1_Timer <= diff)
			{
				DoCastSpellIfCan(m_creature, 10181);
				spell2_phase1_Timer = 1000+rand()%29000;
			} else spell2_phase1_Timer -= diff;

			if (spell3_phase1_Timer <= diff)
			{
				DoCastSpellIfCan((m_creature->getVictim(), 0), 10133);
				spell3_phase1_Timer = 1000+rand()%30000;
			} else spell3_phase1_Timer -= diff;

			if (spell4_phase1_Timer <= diff)
			{
				DoCastSpellIfCan((m_creature->getVictim(), 0), 10181);
				spell4_phase1_Timer = 1000+rand()%31000;
			} else spell4_phase1_Timer -= diff;

			if (spell5_phase1_Timer <= diff)
			{
				DoCastSpellIfCan((m_creature->getVictim(), 0), 10192);
				spell5_phase1_Timer = 1000+rand()%24000;
			} else spell5_phase1_Timer -= diff;
		}

		if (phase == 2)
		{
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAIboss_echo_jaina(Creature* pCreature)
{
	return new boss_echo_jainaAI (pCreature);
}

void AddSC_boss_echo_jaina()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "npc_boss_echo_jaina";
	newscript->GetAI = &GetAIboss_echo_jaina;
	newscript->RegisterSelf();
}
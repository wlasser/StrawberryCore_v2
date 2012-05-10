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
SDName: Boss_Echo_Baine
SD%Complete: 20
SDComment: Some timers are not correct. Alpha Script.
SDCategory: Caverns of Time, End Time
EndScriptData */

// Includes
#include "instance_end_time.h"
#include "pchdef.h"

// UPDATE `creature_template` SET ScriptName='npc_boss_echo_baine' WHERE `entry`=54431;

struct boss_echo_baineAI : public ScriptedAI
{
    boss_echo_baineAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        enraged = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

	uint32 spell1_phase1_Timer;
	uint32 phase1_Percent;
	uint32 phase2_Percent;
	uint32 phase;

	bool enraged;

	void Aggro(Unit* pWho)
	{
		m_creature->MonsterYell("What dark horrors have you wrought in this place? By my ancestors' honor, I shall take you to task!", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(m_creature, 10183);
	}

	void KilledUnit(Unit* pVictim)
	{
		m_creature->MonsterYell("This is the price you pay!", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan((m_creature->getVictim()), 10162);
	}

	void JustDied(Unit* pKiller)
	{
		m_creature->MonsterYell("Where... is this place? What... have I done? Forgive me, my father...", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan((m_creature->getVictim()), 10184);
	}

	void Reset()
	{
		spell1_phase1_Timer = 0+rand()%30000;
		phase1_Percent = 100;
		phase2_Percent = 0;
		phase = 0;
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_HOLY, true);
		m_creature->ApplySpellImmune(853, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(20253, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(100, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(676, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(6552, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NATURE, true);
		m_creature->ApplySpellImmune(5246, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(85388, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(1776, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(408, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(1833, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(91800, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(47481, IMMUNITY_DAMAGE, 0, true);
		m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);
		m_creature->ApplySpellImmune(91797, IMMUNITY_DAMAGE, 0, true);
		enraged = false;
	}

	void UpdateAI(const uint32 diff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 50) && !enraged)
		{
			enraged = true;
			DoCastSpellIfCan(m_creature->getVictim(), 10161);
			m_creature->MonsterYell("A just punishment.", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 100) && phase == 0)
		{
			phase = 1;
			m_creature->MonsterYell("What dark horrors have you wrought in this place? By my ancestors' honor, I shall take you to task!", LANG_UNIVERSAL, NULL);
		}

		if ((m_creature->GetHealth() * 100 / m_creature->GetMaxHealth() <= 0) && phase == 1)
		{
			phase = 2;
			m_creature->MonsterYell("Where... is this place? What... have I done? Forgive me, my father...", LANG_UNIVERSAL, NULL);
		}

		if (phase == 1)
		{

			if (spell1_phase1_Timer <= diff)
			{
				DoCastSpellIfCan((m_creature->getVictim()), 10162);
				spell1_phase1_Timer = 0+rand()%30000;
			} else spell1_phase1_Timer -= diff;
		}

		if (phase == 2)
		{
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAIboss_echo_baine(Creature* pCreature)
{
	return new boss_echo_baineAI (pCreature);
}

void AddSC_boss_echo_baine()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "npc_boss_echo_baine";
	newscript->GetAI = &GetAIboss_echo_baine;
	newscript->RegisterSelf();
}
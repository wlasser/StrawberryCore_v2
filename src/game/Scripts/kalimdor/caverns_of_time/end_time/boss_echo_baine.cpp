/* 
Starwberry Scripts
*/

// Includes
#include instance_end_time.h
#include "precompiled.h"

enum
{
	// Spells of Baine and Player
	SPELL_MOLTEN_BLAST	= 101840 // Molten Blast
	SPELL_PULVERIZE		= 101627 // Pulverize
	SPELL_MAGMA			= 101619 // Magma IF Player stood in Lava
	SPELL_MOLTEN_FISTS	= 101866 // Molten Fists IF Player stood in Lava
	SPELL_THROW_TOTEM	= 101614 // Throw Totem
	SPELL_BAINES_TOTEM	= 101594 // Baine's Totem
	SPELL_PLAYER_TOTEM	= 101602 // Player's Throw Totem
	SPELL_MOLTEN_AXE	= 101836 // Boss Buff IF stood in Lava
};

// UPDATE `creature_template` SET ScriptName='npc_boss_echo_baine' WHERE `entry`=54431;

struct MANGOS_DLL_DECL boss_echo_baineAI : public ScriptedAI
{
	boss_echo_baineAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
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
		DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10162);
	}

	void JustDied(Unit* pKiller)
	{
		m_creature->MonsterYell("Where... is this place? What... have I done? Forgive me, my father...", LANG_UNIVERSAL, NULL);
		DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10184);
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
				DoCastSpellIfCan(SelectUnit(SELECT_TARGET_RANDOM, 0), 10162);
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
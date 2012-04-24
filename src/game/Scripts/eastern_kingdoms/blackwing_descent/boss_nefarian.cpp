enum eSpells
{
    SPELL_SHADOWFLAME_BREATH        = 94126,
    SPELL_SHADOW_COWARDICE          = 79353
};

class boss_bd_nefarian : public CreatureScript
{
public:
    boss_bd_nefarian() : CreatureScript("boss_bd_nefarian") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_bd_nefarianAI (creature);
    }

    struct boss_bd_nefarianAI : public ScriptedAI
    {
        boss_bd_nefarianAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 ShadowflameBreathTimer;
        uint32 ShadowCowardiceTimer;

        void Reset()
        {
            ShadowflameBreathTimer = 10*IN_MILLISECONDS;
            ShadowCowardiceTimer = 13*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/)
        {
            instance->SetData(DATA_NEFARIAN, IN_PROGRESS);
        }

        void JustDied(Unit* /*Killer*/)
        {
            instance->SetData(DATA_NEFARIAN, DONE);
        }

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;

            if (ShadowflameBreathTimer <= Diff)
            {
                me->CastSpell(me->getVictim(), SPELL_SHADOWFLAME_BREATH, true);

                ShadowflameBreathTimer = urand(10*IN_MILLISECONDS, 12*IN_MILLISECONDS);
            } else ShadowflameBreathTimer -= Diff;

            if (ShadowCowardiceTimer <= Diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    me->CastSpell(target, SPELL_SHADOW_COWARDICE , true);

                ShadowCowardiceTimer = urand(8*IN_MILLISECONDS, 9*IN_MILLISECONDS);
            } else ShadowCowardiceTimer -= Diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_bd_nefarian()
{
    new boss_bd_nefarian();
}
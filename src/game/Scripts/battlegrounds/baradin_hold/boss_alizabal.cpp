class boss_alizabal : public CreatureScript
{
public:
    boss_alizabal() : CreatureScript("boss_alizabal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_alizabalAI (creature);
    }

    struct boss_alizabalAI : public ScriptedAI
    {
        boss_alizabalAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        void Reset() {}

        void EnterCombat(Unit* /*pWho*/) {}

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_alizabal()
{
    new boss_alizabal();
}
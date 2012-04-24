class boss_occuthar : public CreatureScript
{
public:
    boss_occuthar() : CreatureScript("boss_occuthar") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_occutharAI (creature);
    }

    struct boss_occutharAI : public ScriptedAI
    {
        boss_occutharAI(Creature* creature) : ScriptedAI(creature)
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

void AddSC_boss_occuthar()
{
    new boss_occuthar();
}
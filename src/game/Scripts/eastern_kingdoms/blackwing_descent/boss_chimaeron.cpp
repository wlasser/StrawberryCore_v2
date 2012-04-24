class boss_chimaeron : public CreatureScript
{
public:
    boss_chimaeron() : CreatureScript("boss_chimaeron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_chimaeronAI (creature);
    }

    struct boss_chimaeronAI : public ScriptedAI
    {
        boss_chimaeronAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset() { }

        void EnterCombat(Unit* /*who*/) {}

        void JustDied(Unit* /*Killer*/)
        {
            instance->SetData(DATA_CHIMAERON, DONE);
        }

        void UpdateAI(const uint32 Diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_chimaeron()
{
    new boss_chimaeron();
}
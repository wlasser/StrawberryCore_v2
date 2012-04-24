class boss_feludius : public CreatureScript
{
    public:
        boss_feludius() : CreatureScript("boss_feludius") { }

        struct boss_feludiusAI : BossAI
        {
            boss_feludiusAI(Creature * creature) : BossAI(creature,DATA_FELUDIUS), summons(me)
            {
                pInstance = (InstanceScript*)creature->GetInstanceScript();
            }

            void Reset()
            {
                uiHydrolanceTimer = 11000;
                uiWaterBombTimer = 16000;
                uiHeartofIceTimer = 39000;
                uiGlaciateTimer = 32000;
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (uiHydrolanceTimer <= uiDiff)
                {
                    uiHydrolanceTimer = 11000;
                    Unit * Victim = SelectTarget(SELECT_TARGET_RANDOM);
                    DoCast(Victim,SPELL_HYDROLANCE);
                } else uiHydrolanceTimer -= uiDiff;
                if (uiWaterBombTimer <= uiDiff)
                {
                    uiHydrolanceTimer = 11000;
                    DoCast(SPELL_WATERBOMB);
                } else uiWaterBombTimer -= uiDiff;
                if (uiHeartofIceTimer <= uiDiff)
                {
                    uiHeartofIceTimer = 39000;
                    Unit * Victim = SelectTarget(SELECT_TARGET_RANDOM);
                    DoCast(Victim,SPELL_HEART_OF_ICE);
                } else uiHeartofIceTimer -= uiDiff;
                if (uiGlaciateTimer <= uiDiff)
                {
                    uiGlaciateTimer = 32000;
                    DoCast(SPELL_GLACIATE);
                } else uiGlaciateTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* pInstance;
            SummonList summons;

            uint32 uiHydrolanceTimer;
            uint32 uiWaterBombTimer;
            uint32 uiHeartofIceTimer;
            uint32 uiGlaciateTimer;
        };

        CreatureAI * GetAI(Creature* creature) const
        {
            return new boss_feludiusAI(creature);
        }
};

class boss_ignacious : public CreatureScript
{
    public:
        boss_ignacious() : CreatureScript("boss_ignacious") { }

        struct boss_ignaciousAI : public BossAI
        {
            boss_ignaciousAI(Creature * creature) : BossAI(creature,DATA_IGNACIOUS), summons(me)
            {
                pInstance = (InstanceScript*)creature->GetInstanceScript();
            }

            void Reset()
            {
                uiBurningBloodTimer = 31000;
                uiFlameTorrentTimer = 10000;
                uiAegisofFlameTimer = 54000;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch(id)
                    {
                        case POINT_THERALION_TAKEOFF:
                            me->GetMotionMaster()->Clear(false);
                            me->GetMotionMaster()->MoveIdle();
                        //case POINT_THERALION_LAND:
                    }
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(uiBurningBloodTimer <= uiDiff)
                {
                    uiBurningBloodTimer = 31000;
                    DoCastVictim(SPELL_BURNING_BLOOD,false);
                } else uiBurningBloodTimer -= uiDiff;

                if (uiFlameTorrentTimer <= uiDiff)
                {
                    uiFlameTorrentTimer = 10000;
                    DoCast(SPELL_FLAME_TORRENT);
                } else uiFlameTorrentTimer -= uiDiff;

                if(uiAegisofFlameTimer <= uiDiff)
                {
                    uiAegisofFlameTimer = 54000;
                    DoCast(me,SPELL_AEGIS_OF_FLAMES);
                } else uiAegisofFlameTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

            private:
                InstanceScript * pInstance;
                SummonList summons;
                uint64 uiBurningBloodTimer;
                uint64 uiFlameTorrentTimer;
                uint64 uiAegisofFlameTimer;
                uint64 uiInfernoLeapTimer;
        };

        CreatureAI * GetAI(Creature * creature) const
        {
            return new boss_ignaciousAI(creature);
        }
};
class spell_inferno_ping : public SpellScriptLoader
{
    spell_inferno_ping() : SpellScriptLoader("spell_inferno_ping") { }

    class spell_inferno_pingSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_inferno_pingSpellScript);

        bool Validate(SpellEntry * /* spellEntry*/)
        {
            return true;
        }

        void Summon(Unit * pCaster)
        {
            pCaster->CastSpell(pCaster->GetPositionX(),pCaster->GetPositionY(),pCaster->GetPositionZ(),87650,true);
        }

        void HandlerDummy()
        {
            Summon(GetCaster());
        }

        void Register()
        {
        }
    };

    SpellScript * GetSpellScript() const
    {
        return new spell_inferno_pingSpellScript();
    }
};
void AddSC_boss_ascendant_council()
{
    new boss_feludius();
    new boss_ignacious();
}
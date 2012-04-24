class instance_baradin_hold: public InstanceMapScript
{
public:
    instance_baradin_hold() : InstanceMapScript("instance_baradin_hold", 757) { }

    InstanceScript* GetInstanceScript(InstanceMap *map) const
    {
        return new instance_baradin_hold_InstanceMapScript(map);
    }

    struct instance_baradin_hold_InstanceMapScript: public InstanceScript
    {
        instance_baradin_hold_InstanceMapScript(InstanceMap *map) : InstanceScript(map)
        {
            SetBossNumber(MAX_ENCOUNTER);
        }
    };
};

void AddSC_instance_baradin_hold()
{
    new instance_baradin_hold();
}
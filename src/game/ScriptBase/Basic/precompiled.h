/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_PRECOMPILED_H
#define SC_PRECOMPILED_H

#include "../ScriptMgr.h"
#include "Object.h"
#include "Unit.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "GameObject.h"
#include "../ScriptBase/Basic/sc_creature.h"
#include "../ScriptBase/Basic/sc_gossip.h"
#include "../ScriptBase/Basic/sc_grid_searchers.h"
#include "../ScriptBase/Basic/sc_instance.h"
#include "../ScriptBase/AI/escort_ai.h"
#include "../ScriptBase/AI/pet_ai.h"
#include "../ScriptBase/Event/EventScripts.h"
#include "../SpellAuras.h"

#ifdef WIN32
#  include <windows.h>
    BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
    {
        return true;
    }
#endif

#endif

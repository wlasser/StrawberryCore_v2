/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
 *
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

#ifndef __BATTLEFIELD_MGR_H
#define __BATTLEFIELD_MGR_H

#include "Battlefield.h"
#include "Player.h"
#include "Log.h"
#include "Opcodes.h"
#include "ObjectAccessor.h"
#include "WorldPacket.h"
#include "Common.h"
#include "SharedDefines.h"
#include "Policies/Singleton.h"
#include "ace/Recursive_Thread_Mutex.h"
#include <map>

class Player;
class Battlefield;

typedef std::list<Player*> PlayerQueue;
typedef std::map<uint64,Battlefield* > BattlefieldList;

class BattlefieldMgr
{
    friend class Battlefield;

    public:
        BattlefieldMgr();
        ~BattlefieldMgr();

        void Initialize();
        void Update(uint32 uiDiff);
        void AddPlayerToQueue(Player* plr);

        Battlefield * FindBattlefield(uint64 guid);

        void SendQueueRequestResponse(Player* plr,bool canJoin);
        void SendInvitePlayerToQueue(Player * player);

    private:
        PlayerQueue             m_queue;
        BattlefieldList         m_battlefieldList;

};

#define sBattlefieldMgr Strawberry::Singleton<BattlefieldMgr>::Instance()
#endif
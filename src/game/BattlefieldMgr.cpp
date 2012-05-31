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

#include "BattlefieldMgr.h"

INSTANTIATE_SINGLETON_1(BattlefieldMgr);

BattlefieldMgr::BattlefieldMgr()
{

}

BattlefieldMgr::~BattlefieldMgr()
{

}

void BattlefieldMgr::Initialize()
{
    sLog.outDebug("Creating battlefields");
}

void BattlefieldMgr::Update(uint32 uiDiff)
{
    sLog.outDebug("Updating the Battlefields");
    for(BattlefieldList::iterator itr = m_battlefieldList.begin(); itr != m_battlefieldList.end(); ++itr)
    {
        (*itr).second->Update(uiDiff);
    }
}

void BattlefieldMgr::AddPlayerToQueue(Player* plr)
{
    m_queue.push_back(plr);
}

void BattlefieldMgr::SendQueueRequestResponse(Player* plr,bool canJoin)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_REQ_RESP,11);

    data << uint8(0);
    data << uint32(4197);
    data << uint64(1);
    data << uint64(plr->GetObjectGuid().GetRawValue());
    data << uint8(1);
    data << uint8(1);
}

void BattlefieldMgr::SendInvitePlayerToQueue(Player * player)
{

}
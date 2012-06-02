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

#include "WorldSession.h"
#include "BattlefieldMgr.h"
#include "ObjectGuid.h"

void WorldSession::HandleBattlefieldJoinQueueOpcode( WorldPacket &recv_data )
{
    uint64 playerGuid = 0;
    bool canJoin = true;

    BitStream mask = recv_data.ReadBitStream(8);

    ByteBuffer bytes(8, true);

    if (mask[4]) bytes[6] = recv_data.ReadUInt8() ^ 1;
    if (mask[1]) bytes[3] = recv_data.ReadUInt8() ^ 1;
    if (mask[5]) bytes[2] = recv_data.ReadUInt8() ^ 1;
    if (mask[3]) bytes[4] = recv_data.ReadUInt8() ^ 1;
    if (mask[2]) bytes[7] = recv_data.ReadUInt8() ^ 1;
    if (mask[6]) bytes[1] = recv_data.ReadUInt8() ^ 1;
    if (mask[7]) bytes[5] = recv_data.ReadUInt8() ^ 1;
    if (mask[0]) bytes[0] = recv_data.ReadUInt8() ^ 1;

    playerGuid = BitConverter::ToUInt64(bytes);
    //ObjectGuid playerGuid;
    //recv_data >> playerGuid.ReadAsPacked();
    sLog.outDebug( "Player with guid: %u ", playerGuid);
    Player * plr = ObjectAccessor::FindPlayer(playerGuid);
    /*if(plr->HasFreeBattleGroundQueueId())
        canJoin = true;*/

    sBattlefieldMgr.SendQueueRequestResponse(plr,canJoin);
}
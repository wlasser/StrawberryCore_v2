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
    ObjectGuid battlefieldGuid;
    bool canJoin = false;

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

    battlefieldGuid = ObjectGuid(HIGHGUID_BATTLEFIELD,(uint32)BitConverter::ToUInt64(bytes));

    WorldPacket send_data(SMSG_BATTLEFIELD_MGR_QUEUE_REQ_RESP,11);
    
    Battlefield * Battlefield = sBattlefieldMgr.FindBattlefield(battlefieldGuid);
    BattlefieldQueue * Queue = sBattlefieldMgr.GetQueueForBattlefield(battlefieldGuid);
    uint8 BattlefieldGuidMask[] = { 1, 6, 5, 7, 0, 4, 3, 2};
    uint8 BattlefieldBytes[] = { 1, 3, 6, 7, 0, 2, 4, 5};
    uint8 QueueGuidMask[] = { 7, 3, 0, 4, 2, 6, 1, 5};
    uint8 QueueGuidBytes[] = { 2, 5, 3, 0, 4, 6, 1, 7};

    send_data.WriteGuidMask(Battlefield->getGuid(),BattlefieldGuidMask,4);
    send_data.WriteBit(0);      //logging in
    send_data.WriteGuidMask(Battlefield->getGuid(),BattlefieldGuidMask,1,4);
    if(Queue)
    {
        send_data.WriteBit(1);
    }
    else
    {
        send_data.WriteBit(0);
    }
    send_data.WriteGuidMask(Battlefield->getGuid(),BattlefieldGuidMask,1,5); //correct do not touch!!

    if(Queue)
    {
        send_data.WriteGuidMask(Queue->GetId(),QueueGuidMask,8);
    }

    send_data.WriteGuidMask(Battlefield->getGuid(),BattlefieldGuidMask,2,6);
    send_data.FlushBits();

    if(Queue)
    {
        send_data.WriteGuidBytes(Queue->GetId(),QueueGuidBytes,8,0);
    }
	send_data << uint8(1); //can join
    send_data.WriteGuidBytes(Battlefield->getGuid(),BattlefieldBytes,5,0);
	send_data << uint8(0); // warmup
	send_data.WriteGuidBytes(Battlefield->getGuid(),BattlefieldBytes,3,5);

    send_data << uint32(Battlefield->GetZoneId());

    SendPacket(&send_data);

    Queue->AddPlayerToQueue(_player);
}

void WorldSession::HandleBattlefieldEntryInviteResponseOpcode( WorldPacket &recv_data )
{
    bool mask[8];
    mask[6] = recv_data.ReadBit();
    mask[1] = recv_data.ReadBit();
    bool accepted = recv_data.ReadBit();
    mask[5] = recv_data.ReadBit();
    mask[3] = recv_data.ReadBit();
    mask[2] = recv_data.ReadBit();
    mask[0] = recv_data.ReadBit();
    mask[7] = recv_data.ReadBit();
    mask[4] = recv_data.ReadBit();
    ByteBuffer bytes(8,true);

    if(mask[0]) bytes[0] = recv_data.ReadUInt8() ^ 1;
    if(mask[3]) bytes[3] = recv_data.ReadUInt8() ^ 1;
    if(mask[4]) bytes[4] = recv_data.ReadUInt8() ^ 1;
    if(mask[2]) bytes[2] = recv_data.ReadUInt8() ^ 1;
    if(mask[1]) bytes[1] = recv_data.ReadUInt8() ^ 1;
    if(mask[6]) bytes[6] = recv_data.ReadUInt8() ^ 1;
    if(mask[7]) bytes[7] = recv_data.ReadUInt8() ^ 1;
    if(mask[5]) bytes[5] = recv_data.ReadUInt8() ^ 1;

    ObjectGuid battlefieldGuid = ObjectGuid(HIGHGUID_BATTLEFIELD,(uint32)BitConverter::ToUInt64(bytes));

    if(accepted)
    {
        Battlefield * battlefield = sBattlefieldMgr.FindBattlefield(battlefieldGuid);
        WorldPacket send_data(SMSG_BATTLEFIELD_MGR_ENTERING,21);

        uint8 guidMask[] = { 0, 6, 3, 1, 7, 5, 2, 4 };
        uint8 guidBytes[] = { 4, 1, 6, 5, 2, 7, 3, 0 };

        send_data.WriteGuidMask(battlefieldGuid,guidMask,8);
        send_data << uint32(1);
        send_data.WriteGuidBytes(battlefieldGuid,guidBytes,2,0);
        send_data << uint32(battlefield->GetZoneId());
        send_data << uint32(_player->isAFK() ? 1 : 0);
        send_data.WriteGuidBytes(battlefieldGuid,guidBytes,6,2);

        battlefield->AddPlayerToGroup(_player);
        SendPacket(&send_data);
    }

    sBattlefieldMgr.GetQueueForBattlefield(battlefieldGuid)->RemovePlayerFromQueue(_player);
}
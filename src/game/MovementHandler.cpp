/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "Vehicle.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Transports.h"
#include "BattleGround.h"
#include "WaypointMovementGenerator.h"
#include "MapPersistentStateMgr.h"
#include "ObjectMgr.h"
#include "MovementStructures.h"
#include <G3D/Vector3.h>

void WorldSession::HandleMoveWorldportAckOpcode( WorldPacket & /*recv_data*/ )
{
    DEBUG_LOG( "WORLD: got MSG_MOVE_WORLDPORT_ACK." );
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if(!GetPlayer()->IsBeingTeleportedFar())
        return;

    if (_player->GetVehicleKit())
        _player->GetVehicleKit()->RemoveAllPassengers();

    // get start teleport coordinates (will used later in fail case)
    WorldLocation old_loc;
    GetPlayer()->GetPosition(old_loc);

    // get the teleport destination
    WorldLocation &loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check (only cheating case possible)
    if (!MapManager::IsValidMapCoord(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation))
    {
        sLog.outError("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to a not valid location "
            "(map:%u, x:%f, y:%f, z:%f) We port him to his homebind instead..",
            GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);
        // stop teleportation else we would try this again and again in LogoutPlayer...
        GetPlayer()->SetSemaphoreTeleportFar(false);
        // and teleport the player to a valid place
        GetPlayer()->TeleportToHomebind();
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.mapid);

    Map* map = NULL;

    // prevent crash at attempt landing to not existed battleground instance
    if(mEntry->IsBattleGroundOrArena())
    {
        if (GetPlayer()->GetBattleGroundId())
            map = sMapMgr.FindMap(loc.mapid, GetPlayer()->GetBattleGroundId());

        if (!map)
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to nonexisten battleground instance "
                " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
                GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

            GetPlayer()->SetSemaphoreTeleportFar(false);

            // Teleport to previous place, if cannot be ported back TP to homebind place
            if (!GetPlayer()->TeleportTo(old_loc))
            {
                DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                    GetPlayer()->GetGuidStr().c_str());
                GetPlayer()->TeleportToHomebind();
            }
            return;
        }
    }

    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(loc.mapid);

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    // relocate the player to the teleport destination
    if (!map)
        map = sMapMgr.CreateMap(loc.mapid, GetPlayer());

    GetPlayer()->SetMap(map);
    GetPlayer()->Relocate(loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!GetPlayer()->GetMap()->Add(GetPlayer()))
    {
        // if player wasn't added to map, reset his map pointer!
        GetPlayer()->ResetMap();

        DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far but couldn't be added to map "
            " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
            GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

        // Teleport to previous place, if cannot be ported back TP to homebind place
        if (!GetPlayer()->TeleportTo(old_loc))
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                GetPlayer()->GetGuidStr().c_str());
            GetPlayer()->TeleportToHomebind();
        }
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if(_player->InBattleGround())
    {
        // cleanup setting if outdated
        if(!mEntry->IsBattleGroundOrArena())
        {
            // We're not in BG
            _player->SetBattleGroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(TEAM_NONE);
        }
        // join to bg case
        else if(BattleGround *bg = _player->GetBattleGround())
        {
            if(_player->IsInvitedForBattleGroundInstance(_player->GetBattleGroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if(!_player->InBattleGround())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Reset(*GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->m_taxi.ClearTaxiDestinations();
    }

    if (mInstance)
    {
        Difficulty diff = GetPlayer()->GetDifficulty(mEntry->IsRaid());
        if(MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID,diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sMapPersistentStateMgr.GetScheduler().GetResetTimeFor(mEntry->MapID,diff))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    GetPlayer()->SendInstanceResetWarning(mEntry->MapID, diff, timeleft);
                }
            }
        }
    }

    // mount allow check
    if(!mEntry->IsMountAllowed())
        _player->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

    // honorless target
    if(GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAckOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("MSG_MOVE_TELEPORT_ACK");

    uint32 counter, time;
    recv_data >> counter >> time;

    BitStream mask = recv_data.ReadBitStream(8);
    ByteBuffer bytes(8, true);

    if (mask[7]) bytes[4] = recv_data.ReadUInt8() ^ 1;
    if (mask[6]) bytes[2] = recv_data.ReadUInt8() ^ 1;
    if (mask[5]) bytes[7] = recv_data.ReadUInt8() ^ 1;
    if (mask[3]) bytes[6] = recv_data.ReadUInt8() ^ 1;
    if (mask[0]) bytes[5] = recv_data.ReadUInt8() ^ 1;
    if (mask[2]) bytes[1] = recv_data.ReadUInt8() ^ 1;
    if (mask[4]) bytes[3] = recv_data.ReadUInt8() ^ 1;
    if (mask[1]) bytes[0] = recv_data.ReadUInt8() ^ 1;

    ObjectGuid guid = (ObjectGuid)BitConverter::ToUInt64(bytes);

    DEBUG_LOG("Guid: %s", guid.GetString().c_str());
    DEBUG_LOG("Counter %u, time %u", counter, time/IN_MILLISECONDS);

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    if(!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if(guid != plMover->GetObjectGuid())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->SetPosition(dest.coord_x, dest.coord_y, dest.coord_z, dest.orientation, true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if(old_zone != newzone)
    {
        // honorless target
        if(plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes( WorldPacket & recv_data )
{
    Opcodes opcode = (Opcodes)recv_data.GetOpcode();
    DEBUG_LOG("WORLD: Recvd %s (%u, 0x%X) opcode", LookupOpcodeName(opcode), opcode, opcode);

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plMover && plMover->IsBeingTeleported())
    {
        recv_data.rfinish();                   // prevent warnings spam
        return;
    }

    /* extract packet */
    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);

    /*----------------*/

    if (!VerifyMovementInfo(movementInfo, movementInfo.guid))
        return;

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plMover && !plMover->IsTaxiFlying())
        plMover->HandleFall(movementInfo);

    /* process position-change */
    HandleMoverRelocation(movementInfo);

    if (plMover)
        plMover->UpdateFallInformationIfNeed(movementInfo, opcode);

    WorldPacket data(SMSG_PLAYER_MOVE, recv_data.size());
    WriteMovementInfo(data, &movementInfo);
    mover->SendMessageToSetExcept(&data, _player);
}

void WorldSession::ReadMovementInfo(WorldPacket &data, MovementInfo *mi)
{
    bool HaveTransportData = false,
        HaveTransportTime2 = false,
        HaveTransportTime3 = false,
        HaveMovementFlags = false,
        HaveMovementFlags2 = false,
        HaveOrientation = false,
        HaveTimeStamp = false,
        HavePitch = false,
        HaveFallData = false,
        HaveFallDirection = false,
        HaveSplineElevation = false,
        HaveUnknownBit = false,
        HaveSpline = false;

    MovementStatusElements *sequence = GetMovementStatusElementsSequence(data.GetOpcodeEnum());
    if(sequence == NULL)
        return;

    ByteBuffer guidBytes(8, true);
    ByteBuffer tGuidBytes(8, true);
    for(uint32 i = 0; i < MSE_COUNT; ++i)
    {
        MovementStatusElements element = sequence[i];
        if (element == MSEEnd)
            break;

        if (element >= MSEGuidByte0 && element <= MSEGuidByte7)
        {
            data.ReadByteMask(guidBytes[element - MSEGuidByte0]);
            continue;
        }

        if (element >= MSETransportGuidByte0 && element <= MSETransportGuidByte7)
        {
            if (HaveTransportData)
                data.ReadByteMask(tGuidBytes[element - MSETransportGuidByte0]);

            continue;
        }

        if (element >= MSEGuidByte0_2 && element <= MSEGuidByte7_2)
        {
            data.ReadByteSeq(guidBytes[element - MSEGuidByte0_2]);
            continue;
        }

        if (element >= MSETransportGuidByte0_2 && element <= MSETransportGuidByte7_2)
        {
            if (HaveTransportData)
                data.ReadByteSeq(tGuidBytes[element - MSETransportGuidByte0_2]);

            continue;
        }

        switch (element)
        {
            case MSEFlags:
                if (HaveMovementFlags)
                    mi->moveFlags = data.ReadBits(30);
                break;
            case MSEFlags2:
                if (HaveMovementFlags2)
                    mi->moveFlags2 = data.ReadBits(12);
                break;
            case MSEHaveUnknownBit:
                HaveUnknownBit = data.ReadBit();
                break;
            case MSETimestamp:
                if (HaveTimeStamp)
                    data >> mi->time;
                break;
            case MSEHaveTimeStamp:
                HaveTimeStamp = !data.ReadBit();
                break;
            case MSEHaveOrientation:
                HaveOrientation = !data.ReadBit();
                break;
            case MSEHaveMovementFlags:
                HaveMovementFlags = !data.ReadBit();
                break;
            case MSEHaveMovementFlags2:
                HaveMovementFlags2 = !data.ReadBit();
                break;
            case MSEHavePitch:
                HavePitch = !data.ReadBit();
                break;
            case MSEHaveFallData:
                HaveFallData = data.ReadBit();
                break;
            case MSEHaveFallDirection:
                if (HaveFallData)
                    HaveFallDirection = data.ReadBit();
                break;
            case MSEHaveTransportData:
                HaveTransportData = data.ReadBit();
                break;
            case MSETransportHaveTime2:
                if (HaveTransportData)
                    HaveTransportTime2 = data.ReadBit();
                break;
            case MSETransportHaveTime3:
                if (HaveTransportData)
                    HaveTransportTime3 = data.ReadBit();
                break;
            case MSEHaveSpline:
                HaveSpline = data.ReadBit();
                break;
            case MSEHaveSplineElev:
                HaveSplineElevation = !data.ReadBit();
                break;
            case MSEPositionX:
                data >> mi->pos.x;
                break;
            case MSEPositionY:
                data >> mi->pos.y;
                break;
            case MSEPositionZ:
                data >> mi->pos.z;
                break;
            case MSEPositionO:
                if (HaveOrientation)
                    data >> mi->pos.o;
                break;
            case MSEPitch:
                if (HavePitch)
                    data >> mi->s_pitch;
                break;;
            case MSEFallTime:
                if (HaveFallData)
                    data >> mi->fallTime;
                break;
            case MSESplineElev:
                if (HaveSplineElevation)
                    data >> mi->splineElevation;
                break;
            case MSEFallHorizontalSpeed:
                if (HaveFallData && HaveFallDirection)
                    data >> mi->jump.xyspeed;
                break;
            case MSEFallVerticalSpeed:
                if (HaveFallData)
                    data >> mi->jump.velocity;
                break;
            case MSEFallCosAngle:
                if (HaveFallData && HaveFallDirection)
                    data >> mi->jump.cosAngle;
                break;
            case MSEFallSinAngle:
                if (HaveFallData && HaveFallDirection)
                    data >> mi->jump.sinAngle;
                break;
            case MSETransportSeat:
                if (HaveTransportData)
                    data >> mi->t_seat;
                break;
            case MSETransportPositionO:
                if (HaveTransportData)
                    data >> mi->t_pos.o;
                break;
            case MSETransportPositionX:
                if (HaveTransportData)
                    data >> mi->pos.x;
                break;
            case MSETransportPositionY:
                if (HaveTransportData)
                    data >> mi->pos.y;
                break;
            case MSETransportPositionZ:
                if (HaveTransportData)
                    data >> mi->pos.z;
                break;
            case MSETransportTime:
                if (HaveTransportData)
                    data >> mi->t_time;
                break;
            case MSETransportTime2:
                if (HaveTransportData && HaveTransportTime2)
                    data >> mi->t_time2;
                break;
            case MSETransportTime3:
                if (HaveTransportData && HaveTransportTime3)
                    data >> mi->fallTime;
                break;
            default:
                WPError(false);
        }
    }

    mi->guid = ObjectGuid(BitConverter::ToUInt64(guidBytes));
    mi->t_guid = ObjectGuid(BitConverter::ToUInt64(tGuidBytes));

    if (HaveTransportData && mi->pos.x != mi->t_pos.x)
        if (GetPlayer()->GetTransport())
            GetPlayer()->GetTransport()->m_position = mi->pos;
}

void WorldSession::WriteMovementInfo(WorldPacket &data, MovementInfo *mi)
{
    bool HaveTransportData = mi->t_guid,
        HaveTransportTime2 = (mi->moveFlags2 & MOVEFLAG2_INTERP_MOVEMENT) != 0,
        HaveTransportTime3 = false,
        HaveTime = mi->time,
        HaveOrientation = !G3D::fuzzyEq(mi->pos.o, 0.0f),
        HavePitch = (mi->HasMovementFlag(MovementFlags(MOVEFLAG_SWIMMING | MOVEFLAG_FLYING))) || (mi->moveFlags2 & MOVEFLAG2_ALLOW_PITCHING),
        HaveFallData = mi->HasMovementFlag2(MOVEFLAG2_INTERP_TURNING),
        HaveFallDirection = mi->HasMovementFlag(MOVEFLAG_FALLING),
        HaveSplineElevation = mi->HasMovementFlag(MOVEFLAG_SPLINE_ELEVATION),
        HaveSpline = false;

    MovementStatusElements *sequence = GetMovementStatusElementsSequence(data.GetOpcodeEnum());
    if(!sequence)
        return;

    ByteBuffer guidBytes(8, true);
    ByteBuffer tGuidBytes(8, true);
    guidBytes << mi->guid;
    tGuidBytes << mi->t_guid;

    for(uint32 i = 0; i < MSE_COUNT; ++i)
    {
        MovementStatusElements element = sequence[i];

        if (element == MSEEnd)
            break;

        if (element >= MSEGuidByte0 && element <= MSEGuidByte7)
        {
            data.WriteByteMask(guidBytes[element - MSEGuidByte0]);
            continue;
        }

        if (element >= MSETransportGuidByte0 && element <= MSETransportGuidByte7)
        {
            if (HaveTransportData)
                data.WriteByteMask(tGuidBytes[element - MSETransportGuidByte0]);
            continue;
        }

        if (element >= MSEGuidByte0_2 && element <= MSEGuidByte7_2)
        {
            data.WriteByteSeq(guidBytes[element - MSEGuidByte0_2]);
            continue;
        }

        if (element >= MSETransportGuidByte0_2 && element <= MSETransportGuidByte7_2)
        {
            if (HaveTransportData)
                data.WriteByteSeq(tGuidBytes[element - MSETransportGuidByte0_2]);
            continue;
        }

        switch (element)
        {
            case MSEHaveMovementFlags:
                data.WriteBit(!mi->GetMovementFlags());
                break;
            case MSEHaveMovementFlags2:
                data.WriteBit(!mi->GetMovementFlags2());
                break;
            case MSEFlags:
                if (mi->GetMovementFlags())
                    data.WriteBits(mi->moveFlags, 30);
                break;
            case MSEFlags2:
                if (mi->GetMovementFlags2())
                    data.WriteBits(mi->moveFlags2, 12);
                break;
            case MSETimestamp:
                if (HaveTime)
                    data << mi->time;
                break;
            case MSEHavePitch:
                data.WriteBit(!HavePitch);
                break;
            case MSEHaveTimeStamp:
                data.WriteBit(!HaveTime);
                break;
            case MSEHaveUnknownBit:
                data.WriteBit(false);
                break;
            case MSEHaveFallData:
                data.WriteBit(HaveFallData);
                break;
            case MSEHaveFallDirection:
                if (HaveFallData)
                    data.WriteBit(HaveFallDirection);
                break;
            case MSEHaveTransportData:
                data.WriteBit(HaveTransportData);
                break;
            case MSETransportHaveTime2:
                if (HaveTransportData)
                    data.WriteBit(HaveTransportTime2);
                break;
            case MSETransportHaveTime3:
                if (HaveTransportData)
                    data.WriteBit(HaveTransportTime3);
                break;
            case MSEHaveSpline:
                data.WriteBit(HaveSpline);
                break;
            case MSEHaveSplineElev:
                data.WriteBit(!HaveSplineElevation);
                break;
            case MSEPositionX:
                data << mi->pos.x;
                break;
            case MSEPositionY:
                data << mi->pos.y;
                break;
            case MSEPositionZ:
                data << mi->pos.z;
                break;
            case MSEPositionO:
                if (HaveOrientation)
                    data << mi->pos.o;
                break;
            case MSEPitch:
                if (HavePitch)
                    data << mi->s_pitch;
                break;
            case MSEHaveOrientation:
                data.WriteBit(!HaveOrientation);
                break;
            case MSEFallTime:
                if (HaveFallData)
                    data << mi->fallTime;
                break;
            case MSESplineElev:
                if (HaveSplineElevation)
                    data << mi->splineElevation;
                break;
            case MSEFallHorizontalSpeed:
                if (HaveFallData && HaveFallDirection)
                    data << mi->jump.xyspeed;
                break;
            case MSEFallVerticalSpeed:
                if (HaveFallData)
                    data << mi->jump.velocity;
                break;
            case MSEFallCosAngle:
                if (HaveFallData && HaveFallDirection)
                    data << mi->jump.cosAngle;
                break;
            case MSEFallSinAngle:
                if (HaveFallData && HaveFallDirection)
                    data << mi->jump.sinAngle;
                break;
            case MSETransportSeat:
                if (HaveTransportData)
                    data << mi->t_seat;
                break;
            case MSETransportPositionO:
                if (HaveTransportData)
                    data << mi->t_pos.o;
                break;
            case MSETransportPositionX:
                if (HaveTransportData)
                    data << mi->pos.x;
                break;
            case MSETransportPositionY:
                if (HaveTransportData)
                    data << mi->pos.y;
                break;
            case MSETransportPositionZ:
                if (HaveTransportData)
                    data << mi->pos.z;
                break;
            case MSETransportTime:
                if (HaveTransportData)
                    data << mi->t_time;
                break;
            case MSETransportTime2:
                if (HaveTransportData && HaveTransportTime2)
                    data << mi->t_time2;
                break;
            case MSETransportTime3:
                if (HaveTransportData && HaveTransportTime3)
                    data << mi->fallTime;
                break;
            default:
                WPError(false);
        }
    }
}

void WorldSession::HandleForceSpeedChangeAckOpcodes(WorldPacket &recv_data)
{
    uint32 opcode = recv_data.GetOpcode();
    DEBUG_LOG("WORLD: Recvd %s (%u, 0x%X) opcode", LookupOpcodeName(opcode), opcode, opcode);

    /* extract packet */
    ObjectGuid guid;
    uint32 unk1;
    MovementInfo movementInfo;
    float  newspeed;

    recv_data >> guid.ReadAsPacked();
    recv_data >> unk1;                                      // counter or moveEvent
    
    movementInfo.guid = guid;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data >> newspeed;

    // now can skip not our packet
    if(_player->GetObjectGuid() != guid)
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;
    UnitMoveType force_move_type;

    static char const* move_type_name[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate", "Flight", "FlightBack", "PitchRate" };

    switch(opcode)
    {
        case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:          move_type = MOVE_WALK;          force_move_type = MOVE_WALK;        break;
        case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:           move_type = MOVE_RUN;           force_move_type = MOVE_RUN;         break;
        case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:      move_type = MOVE_RUN_BACK;      force_move_type = MOVE_RUN_BACK;    break;
        case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:          move_type = MOVE_SWIM;          force_move_type = MOVE_SWIM;        break;
        case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:     move_type = MOVE_SWIM_BACK;     force_move_type = MOVE_SWIM_BACK;   break;
        case CMSG_FORCE_TURN_RATE_CHANGE_ACK:           move_type = MOVE_TURN_RATE;     force_move_type = MOVE_TURN_RATE;   break;
        case CMSG_FORCE_FLIGHT_SPEED_CHANGE_ACK:        move_type = MOVE_FLIGHT;        force_move_type = MOVE_FLIGHT;      break;
        case CMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_FLIGHT_BACK;   force_move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_FORCE_PITCH_RATE_CHANGE_ACK:          move_type = MOVE_PITCH_RATE;    force_move_type = MOVE_PITCH_RATE;  break;
        default:
            sLog.outError("WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if(_player->m_forced_speed_changes[force_move_type] > 0)
    {
        --_player->m_forced_speed_changes[force_move_type];
        if(_player->m_forced_speed_changes[force_move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if(_player->GetSpeed(move_type) > newspeed)         // must be greater - just correct
        {
            sLog.outError("%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                move_type_name[move_type], _player->GetName(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeedRate(move_type,_player->GetSpeedRate(move_type),true);
        }
        else                                                // must be lesser - cheating
        {
            BASIC_LOG("Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                _player->GetName(),_player->GetSession()->GetAccountId(),_player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket &recv_data)
{
    DEBUG_LOG("WORLD: Recvd CMSG_SET_ACTIVE_MOVER");
    recv_data.hexlike();

    ObjectGuid guid;
    recv_data >> guid;

    if(_player->GetMover()->GetObjectGuid() != guid)
    {
        sLog.outError("HandleSetActiveMoverOpcode: incorrect mover guid: mover is %s and should be %s",
            _player->GetMover()->GetGuidStr().c_str(), guid.GetString().c_str());
        return;
    }
}

void WorldSession::HandleMoveNotActiveMoverOpcode(WorldPacket &recv_data)
{
    DEBUG_LOG("WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");
    recv_data.hexlike();

    ObjectGuid old_mover_guid;
    MovementInfo mi;

    recv_data >> old_mover_guid.ReadAsPacked();
    mi.guid = old_mover_guid;
    ReadMovementInfo(recv_data, &mi);

    if(_player->GetMover()->GetObjectGuid() == old_mover_guid)
    {
        sLog.outError("HandleMoveNotActiveMover: incorrect mover guid: mover is %s and should be %s instead of %s",
            _player->GetMover()->GetGuidStr().c_str(),
            _player->GetGuidStr().c_str(),
            old_mover_guid.GetString().c_str());
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    _player->m_movementInfo = mi;
}

/*void WorldSession::HandleDismissControlledVehicle(WorldPacket &recv_data)
{
    DEBUG_LOG("WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");
    recv_data.hexlike();

    ObjectGuid guid;
    MovementInfo mi;

    recv_data >> guid.ReadAsPacked();
    recv_data >> mi;

    ObjectGuid vehicleGUID = _player->GetCharmGuid();
    if (!vehicleGUID)                                       // something wrong here...
        return;

    _player->m_movementInfo = mi;
}*/

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvdata*/)
{
    //DEBUG_LOG("WORLD: Recvd CMSG_MOUNTSPECIAL_ANIM");

    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << GetPlayer()->GetObjectGuid();

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck( WorldPacket & recv_data )
{
    DEBUG_LOG("CMSG_MOVE_KNOCK_BACK_ACK");

    Unit *mover = _player->GetMover();
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if(plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    ObjectGuid guid;
    MovementInfo movementInfo;

    recv_data >> guid.ReadAsPacked();
    recv_data.read_skip<uint32>();                          // knockback packets counter
    
    ReadMovementInfo(recv_data, &movementInfo);

    if (!VerifyMovementInfo(movementInfo, guid))
        return;

    HandleMoverRelocation(movementInfo);

    WorldPacket data(MSG_MOVE_KNOCK_BACK, recv_data.size() + 15);
    data << mover->GetPackGUID();
    //data << movementInfo;
    data << movementInfo.GetJumpInfo().sinAngle;
    data << movementInfo.GetJumpInfo().cosAngle;
    data << movementInfo.GetJumpInfo().xyspeed;
    data << movementInfo.GetJumpInfo().velocity;
    mover->SendMessageToSetExcept(&data, _player);
}

void WorldSession::HandleMoveHoverAck( WorldPacket& recv_data )
{
    DEBUG_LOG("CMSG_MOVE_HOVER_ACK");

    ObjectGuid guid;                                        // guid - unused
    MovementInfo movementInfo;

    recv_data >> guid.ReadAsPacked();
    recv_data.read_skip<uint32>();                          // unk1

    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recv_data)
{
    DEBUG_LOG("CMSG_MOVE_WATER_WALK_ACK");

    ObjectGuid guid;                                        // guid - unused
    MovementInfo movementInfo;

    recv_data >> guid.ReadAsPacked();
    recv_data.read_skip<uint32>();                          // unk1

    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recv_data)
{
    if (!_player->isAlive() || _player->isInCombat())
        return;

    ObjectGuid summonerGuid;
    bool agree;
    recv_data >> summonerGuid;
    recv_data >> agree;

    _player->SummonIfPossible(agree);
}

bool WorldSession::VerifyMovementInfo(MovementInfo const& movementInfo, ObjectGuid const& guid) const
{
    // ignore wrong guid (player attempt cheating own session for not own guid possible...)
    if (guid != _player->GetMover()->GetObjectGuid())
        return false;

    if (!Strawberry::IsValidMapCoord(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o))
        return false;

    if (movementInfo.t_guid)
    {
        // transports size limited
        // (also received at zeppelin/lift leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if( movementInfo.GetTransportPos()->x > 50 || movementInfo.GetTransportPos()->y > 50 || movementInfo.GetTransportPos()->z > 100 )
            return false;

        if( !Strawberry::IsValidMapCoord(movementInfo.GetPos()->x + movementInfo.GetTransportPos()->x, movementInfo.GetPos()->y + movementInfo.GetTransportPos()->y,
            movementInfo.GetPos()->z + movementInfo.GetTransportPos()->z, movementInfo.GetPos()->o + movementInfo.GetTransportPos()->o) )
        {
            return false;
        }
    }

    return true;
}

void WorldSession::HandleMoverRelocation(MovementInfo& movementInfo)
{
    movementInfo.UpdateTime(WorldTimer::getMSTime());

    Unit *mover = _player->GetMover();

    if (Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL)
    {
        if (movementInfo.t_guid)
        {
            if (!plMover->GetTransport())
            {
                // elevators also cause the client to send transport guid - just unmount if the guid can be found in the transport list
                for (MapManager::TransportSet::const_iterator iter = sMapMgr.m_Transports.begin(); iter != sMapMgr.m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetObjectGuid() == movementInfo.GetTransportGuid())
                    {
                        plMover->m_transport = (*iter);
                        (*iter)->AddPassenger(plMover);

                        if (plMover->GetVehicleKit())
                            plMover->GetVehicleKit()->RemoveAllPassengers();

                        break;
                    }
                }
            }
        }
        else if (plMover->GetTransport())               // if we were on a transport, leave
        {
            plMover->GetTransport()->RemovePassenger(plMover);
            plMover->SetTransport(NULL);
            movementInfo.ClearTransportData();
        }

        if (movementInfo.HasMovementFlag(MOVEFLAG_SWIMMING) != plMover->IsInWater())
        {
            // now client not include swimming flag in case jumping under water
            plMover->SetInWater( !plMover->IsInWater() || plMover->GetTerrain()->IsUnderWater(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z) );
        }

        plMover->SetPosition(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
        plMover->m_movementInfo = movementInfo;

        if(movementInfo.GetPos()->z < -500.0f)
        {
            if(plMover->InBattleGround()
                && plMover->GetBattleGround()
                && plMover->GetBattleGround()->HandlePlayerUnderMap(_player))
            {
                // do nothing, the handle already did if returned true
            }
            else
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if(plMover->isAlive())
                {
                    plMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, plMover->GetMaxHealth());
                    // pl can be alive if GM/etc
                    if(!plMover->isAlive())
                    {
                        // change the death state to CORPSE to prevent the death timer from
                        // starting in the next player update
                        plMover->KillPlayer();
                        plMover->BuildPlayerRepop();
                    }
                }

                // cancel the death timer here if started
                plMover->RepopAtGraveyard();
            }
        }
    }
    else                                                    // creature charmed
    {
        if (mover->IsInWorld())
        {
            mover->m_movementInfo = movementInfo;
            mover->SetPosition(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
        }
    }
}

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

#include "packet_builder.h"
#include "MoveSpline.h"
#include "../WorldPacket.h"

namespace Movement
{
    inline void operator << (ByteBuffer& b, const Vector3& v)
    {
        b << v.x << v.y << v.z;
    }

    inline void operator >> (ByteBuffer& b, Vector3& v)
    {
        b >> v.x >> v.y >> v.z;
    }

    enum MonsterMoveType
    {
        MonsterMoveNormal       = 0,
        MonsterMoveStop         = 1,
        MonsterMoveFacingSpot   = 2,
        MonsterMoveFacingTarget = 3,
        MonsterMoveFacingAngle  = 4
    };

    void PacketBuilder::WriteCommonMonsterMovePart(const MoveSpline& move_spline, WorldPacket& data)
    {
        MoveSplineFlag splineflags = move_spline.splineflags;

        /*if (mov.IsBoarded())
        {
            data.SetOpcode(SMSG_MONSTER_MOVE_TRANSPORT);
            data << mov.GetTransport()->Owner.GetPackGUID();
            data << int8(mov.m_unused.transport_seat);
        }*/

        data << uint8(0);
        data << move_spline.spline.getPoint(move_spline.spline.first());
        data << move_spline.GetId();

        switch(splineflags & MoveSplineFlag::Mask_Final_Facing)
        {
            case MoveSplineFlag::FinalTarget:
                data << uint8(MonsterMoveFacingTarget);
                data << move_spline.facing.target;
                break;
            case MoveSplineFlag::FinalOrientation:
                data << uint8(MonsterMoveFacingAngle);
                data << move_spline.facing.angle;
                break;
            case MoveSplineFlag::FinalPoint:
                data << uint8(MonsterMoveFacingSpot);
                data << move_spline.facing.f.x << move_spline.facing.f.y << move_spline.facing.f.z;
                break;
            default:
                data << uint8(MonsterMoveNormal);
                break;
        }

        // add fake Enter_Cycle flag - needed for client-side cyclic movement (client will erase first spline vertex after first cycle done)
        splineflags.enter_cycle = move_spline.isCyclic();
        data << uint32(splineflags & ~MoveSplineFlag::Mask_No_Monster_Move);

        if (splineflags.animation)
        {
            data << splineflags.getAnimationId();
            data << move_spline.effect_start_time;
        }

        data << move_spline.Duration();

        if (splineflags.parabolic)
        {
            data << move_spline.vertical_acceleration;
            data << move_spline.effect_start_time;
        }
    }

    void WriteLinearPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 last_idx = spline.getPointCount() - 3;
        const Vector3 * real_path = &spline.getPoint(1);

        data << last_idx;
        data << real_path[last_idx];   // destination
        if (last_idx > 1)
        {
            Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
            Vector3 offset;
            // first and last points already appended
            for(uint32 i = 1; i < last_idx; ++i)
            {
                offset = middle - real_path[i];
                data.appendPackXYZ(offset.x, offset.y, offset.z);
            }
        }
    }

    void WriteCatmullRomPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 3;
        data << count;
        data.append<Vector3>(&spline.getPoint(2), count);
    }

    void WriteCatmullRomCyclicPath(const Spline<int32>& spline, ByteBuffer& data)
    {
        uint32 count = spline.getPointCount() - 3;
        data << uint32(count + 1);
        data << spline.getPoint(1); // fake point, client will erase it from the spline after first cycle done
        data.append<Vector3>(&spline.getPoint(1), count);
    }

    void PacketBuilder::WriteMonsterMove(const MoveSpline& move_spline, WorldPacket& data)
    {
        WriteCommonMonsterMovePart(move_spline, data);

        const Spline<int32>& spline = move_spline.spline;
        MoveSplineFlag splineflags = move_spline.splineflags;
        if (splineflags & MoveSplineFlag::Mask_CatmullRom)
        {
            if (splineflags.cyclic)
                WriteCatmullRomCyclicPath(spline, data);
            else
                WriteCatmullRomPath(spline, data);
        }
        else
            WriteLinearPath(spline, data);
    }

    void PacketBuilder::WriteBytes(const MoveSpline& move_spline, ByteBuffer& data)
    {
        MoveSplineFlag splineFlags = move_spline.splineflags;
        uint32 nodes = move_spline.getPath().size();

        data.WriteBits(SPLINEMODE_LINEAR, 2);
        data.WriteBit(1);
        data.WriteBits(nodes, 22);
        data.WriteBits(SPLINETYPE_NORMAL, 2);

        if (splineFlags.final_target)
        {
            uint64 Guid = move_spline.facing.target;

            uint8 guidMask[] = { 4, 3, 7, 2, 6, 1, 0, 5 };
            data.WriteGuidMask(Guid, guidMask, 8);
        }

        data.WriteBit(0);
        data.WriteBits(0, 25);
    }

    void PacketBuilder::WriteData(const MoveSpline& move_spline, ByteBuffer& data)
    {
        MoveSplineFlag splineFlags = move_spline.splineflags;
        uint32 nodes = move_spline.getPath().size();

        data << move_spline.timePassed();

        if (splineFlags.orientationFixed)
            data << move_spline.facing.angle;

        if (splineFlags.final_target)
        {
            uint64 Guid = move_spline.facing.target;

            uint8 guidBytes[] = { 5, 3, 7, 1, 6, 4, 2, 0 };
            data.WriteGuidBytes(Guid, guidBytes, 8, 0);
        }

        for (uint32 i = 0; i < nodes; i++)
        {
            data << move_spline.getPath()[0].z;
            data << move_spline.getPath()[0].x;
            data << move_spline.getPath()[0].y;
        }

        if(splineFlags.flying)
            data << move_spline.facing.f.x << move_spline.facing.f.z << move_spline.facing.f.y;

        data << float(1.f);
        data << uint32(0);

        data << move_spline.effect_start_time;

        data << float(1.f);

        data << move_spline.FinalDestination().z;
        data << move_spline.FinalDestination().x;
        data << move_spline.FinalDestination().y;
        data << move_spline.GetId();
    }
}

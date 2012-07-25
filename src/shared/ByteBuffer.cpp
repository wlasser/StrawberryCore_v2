/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://strawberry-pr0jcts.com/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ByteBuffer.h"

void BitStream::Clear()
{
    _data.clear();
    _rpos = _wpos = 0;
}

uint8 BitStream::GetBit(uint32 bit)
{
    STRAWBERRY_ASSERT(_data.size() > bit);
    return _data[bit];
}

uint8 BitStream::ReadBit()
{
    STRAWBERRY_ASSERT(_data.size() < _rpos);
    uint8 b = _data[_rpos];
    ++_rpos;
    return b;
}

void BitStream::WriteBit(uint32 bit)
{
    _data.push_back(bit ? uint8(1) : uint8(0));
    ++_wpos;
}

template <typename T> void BitStream::WriteBits(T value, size_t bits)
{
    for (int32 i = bits-1; i >= 0; --i)
        WriteBit((value >> i) & 1);
}

bool BitStream::Empty()
{
    return _data.empty();
}

void BitStream::Reverse()
{
    uint32 len = GetLength();
    std::vector<uint8> b = _data;
    Clear();

    for(uint32 i = len; i > 0; --i)
        WriteBit(b[i-1]);
}

void BitStream::Print()
{
    std::stringstream ss;
    ss << "BitStream: ";
    for (uint32 i = 0; i < GetLength(); ++i)
        ss << uint32(GetBit(i)) << " ";

    sLog.outDebug(ss.str().c_str());
}

ByteBuffer::ByteBuffer(size_t res, bool init): _rpos(0), _wpos(0), _bitpos(8), _curbitval(0)
{
    if (init)
        _storage.resize(res, 0);
    else
        _storage.reserve(res);
}

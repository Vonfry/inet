//
// Copyright (C) 2020 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <algorithm>
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "inet/linklayer/ieee8021q/Ieee8021qTagHeader_m.h"
#include "inet/linklayer/ieee8021q/Ieee8021qTagHeaderSerializer.h"

namespace inet {

Register_Serializer(Ieee8021qTagHeader, Ieee8021qTagHeaderSerializer);
Register_Serializer(ShiftedIeee8021qTagHeader, ShiftedIeee8021qTagHeaderSerializer);
Register_Serializer(Ieee8021qTagHeaderTpidField, Ieee8021qTagHeaderTpidFieldSerializer);
Register_Serializer(Ieee8021qTagHeaderTciField, Ieee8021qTagHeaderTciFieldSerializer);

void Ieee8021qTagHeaderSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& header = staticPtrCast<const Ieee8021qTagHeader>(chunk);
    stream.writeUint16Be(header->getTpid());
    stream.writeUint16Be((header->getVid() & 0xFFF) |
                         ((header->getPcp() & 7) << 13) |
                         (header->getDei() ? 0x1000 : 0));
}

const Ptr<Chunk> Ieee8021qTagHeaderSerializer::deserialize(MemoryInputStream& stream) const
{
    const auto& header = makeShared<Ieee8021qTagHeader>();
    header->setTpid(stream.readUint16Be());
    uint16_t value = stream.readUint16Be();
    header->setVid(value & 0xFFF);
    header->setPcp((value >> 13) & 7);
    header->setDei((value & 0x1000) != 0);
    return header;
}

void ShiftedIeee8021qTagHeaderSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& header = staticPtrCast<const ShiftedIeee8021qTagHeader>(chunk);
    stream.writeUint16Be((header->getVid() & 0xFFF) |
                         ((header->getPcp() & 7) << 13) |
                         (header->getDei() ? 0x1000 : 0));
    stream.writeUint16Be(header->getTypeOrLength());
}

const Ptr<Chunk> ShiftedIeee8021qTagHeaderSerializer::deserialize(MemoryInputStream& stream) const
{
    const auto& header = makeShared<ShiftedIeee8021qTagHeader>();
    uint16_t value = stream.readUint16Be();
    header->setVid(value & 0xFFF);
    header->setPcp((value >> 13) & 7);
    header->setDei((value & 0x1000) != 0);
    header->setTypeOrLength(stream.readUint16Be());
    return header;
}

void Ieee8021qTagHeaderTpidFieldSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& header = staticPtrCast<const Ieee8021qTagHeader>(chunk);
    stream.writeUint16Be(header->getTpid());
}

const Ptr<Chunk> Ieee8021qTagHeaderTpidFieldSerializer::deserialize(MemoryInputStream& stream) const
{
    const auto& header = makeShared<Ieee8021qTagHeader>();
    header->setTpid(stream.readUint16Be());
    return header;
}

void Ieee8021qTagHeaderTciFieldSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& header = staticPtrCast<const Ieee8021qTagHeaderTciField>(chunk);
    stream.writeUint16Be((header->getVid() & 0xFFF) |
                         ((header->getPcp() & 7) << 13) |
                         (header->getDei() ? 0x1000 : 0));
}

const Ptr<Chunk> Ieee8021qTagHeaderTciFieldSerializer::deserialize(MemoryInputStream& stream) const
{
    const auto& header = makeShared<Ieee8021qTagHeaderTciField>();
    uint16_t value = stream.readUint16Be();
    header->setVid(value & 0xFFF);
    header->setPcp((value >> 13) & 7);
    header->setDei((value & 0x1000) != 0);
    return header;
}

} // namespace inet


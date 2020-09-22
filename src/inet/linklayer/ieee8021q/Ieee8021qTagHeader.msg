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

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;

//
// This chunk represents an IEEE 802.1Q tag header as defined by the section
// 9.3 Tag format of the IEEE Std 802.1Q-2018, IEEE Standard for Local and
// Metropolitan Area Networks, Bridges and Bridged Networks.
//
//  0                   1                   2                   3              
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
// +-------------------------------+-----+-+-----------------------+
// |       TPID (e.g. 0x8100)      | PCP |D|        VLAN ID        |
// +-------------------------------+-----+-+-----------------------+
//
class Ieee8021qTagHeader extends FieldsChunk
{
    chunkLength = B(4);
    uint16_t tpid; // Tag Protocol Identifier (TPID): 16-bit field specifying an EtherType value that is used to identify the frame as a tagged frame and to select the correct tag decoding functions. 
    uint8_t pcp;   // Priority Code Point (PCP): a 3-bit field which refers to the IEEE 802.1p priority. It indicates the frame priority level.
                   // Values are from 0 (best effort) to 7 (highest); 1 represents the lowest priority.
                   // Priority order lowest to highest: 1 0 2 3 4 5 6 7
    bool dei;      // Drop Eligible Indicator (DEI): a 1-bit field. May be used separately or in conjunction with PCP to indicate frames eligible to be dropped in the presence of congestion.
    uint16_t vid;  // VLAN Identifier (VID): a 12-bit field specifying the VLAN to which the frame belongs.
                   // The hexadecimal values of 0x000 and 0xFFF are reserved. All other values may be used as VLAN identifiers, allowing up to 4,094 VLANs.
                   // The reserved value 0x000 indicates that the frame does not belong to any VLAN; in this case,
                   // the 802.1Q tag specifies only a priority and is referred to as a priority tag. On bridges,
                   // VLAN 1 (the default VLAN ID) is often reserved for a management VLAN; this is vendor-specific.
}

//
// This chunk represents an IEEE 802.1Q tag data and the following 2 bytes in
// the packet. The reason for this shifted representation is that it's often
// easier to look at a Q tag this way during processing. The typeOrLength field
// contains the EtherType or data length of the following protocol (e.g. Ethernet).
//
//  0                   1                   2                   3              
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
// +-----+-+-----------------------+-------------------------------+
// | PCP |D|        VLAN ID        |        EtherType/Length       |
// +-----+-+-----------------------+-------------------------------+
//
class ShiftedIeee8021qTagHeader extends FieldsChunk
{
    chunkLength = B(4);
    uint8_t pcp;           // Priority Code Point (PCP): a 3-bit field which refers to the IEEE 802.1p priority. It indicates the frame priority level.
                           // Values are from 0 (best effort) to 7 (highest); 1 represents the lowest priority.
                           // Priority order lowest to highest: 1 0 2 3 4 5 6 7
    bool dei;              // Drop Eligible Indicator (DEI): a 1-bit field. May be used separately or in conjunction with PCP to indicate frames eligible to be dropped in the presence of congestion.
    uint16_t vid;          // VLAN Identifier (VID): a 12-bit field specifying the VLAN to which the frame belongs.
                           // The hexadecimal values of 0x000 and 0xFFF are reserved. All other values may be used as VLAN identifiers, allowing up to 4,094 VLANs.
                           // The reserved value 0x000 indicates that the frame does not belong to any VLAN; in this case,
                           // the 802.1Q tag specifies only a priority and is referred to as a priority tag. On bridges,
                           // VLAN 1 (the default VLAN ID) is often reserved for a management VLAN; this is vendor-specific.
    uint16_t typeOrLength; // This two-octet field takes one of two meanings, depending on its numeric value. For numerical evaluation,
                           // the first octet is the most significant octet of this field.
                           // a) If the value of this field is less than or equal to 1500 decimal (05DC hexadecimal), then the Length/
                           //    Type field indicates the number of MAC client data octets contained in the subsequent MAC Client
                           //    Data field of the basic frame (Length interpretation).
                           // b) If the value of this field is greater than or equal to 1536 decimal (0600 hexadecimal), then the
                           //    Length/Type field indicates the Ethertype of the MAC client protocol (Type interpretation). 28 
                           //    The Length and Type interpretations of this field are mutually exclusive.
                           //    When used as a Type field, it is the responsibility of the MAC client to ensure that the MAC client
                           //    operates properly when the MAC sublayer pads the supplied MAC Client data, as discussed in 3.2.7.    
}

//
// This chunk represents a portion of the IEEE 802.1Q tag header as defined by
// the section 9.5 Tag Protocol identification of the IEEE Std 802.1Q-2018, IEEE
// Standard for Local and Metropolitan Area Networks, Bridges and Bridged Networks.
//
//  0                   1              
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
// +-------------------------------+
// |       TPID (e.g. 0x8100)      |
// +-------------------------------+
//
class Ieee8021qTagHeaderTpidField extends FieldsChunk
{
    chunkLength = B(2);
    uint16_t tpid; // Tag Protocol Identifier (TPID): 16-bit field specifying an EtherType value that is used to identify the frame as a tagged frame and to select the correct tag decoding functions. 
}

//
// This chunk represents a portion of the IEEE 802.1Q tag as defined by the
// section 9.6 VLAN Tag Control Information (TCI) IEEE Std 802.1Q-2018, IEEE
// Standard for Local and Metropolitan Area Networks, Bridges and Bridged
// Networks.
//
//  0                   1              
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
// +-----+-+-----------------------+
// | PCP |D|        VLAN ID        |
// +-----+-+-----------------------+
//
class Ieee8021qTagHeaderTciField extends FieldsChunk
{
    chunkLength = B(2);
    uint8_t pcp;   // Priority Code Point (PCP): a 3-bit field which refers to the IEEE 802.1p priority. It indicates the frame priority level.
                   // Values are from 0 (best effort) to 7 (highest); 1 represents the lowest priority.
                   // Priority order lowest to highest: 1 0 2 3 4 5 6 7
    bool dei;      // Drop Eligible Indicator (DEI): a 1-bit field. May be used separately or in conjunction with PCP to indicate frames eligible to be dropped in the presence of congestion.
    uint16_t vid;  // VLAN Identifier (VID): a 12-bit field specifying the VLAN to which the frame belongs.
                   // The hexadecimal values of 0x000 and 0xFFF are reserved. All other values may be used as VLAN identifiers, allowing up to 4,094 VLANs.
                   // The reserved value 0x000 indicates that the frame does not belong to any VLAN; in this case,
                   // the 802.1Q tag specifies only a priority and is referred to as a priority tag. On bridges,
                   // VLAN 1 (the default VLAN ID) is often reserved for a management VLAN; this is vendor-specific.
}

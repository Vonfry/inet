//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
// Author: Zoltan Bojthe
//

#include <stdio.h>
#include <string.h>

#include "inet/linklayer/loopback/Loopback.h"
#include "inet/linklayer/common/InterfaceTag_m.h"

#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/common/queue/IPassiveQueue.h"
#include "inet/common/Simsignals.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"

namespace inet {

Define_Module(Loopback);

Loopback::~Loopback()
{
}

void Loopback::initialize(int stage)
{
    MacBase::initialize(stage);

    // all initialization is done in the first stage
    if (stage == INITSTAGE_LOCAL) {
        numSent = numRcvdOK = 0;
        WATCH(numSent);
        WATCH(numRcvdOK);
    }
    else if (stage == INITSTAGE_NETWORK_INTERFACE_CONFIGURATION) {
        registerInterface();
    }
}

void Loopback::configureInterfaceEntry()
{
//    // generate a link-layer address to be used as interface token for IPv6
//    InterfaceToken token(0, getSimulation()->getUniqueNumber(), 64);
//    interfaceEntry->setInterfaceToken(token);

    // set protocol
    //interfaceEntry->setProtocol(&Protocol::loopback);     //TODO

    // capabilities
    interfaceEntry->setMtu(par("mtu"));
    interfaceEntry->setLoopback(true);
}

void Loopback::handleMessage(cMessage *msg)
{
    if (!isOperational) {
        handleMessageWhenDown(msg);
        return;
    }

    auto packet = check_and_cast<Packet *>(msg);
    emit(packetReceivedFromUpperSignal, packet);
    EV << "Received " << packet << " for transmission\n";
    ASSERT(packet->hasBitError() == false);

    // pass up payload
    numRcvdOK++;
    emit(packetSentToUpperSignal, packet);
    numSent++;
    auto protocol = packet->getTag<PacketProtocolTag>()->getProtocol();
    packet->clearTags();
    packet->addTag<DispatchProtocolReq>()->setProtocol(protocol);
    packet->addTag<PacketProtocolTag>()->setProtocol(protocol);
    packet->addTag<InterfaceInd>()->setInterfaceId(interfaceEntry->getInterfaceId());
    send(packet, "upperLayerOut");
}

void Loopback::flushQueue()
{
    // do nothing, lo interface doesn't have any queue
}

void Loopback::clearQueue()
{
    // do nothing, lo interface doesn't have any queue
}

bool Loopback::isUpperMsg(cMessage *msg)
{
    return true;
}

void Loopback::refreshDisplay() const
{
    /* TBD find solution for displaying IPv4 address without dependence on IPv4 or IPv6
            Ipv4Address addr = interfaceEntry->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
            sprintf(buf, "%s / %s\nrcv:%ld snt:%ld", addr.isUnspecified()?"-":addr.str().c_str(), datarateText, numRcvdOK, numSent);
     */
    char buf[80];
    sprintf(buf, "rcv:%ld snt:%ld", numRcvdOK, numSent);

    getDisplayString().setTagArg("t", 0, buf);
}

} // namespace inet

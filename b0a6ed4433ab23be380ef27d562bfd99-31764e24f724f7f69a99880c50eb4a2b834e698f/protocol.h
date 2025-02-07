#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <iostream>
#include <cstring>

const int ProtocolId = 0x11223344;
const int MaxPacketSize = 1024;

struct Packet {
    int packetId;
    int dataSize;
    char data[MaxPacketSize];
};

class Protocol {
public:
    static void createPacket(Packet& packet, const char* data, int size) {
        packet.packetId = ProtocolId;
        packet.dataSize = size;
        std::memcpy(packet.data, data, size);
    }
};

#endif // PROTOCOL_H

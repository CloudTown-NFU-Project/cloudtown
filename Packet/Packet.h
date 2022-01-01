#ifndef Packet_H
#define Packet_H
#include <string.h>
using namespace std;
#include <iostream>
class BasePacket {
public:
    void* transferData;
    int CompareID = 0;
};


template <class PacketType>
class Packet : public BasePacket {
public:
    static int ID;
    PacketType* data, * org;
    Packet(PacketType c_data) {
        CompareID = ID;
        org = new PacketType(c_data);
        data = new PacketType;
        transferData = (void*)org;
        memcpy(((PacketType*)transferData), org, sizeof(org));
    }
    Packet(BasePacket& packet) {
        org = new PacketType;
        transferData = (void*)org;
        data = new PacketType;
        memcpy(data, ((PacketType*)packet.transferData), sizeof((*((PacketType*)packet.transferData))));
    }
    ~Packet() {
        delete transferData;
        delete data;
    }
    static bool instanceof(BasePacket& packet) {
        return ID == packet.CompareID;
    }
};
int uniqueID = 0;

template<class PacketType>
int Packet<PacketType>::ID = ++uniqueID;
#endif
#include "Packet.h"
#include <sstream>
#include <iomanip> 
#include <zlib.h>
#include <iostream>

Packet::Packet() : seq_(0), csum_(0), payload("") {}

Packet::Packet(const std::string& message) : seq_(0), csum_(0), payload(message) {}

void Packet::set_sequence(uint8_t s) 
{
    seq_ = s;
}

void Packet::set_csum(uint32_t csum) {
    csum_ = csum;
}

void Packet::set_payload(const std::string& message) 
{
    payload = message;
}

std::string Packet::serialize() const 
{
    std::string data;
    data.push_back(seq_);
    data.append(reinterpret_cast<const char*>(&csum_), sizeof(csum_));
    data.append(payload);
    std::cout << "Serialized data: " << data << std::endl;
    return data;
}


void Packet::deserialize(const std::string& data) 
{
    seq_ = data[0];
    csum_ = *reinterpret_cast<const uint32_t*>(data.data() + 1);
    payload.assign(data.begin() + 1 + sizeof(uint32_t), data.end());
}

bool Packet::is_corrupt() const 
{
    uint32_t computed_csum = crc32(0, reinterpret_cast<const Bytef*>(payload.c_str()), payload.size());
    return computed_csum != csum_;
}

uint32_t Packet::compute_checksum() const 
{
    return crc32(0, reinterpret_cast<const Bytef*>(payload.c_str()), payload.size());
}

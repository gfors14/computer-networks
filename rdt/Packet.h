#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <string>
#include <cstring> 

class Packet
{
private:
    uint8_t seq_;
    uint32_t csum_;
    std::string payload;

public:
    Packet();
    explicit Packet(const std::string& message);
    void set_sequence(uint8_t s);
    void set_csum(uint32_t csum);
    void set_payload(const std::string& message);
    uint8_t get_sequence() const { return seq_; }
    uint32_t get_csum() const { return csum_; }
    std::string get_payload() const { return payload; }
	 uint32_t compute_checksum() const;

    std::string serialize() const;
    void deserialize(const std::string& data);
	bool is_corrupt() const;
};

#endif

#ifndef RDT_H
#define RDT_H

#include <cstdint>
#include <iostream>
#include <string>
#include <zlib.h>

#include "Packet.h"
#include "Socket.h"

class rdt {

private:
  std::string host_;
  int port_;
  uint8_t seq_;
  Socket *s;
 

public:
  rdt();
  rdt(std::string host, int port);
  ~rdt();
  void rdt_send(std::string message);
  std::string rdt_rcv();
  void udt_send(std::string packet);
  bool is_ack(Packet *p);
  bool is_corrupt(Packet *p);
  bool has_sequence(int sequence);
  void increment_sequence_number();
  std::string make_packet(std::string message);
  void extract_data(Packet *p, std::string message);
  uint32_t compute_checksum(std::string payload);
};

#endif
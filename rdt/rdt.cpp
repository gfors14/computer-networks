#include "rdt.h"

// Default Constructor
rdt::rdt() 
{
    host_ = "thomas.butler.edu";
    port_ = 43539;
    seq_ = 0;
    s = new Socket(host_, port_, true);
    std::cout << "RDT client initialized with default settings: " << host_ << ":" << port_ << std::endl;
}

rdt::rdt(std::string host, int port) 
{
    host_ = host;
    port_ = port;
    seq_ = 0;
    s = new Socket(host_, port_, true);
    std::cout << "RDT client initialized: " << host_ << ":" << port_ << std::endl;
}


// Destructor
rdt::~rdt() 
{
    delete s;
}

// Sending data using RDT protocol
void rdt::rdt_send(std::string message) 
{
    std::string packet = make_packet(message);
    std::cout << "Prepared packet for sending: " << packet << std::endl;
    udt_send(packet);
}

std::string rdt::rdt_rcv() 
{
    sockaddr_in from_addr;
    int source_port;
    std::string received_data = s->recvFrom(from_addr, source_port);
    std::cout << "Attempted to receive data. Bytes received: " << received_data.size() << std::endl;

    if (!received_data.empty()) 
	{
        Packet p(received_data);
        std::cout << "Received packet. Checking for corruption and sequence." << std::endl;
        if (!is_corrupt(&p) && p.get_sequence() == seq_) 
		{
            std::cout << "Packet is good. Incrementing sequence." << std::endl;
            increment_sequence_number();
            return p.get_payload();
        } else 
		{
            std::cout << "Packet failed validation." << std::endl;
        }
    }

    return "";
}


// Low-level data transmission
void rdt::udt_send(std::string packet) 
{
    std::cout << "Sending packet over UDP: " << packet << std::endl;
    s->sendTo(packet, host_, port_);
}

// Check if the packet is an ACK
bool rdt::is_ack(Packet *p) 
{
    return p->get_payload() == "ACK";
}

bool rdt::is_corrupt(Packet *p) 
{
    bool result = p->compute_checksum() != p->get_csum();
    std::cout << "Packet checksum comparison: Computed=" << p->compute_checksum() << " Expected=" << p->get_csum() << " Result=" << (result ? "Corrupt" : "Good") << std::endl;
    return result;
}

// Check if the packet has the expected sequence number
bool rdt::has_sequence(int sequence) 
{
    return seq_ == sequence;
}

// Increment the sequence number (toggle between 0 and 1 for RDT 3.0)
void rdt::increment_sequence_number() 
{
    seq_ = 1 - seq_;  // Toggle between 0 and 1
}

std::string rdt::make_packet(std::string message) 
{
    Packet p(message);
    p.set_sequence(seq_);
    p.set_csum(compute_checksum(message));
    std::string serialized = p.serialize();

    std::cout << "Serialized data (hex): ";
    for (unsigned char c : serialized) 
	{
        printf("%02X ", c);
    }
    std::cout << std::endl;

    return serialized;
}

// Extract data from a packet (dummy implementation)
void rdt::extract_data(Packet *p, std::string message) 
{
  
}

// Compute checksum for payload
uint32_t rdt::compute_checksum(std::string payload) 
{
    uint32_t crc = crc32(0, (const Bytef *)payload.c_str(), payload.length());
    std::cout << "Computed checksum for payload: " << crc << std::endl;
    return crc;
}
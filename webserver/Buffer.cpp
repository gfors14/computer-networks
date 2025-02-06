#include "Buffer.h"

void Buffer::append(const std::byte* data, size_t length) 
{
    buffer_.insert(buffer_.end(), data, data + length);
}

std::string Buffer::toString() const 
{
    
    return std::string(reinterpret_cast<const char*>(buffer_.data()), buffer_.size());
}

void Buffer::clear() {
    buffer_.clear();
}

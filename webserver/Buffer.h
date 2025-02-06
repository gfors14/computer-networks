#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>

class Buffer 
{
	public:
		Buffer() = default;
		void append(const std::byte* data, size_t length);
		std::string toString() const;
		void clear();
	private:
		std::vector<std::byte> buffer_;
};

#endif 

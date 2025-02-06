#include "checksum.h"
#include <zlib.h> // CRC32 Implementation
#pragma warning (disable:4996) 
uint32_t calculateCRC32(const std::vector<uint8_t>& data) {
    return crc32(0, data.data(), data.size());
}

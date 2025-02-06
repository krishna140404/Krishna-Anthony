#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <cstdint>
#include <vector>

uint32_t calculateCRC32(const std::vector<uint8_t>& data);

#endif // CHECKSUM_H
#pragma once

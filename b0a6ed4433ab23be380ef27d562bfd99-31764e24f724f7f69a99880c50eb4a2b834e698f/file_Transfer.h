#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "protocol.h"

class FileTransfer {
public:
    static bool readFile(const std::string& filename, std::vector<uint8_t>& buffer);
    static bool writeFile(const std::string& filename, const std::vector<uint8_t>& buffer);
};

#endif // FILE_TRANSFER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "file_Transfer.h"

bool FileTransfer::readFile(const char* filename, char*& buffer, size_t& fileSize) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new char[fileSize];
    file.read(buffer, fileSize);
    file.close();
    return true;
}

unsigned int FileTransfer::calculateCRC(const char* data, size_t length) {
    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<unsigned int>(data[i]);
        for (int j = 0; j < 8; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
}

void FileTransfer::sendFile(const char* filename) {
    char* fileBuffer;
    size_t fileSize;
    if (!readFile(filename, fileBuffer, fileSize)) {
        return;
    }

    unsigned int crc = calculateCRC(fileBuffer, fileSize);
    std::cout << "Sending file: " << filename << " (" << fileSize << " bytes)" << std::endl;
    std::cout << "CRC Checksum: " << crc << std::endl;

    delete[] fileBuffer;
}

#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include <iostream>
#include <fstream>
#include <cstring>

// defining the structure 
class FileTransfer {
public:
    static bool readFile(const char* filename, char*& buffer, size_t& fileSize);
    static unsigned int calculateCRC(const char* data, size_t length);
    static void sendFile(const char* filename);
};

#endif // FILE_TRANSFER_H


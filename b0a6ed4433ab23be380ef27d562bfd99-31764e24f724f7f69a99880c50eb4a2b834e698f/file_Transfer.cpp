#include "file_transfer.h"

bool FileTransfer::readFile(const char* filename, std::vector<uint8_t>& buffer) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    if (fileSize <= 0) {
        std::cerr << "Error: Invalid file size for " << filename << std::endl;
        return false;
    }
    file.seekg(0, std::ios::beg);

    buffer.resize(static_cast<size_t>(fileSize));
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    return file.good();
}

bool FileTransfer::writeFile(const char* filename, const std::vector<uint8_t>& buffer) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to create file " << filename << std::endl;
        return false;
    }
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    return file.good();
}


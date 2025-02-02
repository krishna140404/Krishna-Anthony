#include "performance.h"

void Performance::startTimer() {
    start = std::chrono::high_resolution_clock::now();
}

void Performance::stopTimer() {
    end = std::chrono::high_resolution_clock::now();
}

double Performance::getElapsedTime() {
    return std::chrono::duration<double>(end - start).count();
}

double Performance::calculateSpeed(size_t fileSize) {
    double timeInSeconds = getElapsedTime();
    double fileSizeInBits = fileSize * 8;
    return (fileSizeInBits / (timeInSeconds * 1e6)); // Mbps
}

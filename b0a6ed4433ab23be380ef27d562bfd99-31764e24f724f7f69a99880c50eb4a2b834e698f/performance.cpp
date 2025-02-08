#include <iostream>
#include <ctime>
#include "performance.h"

double Performance::getCurrentTime() {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        std::cerr << "Error: Failed to get current time." << std::endl;
        return -1.0; // Return error value
    }
    return ts.tv_sec + (ts.tv_nsec / 1e9); // Convert to seconds with nanosecond precision
}

void Performance::measureTransferSpeed(size_t fileSize, double startTime, double endTime) {
    if (startTime < 0 || endTime < 0) {
        std::cerr << "Error: Invalid time measurement." << std::endl;
        return;
    }

    double duration = endTime - startTime;
    if (duration <= 0) {
        std::cerr << "Error: Invalid time duration." << std::endl;
        return;
    }

    double speedMbps = (fileSize * 8.0) / (duration * 1e6); // Convert bytes to megabits per second
    std::cout << "Transfer Speed: " << speedMbps << " Mbps" << std::endl;
}

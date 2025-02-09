#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <cstddef> // For size_t
// defining every function in header file 
class Performance {
public:
    static double getCurrentTime(); // Get current time in seconds
    static void measureTransferSpeed(size_t fileSize, double startTime, double endTime);
};

#endif // PERFORMANCE_H

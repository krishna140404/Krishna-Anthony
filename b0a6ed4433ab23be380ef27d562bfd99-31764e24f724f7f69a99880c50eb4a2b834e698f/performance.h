#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <iostream>
#include <ctime>

class Performance {
public:
    static double getCurrentTime();
    static void measureTransferSpeed(size_t fileSize, double startTime, double endTime);
};

#endif // PERFORMANCE_H


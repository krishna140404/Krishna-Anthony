#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <chrono>

class Performance {
public:
    void startTimer();
    void stopTimer();
    double getElapsedTime(); // In seconds
    double calculateSpeed(size_t fileSize); // Speed in Mbps

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
};

#endif // PERFORMANCE_H

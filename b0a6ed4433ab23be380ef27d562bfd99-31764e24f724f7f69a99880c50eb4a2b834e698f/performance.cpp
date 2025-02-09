#include <iostream>
#include <ctime>
#include "performance.h"

/*FUNCTION NAME: getCurrentTime
*
* parameter : none 
*
* return : returns time with milisecond 
*/
double Performance::getCurrentTime() {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        std::cerr << "Error: Failed to get current time." << std::endl;
        return -1.0; // return error value
    }
    return ts.tv_sec + (ts.tv_nsec / 1e9); // convert to seconds with nanosecond precision
}

/*FUNCTION NAME: measureTransferSpeed
*
* parameter : fileSize : it retrieves the file name from the file
*           : fileSize : it takes the buffer time
*           : endTime  :it notes the end time of the session 
* return : if it is true or not
*/
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

    double speedMbps = (fileSize * 8.0) / (duration * 1e6); // convert bytes to megabits per second
    std::cout << "Transfer Speed: " << speedMbps << " Mbps" << std::endl;
}

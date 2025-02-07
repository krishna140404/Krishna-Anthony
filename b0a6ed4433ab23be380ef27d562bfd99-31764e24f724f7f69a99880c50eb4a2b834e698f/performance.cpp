#include <iostream>
#include <ctime>

class Performance {
public:
    static double getCurrentTime() {
        return static_cast<double>(std::time(nullptr));
    }

    static void measureTransferSpeed(size_t fileSize, double startTime, double endTime) {
        double duration = endTime - startTime;
        if (duration <= 0) {
            std::cerr << "Error: Invalid time duration." << std::endl;
            return;
        }
        double speedMbps = (fileSize * 8.0) / (duration * 1e6);
        std::cout << "Transfer Speed: " << speedMbps << " Mbps" << std::endl;
    }
};

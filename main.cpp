/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <cstdio>
#include <memory>

using namespace std;

struct MemoryInfo {
    long totalMemory = 0;
    long freeMemory = 0;
    long availableMemory = 0;
};

MemoryInfo getMemoryInfo() {
    std::ifstream meminfoFile("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;
    MemoryInfo memInfo;

    if (meminfoFile.is_open()) {
        while (meminfoFile >> key >> value >> unit) {
            if (key == "MemTotal:") {
                memInfo.totalMemory = value;
            } else if (key == "MemFree:") {
                memInfo.freeMemory = value;
            } else if (key == "MemAvailable:") {
                memInfo.availableMemory = value;
            }
        }
        meminfoFile.close();
    } else {
        throw std::runtime_error("Could not open /proc/meminfo");
    }

    return memInfo;
}

double calculateMemoryUsagePercentage(const MemoryInfo& memInfo) {
    long usedMemory = memInfo.totalMemory - memInfo.availableMemory;
    return (double)usedMemory / memInfo.totalMemory * 100;
}

string checkOperatingSystem (){
    
    string res;
    
    #ifdef _WIN32
        cout << "You are using Windows (32-bit or 64-bit)" << endl;
        res = "Windows";
    #elif defined(__linux__)
        cout << "You are using Linux" << endl;
        res = "Linux";
    #elif defined(__APPLE__) || defined(__MACH__)
        cout << "You are using macOS" << std::endl;
    #elif defined(__unix__)
        cout << "You are using Unix" << endl;
    #else
        cout << "Unknown operating system" << endl;
    #endif
    
    return res;
} 

// Function to get the CPU times from /proc/stat
void getCpuTimes(long &user, long &nice, long &system, long &idle) {
    ifstream procStat("/proc/stat");
    string line;
    
    // Reading the first line which contains CPU times
    getline(procStat, line);

    stringstream ss(line);
    string cpu;
    ss >> cpu >> user >> nice >> system >> idle;

    procStat.close();
}

// Function to calculate CPU usage percentage
double getCpuUsage() {
    long user1, nice1, system1, idle1;
    long user2, nice2, system2, idle2;

    // Get initial CPU times
    getCpuTimes(user1, nice1, system1, idle1);
    sleep(1);  // Sleep for 1 second to get a time difference

    // Get CPU times after 1 second
    getCpuTimes(user2, nice2, system2, idle2);

    // Calculate the difference in times
    long total1 = user1 + nice1 + system1 + idle1;
    long total2 = user2 + nice2 + system2 + idle2;

    long totalDiff = total2 - total1;
    long idleDiff = idle2 - idle1;

    // Calculate CPU usage percentage
    double cpuUsage = (double)(totalDiff - idleDiff) / totalDiff * 100.0;

    return cpuUsage;
}

float getCPUTemperature() {
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
    float temperature;
    if (tempFile.is_open()) {
        tempFile >> temperature;
        temperature /= 1000; // Convert from millidegrees to degrees
        tempFile.close();
    } else {
        std::cerr << "Unable to read temperature file." << std::endl;
        return -1;
    }
    return temperature;
}



int main()
{
    cout<<"Hello World" << "\n";
    
    string operatingSystem = checkOperatingSystem();
    
    std::cout << operatingSystem << "\n"; 
    
    
    try {
        MemoryInfo memInfo = getMemoryInfo();
        double memoryUsagePercentage = calculateMemoryUsagePercentage(memInfo);

        std::cout << "Total Memory: " << memInfo.totalMemory / 1024 << " MB" << std::endl;
        std::cout << "Free Memory: " << memInfo.freeMemory / 1024 << " MB" << std::endl;
        std::cout << "Available Memory: " << memInfo.availableMemory / 1024 << " MB" << std::endl;
        std::cout << "Memory Usage: " << memoryUsagePercentage << " %" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    float cpuTemp = getCPUTemperature();
    if (cpuTemp != -1) {
        std::cout << "CPU Temperature: " << cpuTemp << "°C" << std::endl;
    }
    
    

    return 0;
}
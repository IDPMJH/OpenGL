// TimeManager.h
#pragma once
#include <chrono>

class TimeManager {
public:
    void Initialize();
    double GetElapsedTime();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> previousFrameTime;
};

// TimeManager.cpp
#include "TimeManager.h"

void TimeManager::Initialize() {
    previousFrameTime = std::chrono::high_resolution_clock::now();
}

double TimeManager::GetElapsedTime() {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;
    return elapsedTime.count();
}

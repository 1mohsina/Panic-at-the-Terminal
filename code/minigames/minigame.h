//
// Created by mohsi on 11/24/2026.
//

#ifndef PANIC_AT_THE_TERMINAL_MINIGAME_H
#define PANIC_AT_THE_TERMINAL_MINIGAME_H


#pragma once

#include <cctype>
#include <chrono>
#include <iostream>
#include <string>
using namespace std;

class minigame
{
    protected:
    int attempts;
    double elapsedTime;
    bool solved;

private:
    using clock = std::chrono::high_resolution_clock;
    Clock::time_point timeRunning;

    void startTimer()
    {

    }

};


#endif //PANIC_AT_THE_TERMINAL_MINIGAME_H

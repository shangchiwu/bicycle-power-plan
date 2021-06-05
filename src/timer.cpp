/**
 * \file    timer.cpp
 * 
 * \brief   Implementation of Timer utility
 * \details This timer provides timing, pause, resume, stop, reset functionalities.
 * 
 * \author  Shang Chi Wu
 * \date    Created: 2019/12/21 Modified: 2021/04/16
 */

#include "timer.h"

Timer::Timer() {
    reset();
}

void Timer::reset() {
    _state = TimerState::INIT;
    _duration = ClockType::duration::zero();
    _start_time = ClockType::now();
    _stop_time = _start_time;
}

unsigned long long Timer::getDuration() const {
    if (_state == TimerState::TIMING)
        return (_duration + (ClockType::now() - _start_time)).count();
    return _duration.count();
}

Timer::TimerState Timer::getState() {
    return _state;
}

void Timer::start() {
    _state = TimerState::TIMING;
    _start_time = ClockType::now();
}

void Timer::stop() {
    _stop_time = ClockType::now();
    _state = TimerState::STOP;
    _duration += _stop_time - _start_time;
}

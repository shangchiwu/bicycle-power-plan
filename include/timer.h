/**
 * \file    timer.h
 * 
 * \brief   Timer utility
 * \details This timer provides timing, pause, resume, stop, reset functionalities.
 * 
 * \author  Shang Chi Wu
 * \date    Created: 2019/12/21 Modified: 2021/04/16
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

/**
 * This class provides the timer functionality.
 */
class Timer {
public:
    /**
     * Clock type for timing.
     */
    using ClockType = std::chrono::high_resolution_clock;

    /**
     * Enum for timer state.
     * This enum indicates the timing state of the timer.
     * 
     */
    enum TimerState {
        INIT = 0,   ///< Ready to start.
        TIMING = 1, ///< Counting time.
        STOP = 2    ///< Stopped. Can get the duration via getDuration().
    };

    /**
     * Default constructor.
     * 
     */
    Timer();

    /**
     * Copy constructor.
     * 
     */
    Timer(const Timer &timer) = default;
    
    /**
     * Reset the timer.
     * 
     */
    void reset();
    
    /**
     * Get the duration of the timer.
     * 
     * \return The duration in nanoseconds.
     */
    unsigned long long getDuration() const;

    /**
     * Get the state of the timer.
     * 
     * \return The state of the timer.
     */
    TimerState getState();
    
    /**
     * Start the timer.
     * 
     */
    void start();
    
    /**
     * Stop the timer.
     * 
     */
    void stop();

private:
    TimerState _state;                  ///< The state of the timer.
    std::chrono::nanoseconds _duration; ///< The duration from last start to last stop.
    ClockType::time_point _start_time;  ///< The last start timepoint of the timer.
    ClockType::time_point _stop_time;   ///< The last stop timepoint of the timer.
};

#endif // TIMER_H_

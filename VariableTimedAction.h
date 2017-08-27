/*
   File:   VariableTimedAction.h
   Author: alexm

   This class is used as an interface to create a timed loop to perform repetitive
   actions. The implementing class must override the pure virtual function 'run',
   which will be called every interval. However, the user specified interval defines
   the minimum length of time which can be passed before the action is executed.

   Created on December 14, 2016, 4:43 PM
 */

#ifndef VARIABLETIMEDACTION_H
#define VARIABLETIMEDACTION_H

#include <Arduino.h>

class VariableTimedAction {
public:

    /**
       Starts the timer.
       @param l the interval in milliseconds
       @return true if the timer started, false if interval is invalid or the max number of actions are being executed
     */
    bool start(unsigned long l);

    /**
       This function is to be called repeatedly, however, not every call will
       result in an update. This function checks if the interval has passed before
       executing the user code.
     */
    void update();

    /**
       Pauses the timer.
     */
    void pause() {
        cycling = false;
    };

    /**
       Resumes the timer. Timer must have been initially started with a set
       interval using the start(long) function and then paused using the pause
       function. If this function returns false you must start it again using the
       start(long) function.
       @return true if the timer could be resumed, false if no interval is set
     */
    bool resume() {
        if (interval != 0) {
            cycling = true;
        }
        return cycling;
    };
    
    /**
     * Stops the user code from executing and removes the action from the actions
     * list, freeing up a spot for another action to be added.
     */
    void stop();

    /**
       Returns the status of the timer.
       @return true if timer is running, false if it has not been initialized or is paused
     */
    bool isRunning() {
        return cycling;
    };

    static const uint8_t MAX_ACTIONS = 5;
    static VariableTimedAction* actions[MAX_ACTIONS];

    static void updateActions();
private:
    bool cycling = false; //status variable
    unsigned long interval = 0; //interval in millis that the action should be executed
    unsigned long nextTick = 0; //time in millis that the action was executed last
    uint8_t myIndex = MAX_ACTIONS; //the index of this action within the actions array
    /**
       This function is to be overridden. It contains the code that will be
       executed every interval. The returned unsigned long is the new interval.
       If 0 is returned then the interval stays the same.
       @return the new interval, 0 if the interval is to stay the same
     */
    virtual unsigned long run() = 0;
};

#endif /* VARIABLETIMEDACTION_H */


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

    void start(unsigned long startInterval, bool startNow = true);
    void toggleRunning();
    bool isRunning() {
        return running;
    }
    void stop();
    static void updateActions();
    
private:
    unsigned long interval;
    unsigned long nextTick;
    bool running = false;
    int index = -1;

    static int maxActions;
    static VariableTimedAction ** actions;
    /**
       This function is to be overridden. It contains the code that will be
       executed every interval. The returned unsigned long is the new interval.
       If 0 is returned then the interval stays the same.
       @return the new interval, 0 if the interval is to stay the same
     */
    virtual unsigned long run() = 0;
    void update();
};

#endif /* VARIABLETIMEDACTION_H */


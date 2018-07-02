#ifndef VARIABLETIMEDACTION_H
#define VARIABLETIMEDACTION_H

#include <Arduino.h>

/**
   This class is used as an interface to create a timed loop to perform repetitive
   actions. The implementing class must override the pure virtual function #run,
   which will be called every interval. However, the user specified interval defines
   the minimum length of time which can be passed before the action is executed.

   Created on December 14, 2016, 4:43 PM
   
   @author amirchev
   @version 1.2.2
 */
class VariableTimedAction {
public:

    /**
       Starts the timer and sets the interval that the #run method should be called.
       This method can be called whether the timer is stopped or paused. If it is
       stopped or has never been started, it will be started. If it is paused, then it will
       resume using the new interval.
       
       @param startInterval The interval to wait between calls to #run.
       @param startNow If true, the timer will execute #run right away and then wait 
        until the next interval. Otherwise, the timer will execute after the interval
        has passed.
     */
    void start(unsigned long startInterval, bool startNow = true);
    /**
       Toggles the timer on and off. This is faster than stopping and starting the
       timer every time. 
     */
    void toggleRunning();
    /**
       Returns current status of the timer.
       
       @return True if the timer is running, false if it is paused or stopped.
     */
    bool isRunning() {
        return running;
    }
    /**
       Stops the timer, removing it from the list of actions. If the timer has to be
       started again, then use #start as it will need to be added into the list of actions.
     */
    void stop();
    /**
       Iterates through all actions and calls the #run method on actions
       whose interval has passed.
       
       This method should be called frequently in the #loop method. In timing-intensive
       applications, this method should be the only thing in the loop method.
      */
    static void updateActions();
    
private:
    unsigned long interval;
    unsigned long nextTick;
    bool running = false;
    int index = -1;

    static int maxActions;
    static VariableTimedAction ** actions;
    /**
       Each implementing class must override this function with custom behavior.
       This method is called after the defined interval has passed. For proper
       functioning, this method must return the new interval in milliseconds or
       0 if the interval is to remain unchanged.
       @return The new interval or 0 if the interval is to stay the same.
     */
    virtual unsigned long run() = 0;
    void update();
};

#endif /* VARIABLETIMEDACTION_H */

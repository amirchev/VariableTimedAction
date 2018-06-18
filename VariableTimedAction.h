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

    void start(unsigned long startInterval, bool startNow = true) {
        static bool init = false;
        if (!init) {
            maxActions = 15;
            actions = new VariableTimedAction*[maxActions](NULL); //start at 15 actions, increase as needed
            init = true;
        }

        int emptyIndex = -1;
        for (int i = 0; i < maxActions; i++) {
            if (actions[i] == NULL) {
                emptyIndex = i;
                break;
            }
        }
        
        if (emptyIndex == -1) { //unable to find spot
            emptyIndex = maxActions;
            int previousMax = maxActions;
            maxActions += 5;
            VariableTimedAction ** newArray = new VariableTimedAction*[maxActions](NULL);
            for (int i = 0; i < previousMax; i++) {
                newArray[i] = actions[i];
            }
            delete [] actions;
            actions = newArray;
        }
        
        actions[emptyIndex] = this;
        index = emptyIndex;
        interval = startInterval;
        if (startNow) {
            nextTick = millis();
        } else {
            nextTick = millis() + interval;
        }
        running = true;
    }

    void toggleRunning() {
        if (index != -1) {
            running = !running;
            if (running) {
                nextTick = millis();
            }
        }
    }

    bool isRunning() {
        return running;
    }

    void stop() {
        if (index != -1) {
            actions[index] = NULL;
            index = -1;
            running = false;
        }
    }

    void update() {
        if (millis() > nextTick && running) {
            unsigned long nextInterval = run();
            if (nextInterval != 0) {
                interval = nextInterval;
            }
            nextTick += interval;
        }
    }

    static void updateActions() {
        for (int i = 0; i < maxActions; i++) {
            if (actions[i] != NULL) {
                actions[i]->update();
            }
        }
    }
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
};

int VariableTimedAction::maxActions;
VariableTimedAction ** VariableTimedAction::actions;

#endif /* VARIABLETIMEDACTION_H */


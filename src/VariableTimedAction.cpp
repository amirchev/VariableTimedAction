/*
 * VariableTimedAction.cpp - A library for creating timed events/actions.
 * Created by Aleksandr N. Mirchev, Aug 27, 2017.
 * Released into the public domain.
 */

#include <VariableTimedAction.h>

int VariableTimedAction::maxActions;
VariableTimedAction ** VariableTimedAction::actions;

void VariableTimedAction::start(unsigned long startInterval, bool startNow) {
    static bool init = false;
    if (!init) {
        maxActions = 15;
        actions = new VariableTimedAction*[maxActions]; //start at 15 actions, increase as needed
        for (int i = 0; i < maxActions; i++) {
            actions[i] = NULL;
        }
        init = true;
    }
    
    if (index == -1) {
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
            VariableTimedAction ** newArray = new VariableTimedAction*[maxActions];
            for (int i = previousMax; i < maxActions; i++) {
                newArray[i] = NULL;
            }
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
    } else {
        interval = startInterval;
        if (!running) {
            toggleRunning();
        }
    }
}

void VariableTimedAction::toggleRunning() {
    if (index != -1) {
        running = !running;
        if (running) {
            nextTick = millis();
        }
    }
}

void VariableTimedAction::stop() {
    if (index != -1) {
        actions[index] = NULL;
        index = -1;
        running = false;
    }
}

void VariableTimedAction::update() {
    if (millis() > nextTick && running) {
        unsigned long nextInterval = run();
        if (nextInterval != 0) {
            interval = nextInterval;
        }
        nextTick += interval;
    }
}

void VariableTimedAction::updateActions() {
    for (int i = 0; i < maxActions; i++) {
        if (actions[i] != NULL) {
            actions[i]->update();
        }
    }
}

#include "VariableTimedAction.h"

static VariableTimedAction* VariableTimedAction::actions[MAX_ACTIONS] = {0};

bool VariableTimedAction::start(unsigned long l) {
    if (l > 0) {
        interval = l;
        nextTick = millis() + interval;

        //find available action array slots
        for (myIndex = 0; myIndex < MAX_ACTIONS; myIndex++) {
            if (actions[myIndex] == 0) {
                break;
            } else if (actions[myIndex] == this) {
                return cycling;
            }
        }
        if (myIndex != MAX_ACTIONS) { //if there is an available slot
            actions[myIndex] = this;
            cycling = true;
        } else {
            cycling = false;
        }
    } else {
        cycling = false;
    }

    return cycling;
}

void VariableTimedAction::stop() {
    interval = 0;
    nextTick = 0;
    cycling = false;
    actions[myIndex] = 0;
}

void VariableTimedAction::update() {
    if (!cycling) {
        return;
    }

    if (millis() >= nextTick) {
        unsigned long newInterval = run();
        if (newInterval != 0) {
            interval = newInterval;
        }
        nextTick += interval;
    }
}

static void VariableTimedAction::updateActions() {
    for (uint8_t i = 0; i < MAX_ACTIONS; i++) {
        if (actions[i] != 0) {
            actions[i]->update();
        }
    }
}
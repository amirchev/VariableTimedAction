# VariableTimedAction

1. [Introduction](#introduction)
2. [Examples](#examples)
3. [Documentation](#documentation)

## Introduction
This library, in technical terms, is an interface that allows you to define a [run](#virtual-unsigned-long-run) method, similar to Java's [Runnable](https://docs.oracle.com/javase/7/docs/api/java/lang/Runnable.html) interface, which will be executed at the specified interval.

In simpler terms, this library allows you to "multi-thread" or have multiple separately executing timed actions.

## Examples
First and foremost, your `loop` method must call [VariableTimedAction::updateActions](#static-void-updateactions) constantly. For more timing-intensive applications it is recommended that the call to [updateActions](#static-void-updateactions) is the only thing in the loop method. It will look like this:

```
void loop() {
  //this static method should really be the only thing called
  //in your loop method to make sure events are updated on time
  VariableTimedAction::updateActions();
}
```

Next, you can create a timer class like this:

```
class Counter : public VariableTimedAction {
private:
  int count = 0;

  //this method will be called at your specified interval
  unsigned long run() {
    //increase the timer
    count++;

    //return code of 0 indicates no change to the interval
    //if the interval must be changed, then return the new interval
    return 0;
  }

public:
  int getCount() {
    return count;
  }
};
```

To start this counter you can do this:

```
Counter secondCounter;

void setup() {
  //call the run method every 1000ms (or 1 sec)
  secondCounter.start(1000);
}
```

To pause/unpause, you can use the [toggleRunning](#void-togglerunning) method. To stop the timer, you can use the [stop](#void-stop) method. If you use the [stop](#void-stop) method, you must use the [start](#void-startunsigned-long-startinterval-bool-startnow--true) method to begin the counter again. To find out if the timer is running or it is currently paused, use the [isRunning](#bool-isrunning) method. You can have as many timers as you like; each of them can be controlled individually.

## Documentation

1. [void start(unsigned long, bool)](#void-startunsigned-long-startinterval-bool-startnow--true)
2. [void toggleRunning()](#void-togglerunning)
3. [bool isRunning()](#bool-isrunning)
4. [void stop()](#void-stop)
5. [static void updateActions()](#static-void-updateactions)
6. [virtual unsigned long run()](#virtual-unsigned-long-run)

### void start(unsigned long startInterval, bool startNow = true)
Starts the timer and sets the interval that the [run](#virtual-unsigned-long-run) method should be called. 
This method can be called whether the timer is stopped or paused. If it is stopped or has never been started, it will be started. If it is paused, then it will resume using the new interval.
#### @param startInterval
The interval to wait between calls to [run](#virtual-unsigned-long-run). 
#### @param startNow
If true, the timer will execute [run](#virtual-unsigned-long-run) right away and then wait until the next interval. Otherwise, the timer will execute after the interval has passed. 

### void toggleRunning()
Toggles the timer on and off. This is faster than stopping and starting the timer every time. 

### bool isRunning()
Returns current status of the timer. 
#### @return
True if the timer is running, false if it is paused or stopped. 

### void stop()
Stops the timer, removing it from the list of actions. 
If the timer has to be started again, then use start as it will need to be added into the list of actions. 

### static void updateActions()
Iterates through all actions and calls the [run](#virtual-unsigned-long-run) method on actions whose interval has passed. 
This method should be called frequently in the `loop` method. In timing-intensive applications, this method should be the only thing in the `loop` method. 

### virtual unsigned long run()
Each implementing class must override this function with custom behavior.
This method is called after the defined interval has passed. For proper functioning, this method must return the new interval in milliseconds or `0` if the interval is to remain unchanged.
#### @return
The new interval or `0` if the interval is to stay the same.

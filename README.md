# VariableTimedAction
This library, in technical terms, is an interface that allows you to define a `run` method, similar to Java's [Runnable](https://docs.oracle.com/javase/7/docs/api/java/lang/Runnable.html) interface, which will be executed at the specified interval.

In simpler terms, this library allows you to "multi-thread" or have multiple separately executing timed actions.

## How to Use
First and foremost, your `loop` method must call `VariableTimedAction::updateActions` constantly. For more timing-intensive applications it is recommended that the call to `updateActions` is the only thing in the loop method. It will look like this:

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

To pause/unpause, you can use the `toggleRunning` method. To stop the timer, you can use the `stop` method. If you use the `stop` method, you must use the `start` method to begin the counter again. To find out if the timer is running or it is currently paused, use the `isRunning` method. You can have as many timers as you like; each of them can be controlled individually.

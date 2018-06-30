/*
 This class uses the VariableTimedAction library to emulate a clock. 
*/

// VariableTimedAction - Version: Latest 
#include <VariableTimedAction.h>

class Clock : public VariableTimedAction {
private:
  int hours = 0, minutes = 0, seconds = 0;
  
  //updates the clock 
  unsigned long run() {
    seconds++;
    if (seconds == 60) {
      seconds = 0;
      minutes++;
      if (minutes == 60) {
        minutes = 0;
        hours++;
        if (hours == 24) {
          hours = 0;
        }
      }
    }
    return 0;
  }
  
public:
  int getHours() { return hours; }
  int getMinutes() { return minutes; }
  int getSeconds() { return seconds; }
  
  //starts the clock at 00:00:00
  Clock() { start(1000); }
  
  //starts the clock with the specified time
  Clock(int hours, int minutes, int seconds)
    : hours(hours), minutes(minutes), seconds(seconds) {
    start(1000, false);
  }
  
  //sets the clock to the specified time 
  setTime(int hours, int minutes, int seconds) {
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
  }
  
} clock;

class ClockDisplay : public VariableTimedAction {
public:
  ClockDisplay(Clock &clock) {
    this->clock = &clock;
  }
private:
  Clock * clock;
  unsigned long run() {
    Serial.print(clock->getHours());
    Serial.print(":");
    Serial.print(clock->getMinutes());
    Serial.print(":");
    Serial.println(clock->getSeconds());
    return 0;
  }
} display(clock);

void setup() {
  //initialize Serial
  Serial.begin(9600);
  
  //obtain current time
  //prompt user input
  Serial.println("What is the current time in 24-hour time? (HH:mm:ss)");
  char input[8];
  //obtain user input
  int len = Serial.readBytesUntil(':', input, 8);
  //null terminate the string
  input[len] = '\0';
  //convert string to integer
  int userHours = atoi(input);
  
  len = Serial.readBytesUntil(':', input, 8);
  //null terminate the string
  input[len] = '\0';
  //convert string to integer
  int userMinutes = atoi(input);
  
  len = Serial.readBytesUntil('\n', input, 8);
  //null terminate the string
  input[len] = '\0';
  //convert string to integer
  int userSeconds = atoi(input);
  
  //set the user's time
  clock.setTime(userHours, userMinutes, userSeconds);
  //update the time display every 15 seconds
  display.start(15 * 1000);
}

void loop() {
  //check if any actions must be executed
  VariableTimedAction::updateActions();
}

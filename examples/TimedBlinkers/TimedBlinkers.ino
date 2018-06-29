/*
 This example uses the VariableTimedAction class to time LED blinking. There are 3 LEDs in pins 1, 2, and 3.
*/
#include <VariableTimedAction.h>

class BlinkingLED : public VariableTimedAction {
private:
  //stores the pin of this LED
  int pin;
 
  unsigned long run() {
    static bool on = false;
    
    //if the pin is off, then turn it on
    if (!on) {
      digitalWrite(pin, HIGH);
      on = true;
      
    //if the pin is on, then turn it off
    } else {
      digitalWrite(pin, LOW);
      on = false;
    }
    
    //returns the amount in seconds to wait before executing the next event
    //if 0 is returned, then the previous interval is maintained
    return 0;
  }
public:
  BlinkingLED(int pin)
  //sets the pin to be used for the LED
  : pin(pin) {
    //sets the pin mode
    pinMode(pin, OUTPUT);
  }
};

//the collection of LED objects that control the LEDs
BlinkingLED LEDs[] = {
  BlinkingLED(1),
  BlinkingLED(2),
  BlinkingLED(3)
};

//LED manager class is responsible for monitoring user input and pausing/unpausing LEDs
class LEDManager : public VariableTimedAction {
  public:
  unsigned long run() {
    //if there is data available from the user
    if (Serial.available() > 0) {
      char input[8];
      //read the user's input
      int len = Serial.readBytesUntil('\n', input, 8);
      //null terminate the retrieved c-string
      input[len] = '\0';
      //convert the string to an integer and subtract one to access the array properly (LED 1 is at array index 0) 
      int index = atoi(input) - 1;
      //pause/unpause the timer
      LEDs[index].toggleRunning();
    }
    return 0;
  }
};

LEDManager manager;

void setup() {
  //start Serial at 9600
  Serial.begin(9600);
  char input[8];
  
  //prompt user's input
  Serial.println(F("How many times per minute should LED 1 blink?: "));
  //read the user's input
  int len = Serial.readBytesUntil('\n', input, 8);
  //null terminate the string input
  input[len] = '\0';
  //calculate how often the LED should be turned on and off to achieve the user's preferred blinks/min
  //note: the interval is divided by two because one timer event will turn the LED on, and the next one will turn the LED off
  unsigned long interval = ((60 * 1000) / atoi(input)) / 2;
  //start the timer
  LEDs[0].start(interval);
  
  Serial.println(F("How many times per minute should LED 2 blink?: "));
  len = Serial.readBytesUntil('\n', input, 8);
  input[len] = '\0';
  interval = ((60 * 1000) / atoi(input)) / 2;
  LEDs[1].start(interval);
  
  Serial.println(F("How many times per minute should LED 3 blink?: "));
  len = Serial.readBytesUntil('\n', input, 8);
  input[len] = '\0';
  interval = ((60 * 1000) / atoi(input)) / 2;
  LEDs[2].start(interval);
  
  Serial.println(F("Now that everything is set up, you can pause/unpause any LED by typing in 1, 2, or 3 into the console!"));
  //have the manager check for user input every 500ms
  manager.start(500);
}

void loop() {
  //this static method should really be the only thing called in your loop method to make sure events are updated on time
  VariableTimedAction::updateActions();
}

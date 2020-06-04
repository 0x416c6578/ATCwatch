
#include "inputoutput.h"
#include "Arduino.h"
#include "pinout.h"

volatile long vibration_end_time = 0;
volatile long led_end_time = 0;
volatile bool inputoutput_inited = false;
volatile int motor_power = 100;

void init_inputoutput() {

  pinMode(STATUS_LED, OUTPUT);
  pinMode(VIBRATOR_OUT, OUTPUT);
  pinMode(PUSH_BUTTON_IN, INPUT);
  if (PUSH_BUTTON_OUT != -1) {
    pinMode(PUSH_BUTTON_OUT, OUTPUT);
    digitalWrite(PUSH_BUTTON_OUT, HIGH);
  }

  set_led(0); //Boolean false
  set_motor(0); //Boolean false
  inputoutput_inited = true;
}

bool get_button() {
  if (!inputoutput_inited)init_inputoutput();
  if (PUSH_BUTTON_OUT != -1) {
    pinMode(PUSH_BUTTON_OUT, OUTPUT);
    digitalWrite(PUSH_BUTTON_OUT, HIGH);
  }
  bool button = digitalRead(PUSH_BUTTON_IN);
  return button;
}

void set_led(bool state) { //Green LED on the back of the device
  if (state)
    digitalWrite(STATUS_LED, HIGH);
  else
    digitalWrite(STATUS_LED, LOW);
}

void set_motor(bool state) { //Internal function to turn the motor on or off
  if (state)
    digitalWrite(VIBRATOR_OUT, LOW);
  else
    digitalWrite(VIBRATOR_OUT, HIGH);
}

void set_motor_ms(int ms) { //Set motor to go for a custom amount of millis
  set_motor(1);
  vibration_end_time = millis() + ms;
}

void set_motor_ms() { //Set motor to go for the time set by motor_power (default: 100)
  set_motor(1);
  vibration_end_time = millis() + motor_power;
}

void set_motor_power(int ms) { //Set motor power (time that the motor spins for on calling set_motor_ms() overloaded function)
  motor_power = ms;
}

int get_motor_power() { //Get the motor spin time
  return motor_power;
}

void set_led_ms(int ms) {
  set_led(1);
  led_end_time = millis() + ms;
}

void check_inputoutput_times() { //Run by the interrupt handler, will stop the motor after the correct amount of milliseconds have ellapsed
  if (millis() > vibration_end_time)set_motor(0);
  if (millis() > led_end_time)set_led(0);
}

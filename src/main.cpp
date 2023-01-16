#include <Arduino.h>
#include <BluetoothSerial.h>
BluetoothSerial BtSerial;

#define RED_TEMP_SENSOR_PIN 2
#define BLACK_TEMP_SENSOR_PIN 3
#define DOR_BUTTON_PIN 4

#define HEATER_PIN_1 5
#define HEATER_PIN_2 6
#define FAN_PIN 7

void SetHeater(bool state)
{
  digitalWrite(HEATER_PIN_1, state);
  digitalWrite(HEATER_PIN_2, state);
}

void SetFan(bool state){
  digitalWrite(FAN_PIN,state);
}

void setup()
{
  BtSerial.begin("Microwave v3");
  pinMode(RED_TEMP_SENSOR_PIN, PULLUP);
  pinMode(BLACK_TEMP_SENSOR_PIN, PULLUP);
  pinMode(DOR_BUTTON_PIN, PULLUP);

  pinMode(HEATER_PIN_1, OUTPUT);
  pinMode(HEATER_PIN_2, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void loop()
{

}

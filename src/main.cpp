#include <Arduino.h>
#include <BluetoothSerial.h>
BluetoothSerial BtSerial;
TaskHandle_t Task1;

#define RED_TEMP_SENSOR_PIN 14
#define BLACK_TEMP_SENSOR_PIN 27
#define DOR_BUTTON_PIN 12

#define HEATER_PIN_1 33
#define HEATER_PIN_2 25
#define FAN_PIN 26

int targetTemp = 20;

void SetHeater(bool state)
{
  digitalWrite(HEATER_PIN_1, !state);
  digitalWrite(HEATER_PIN_2, !state);
}

void SetFan(bool state)
{
  digitalWrite(FAN_PIN, !state);
}

int GetTemp(){
  int red = analogRead(RED_TEMP_SENSOR_PIN);
  int black = analogRead(RED_TEMP_SENSOR_PIN);
  int average = (red+black)/2;

  //convert from sensor readings to temperature
  float temp=(average-684.98)/22.07;

  return temp;
}

void SetTargetTemp(int temp){
  targetTemp=temp;
}

void BtSerialCode(void *parameter)
{
  for (;;)
  {
    if (BtSerial.available() > 0)
    {
      BtSerial.println(BtSerial.read());
    }
  }
}

void setup()
{
  xTaskCreatePinnedToCore(
      BtSerialCode, /* Function to implement the task */
      "Task1",      /* Name of the task */
      10000,        /* Stack size in words */
      NULL,         /* Task input parameter */
      0,            /* Priority of the task */
      &Task1,       /* Task handle. */
      0);           /* Core where the atask should run */
  BtSerial.begin("Microwave v4");
  Serial.begin(115200);
  pinMode(RED_TEMP_SENSOR_PIN, INPUT);
  pinMode(BLACK_TEMP_SENSOR_PIN, INPUT);
  pinMode(DOR_BUTTON_PIN, INPUT);

  pinMode(HEATER_PIN_1, OUTPUT);
  pinMode(HEATER_PIN_2, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  
  SetFan(false);
  SetHeater(false);
}

void loop()
{
  BtSerial.print("RED: ");
  BtSerial.print(analogRead(RED_TEMP_SENSOR_PIN));
  BtSerial.print("                              BLACK: ");
  BtSerial.println(analogRead(BLACK_TEMP_SENSOR_PIN));

  BtSerial.print("{");
  BtSerial.print("\"C0\":\"");//"C0":"
  BtSerial.print(GetTemp());//xxx.xxx
  BtSerial.print("\",");//",

    BtSerial.print("\"T0\":\"");//"T0":"
  BtSerial.print(targetTemp);//xxx.xxx
  BtSerial.print("\",");//",

    BtSerial.print("\"H0\":\"");//"H0":"
  BtSerial.print(GetTemp());//x
  BtSerial.print("\",");//",

    BtSerial.print("\"F0\":\"");//"F0":"
  BtSerial.print(GetTemp());//x
  BtSerial.print("\",");//",

    BtSerial.print("\"D0\":\"");//"D0":"
  BtSerial.print(GetTemp());//x
  BtSerial.print("\",");//",
  SetFan(true);
  delay(100);
}

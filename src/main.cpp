#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "dd-wrt";
const char *password = "modecom32";
// Domain name with URL path or IP address with path
String serverName = "http://192.168.10.12:1880/update-sensor";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

TaskHandle_t Task1;

#define RED_TEMP_SENSOR_PIN 33
#define BLACK_TEMP_SENSOR_PIN 32
#define DOR_BUTTON_PIN 35

#define HEATER_PIN_1 27
#define HEATER_PIN_2 26
#define FAN_PIN 25

int targetTemp = 20;

void SetHeater(bool state)
{
  digitalWrite(HEATER_PIN_1, !state);
  digitalWrite(HEATER_PIN_2, !state);
}

bool GetDor()
{
  return digitalRead(DOR_BUTTON_PIN);
}

bool GetFan()
{
  return digitalRead(FAN_PIN);
}

bool GetHeater()
{
  return digitalRead(HEATER_PIN_1) && digitalRead(HEATER_PIN_2);
}

void SetFan(bool state)
{
  digitalWrite(FAN_PIN, !state);
}

int GetTemp()
{
  int red = analogRead(RED_TEMP_SENSOR_PIN);
  int black = analogRead(RED_TEMP_SENSOR_PIN);
  int average = (red + black) / 2;

  // convert from sensor readings to temperature
  float temp = (average - 684.98) / 22.07;

  return temp;
}

void SetTargetTemp(int temp)
{
  targetTemp = temp;
}

void BtSerialCode(void *parameter)
{ 
  vTaskDelay(50000);
}

void setup()
{

  pinMode(RED_TEMP_SENSOR_PIN, INPUT);
  pinMode(BLACK_TEMP_SENSOR_PIN, INPUT);
  pinMode(DOR_BUTTON_PIN, INPUT);

  pinMode(HEATER_PIN_1, OUTPUT);
  pinMode(HEATER_PIN_2, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  SetFan(false);
  SetHeater(false);

  xTaskCreatePinnedToCore(
      BtSerialCode, /* Function to implement the task */
      "Task1",      /* Name of the task */
      10000,        /* Stack size in words */
      NULL,         /* Task input parameter */
      0,            /* Priority of the task */
      &Task1,       /* Task handle. */
      0);           /* Core where the atask should run */

  WiFi.begin(ssid, password);
  Serial.begin(115200);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      String serverPath = serverName + "?tragertTemp="+targetTemp+"&currentTemp="+GetTemp()+"&isDorOpened"+GetDor()+"&isHeaterOn"+GetHeater()+"&isFanOn"+GetFan();
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
    SetFan(true);
    delay(100);
  }
}
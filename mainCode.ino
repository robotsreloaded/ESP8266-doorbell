// If unable to connect to WIFI, the config portal opens and stays for 2 mins after which it goes into deepsleep

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266Webhook.h> // By Rupak Poddar
#include <ESP8266WiFi.h>
#define KEY "YOUR_WEBHOOKS_KEY_HERE"        // Webhooks Key
#define EVENT "YOU_WEBHOOKS_EVENT_NAME"      // Webhooks Event Name
WiFiManager wm;
Webhook webhook(KEY, EVENT);    // Create an object.
int timeout = 180; // seconds to run for
int startTime = 0;
bool ds = false;
void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  wm.setConfigPortalBlocking(false);
  Serial.begin(115200);
  startTime = millis();
  bool res;
  res = wm.autoConnect("EspDoorBell", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    int response = webhook.trigger();
    if (response == 200)
    {
      Serial.println("Webhook Trigger OK");
      ds = true;
      Serial.println("Entering Deep Sleep Mode");
      ESP.deepSleep(0);
    }
    else
    {
      Serial.println("Webhook Trigger Failed");
    }
  }

}

void loop() {
  wm.process();
  if (millis() - startTime > timeout*1000 && !ds)
  {
    ds = true;
    Serial.println("Entering Deep Sleep Mode");
    ESP.deepSleep(0);
  }

}

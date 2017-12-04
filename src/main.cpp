#include <Wire.h>
#include <SparkFun_MMA8452Q.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

MMA8452Q accel(0x1c);
short x, y, z;
WiFiServer server(4343);

void setup()
{
  Serial.begin(115200);
  Serial.println("MMA8452Q Test Code!");
  WiFi.mode(WIFI_STA);

  WiFi.begin("", "");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  Wire.pins(D6, D5);
  accel.init();
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      long d = 0;
      int i = 10;
      while (i--) {
        if (accel.available())
        {
          accel.read();

          d += abs(x - accel.x) + abs(y - accel.y) + abs(z - accel.z);
          x = accel.x;
          y = accel.y;
          z = accel.z;
        }
      }
      client.println(log2f(d));
      client.flush();
      delay(10);
    }
  }
}

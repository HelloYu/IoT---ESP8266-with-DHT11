/**The MIT License (MIT)

Copyright (c) 2016 by Jeffrey Yu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <ESP8266WiFi.h>

const char* SSID = "Pursuit of happiness";
const char* password = "yuyanping1991";

const char* host = "api.thingspeak.com";
const char* writeKey = "NUC9L8BOIZUG9DX5";
const int httpPort = 80;

bool initWIFI() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID,password);

  Serial.print("Connecting");

  // waitting for wifi connected.
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

bool uploadDataToThingSpeak(String temperatureC, String humidity) {
WiFiClient client;

  if (!client.connect(host, httpPort)) {
    return false;
  }

  // compose ThindSpeak update data url
  String url = "/update?key=";
  url = url + writeKey;
  url = url + "&field1=";
  url = url + String(temperatureC);
  url = url + "&field2=";
  url = url + String(humidity);
  url = url + "\r\n";
  
  // This will send the request to the ThingSpeak
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  return true;
}

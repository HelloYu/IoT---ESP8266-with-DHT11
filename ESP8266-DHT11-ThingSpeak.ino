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
#include <stdlib.h>
#include "DHT.h"

#define DHTPIN 2    // Connect to ESP8266 GPIO2 pin
#define DHTTYPE DHT11   // Define DHT type

#define DELAY_TIME 10*60*1000 //ten minutes interval between updating data

#define BAUD_RATE 115200

DHT dht(DHTPIN, DHTTYPE);

//set up module like main founction
void setup()
{
  Serial.begin(BAUD_RATE);

  bool initiated = initWIFI();

  // invoke only WIFI module has been setted up 
  if(initiated){
    Serial.println("WIFI Module initiated!");
    //initalize DHT sensor
    dht.begin();
  };
 
}

//this runs over and over
void loop(){
  float h = dht.readHumidity();
  // if Read temperature as Fahrenheit then set a parameter (isFahrenheit = true)
  float c = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(c)) {
    Serial.println("read temperature or humidity data error!");
    return;
  }
  
  Serial.print("read temperature:");
  Serial.println(c);
  Serial.print("read humidity:");
  Serial.println(h);

  Serial.println("begin to upload data to ThingSpeak.");

  //update ThingSpeak channel with new values
  bool isUploaded = uploadDataToThingSpeak(String(c), String(h));
 
  if (isUploaded) {
     Serial.println(" data uploaded!");
  } else {
     Serial.println(" data upload failed!");
     _blinkBuiltInLED();
  }
  //wait for delay time before attempting to post again
  delay(DELAY_TIME);
}


void _blinkBuiltInLED(){
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  for (int i = 0; i < 5; i++)  {
                                    
    delay(1000);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(2000);    
  } 

}

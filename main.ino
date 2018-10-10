//#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include "Adafruit_Si7021.h"

#define data   13 // MO(SI)
#define clock  14 // SCK

#define numStrip 2

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "SOMENETWORK" // EDIT THIS FOR YOUR USE
#define WLAN_PASS       "SOMEPASSWORD" // EDIT THIS FOR YOUR USE

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "SOMEUSER" // EDIT THIS FOR YOUR USE
#define AIO_KEY         "SOMEKEY" // EDIT THIS FOR YOUR USE

Adafruit_Si7021 sensor = Adafruit_Si7021(); // temp & humidity


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish tempMQTT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish humidMQTT = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif



unsigned long previousMillis = 0;
const long interval = 1 * 60 * 1000; // every min

uint32_t temp=0;
uint32_t humid=0;


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 60 seconds...");
       mqtt.disconnect();
       delay(60000);  // wait 60 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}



void setup() {

  #if !defined(ESP8266)
    while (!Serial) delay(1);
  #endif

  Serial.begin(115200);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  yield();

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  if (!sensor.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  /* Setup the sensor gain and integration time */
//  configureSensor();


  // get initial reading
  temp = sensor.readTemperature() * 9.0/5 + 32;
  humid = sensor.readHumidity();
}

void loop() {

  unsigned long currentMillis = millis();

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).
  MQTT_connect();

  temp = sensor.readTemperature() * 9.0/5 + 32;
  humid = sensor.readHumidity();

  if (abs(currentMillis - previousMillis) >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    Serial.print(temp);
    Serial.print(" ");
    Serial.println(humid);

    tempMQTT.publish(temp);
    humidMQTT.publish(humid);

  }

  // keep the connection alive
  mqtt.ping();

}

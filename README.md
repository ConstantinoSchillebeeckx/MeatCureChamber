# MeatCureChamber

Some quick code put together to log the temp & humidity using:

- [Adafruit Si7021](https://www.adafruit.com/product/3251)
- [Adafruit Feater Huzzah ESP8266](https://www.adafruit.com/product/2821)

## Usage

1. Install the [Arduino IDE](https://www.arduino.cc/en/main/software)
2. Follow the [guide here](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide) for conencting the IDE to the Feather
3. [Install the libraries](https://www.arduino.cc/en/guide/libraries) [MQTT](https://github.com/adafruit/Adafruit_MQTT_Library) and [Si7021](https://github.com/adafruit/Adafruit_Si7021)
4. Put together a dashboard at https://io.adafruit.com; use [this](https://learn.adafruit.com/adafruit-io-basics-dashboards) guide to help you out; for example, mine looks like ![dashboard](dashboard.png "dashboard")
5. Modify [the code](main.ino) by updating any of the lines with the comment `// EDIT THIS FOR YOUR USE`
6. Attach the sensor to your board; see [guide](https://learn.adafruit.com/adafruit-si7021-temperature-plus-humidity-sensor/arduino-code). Also see [here](https://learn.adafruit.com/assets/46249) for the pinout of the Feather. I've wired it up like this ![wiring](wiring.png "wiring")
7. Upload the code and check out the dashboard or even the Serial Monitor if hooked up to your computer.

## NOTEs

- the code is setup to send data every minute to the dashboard; at the time of writing, the **free** account allows you 30 data points per minutes which will be kept for 30 days

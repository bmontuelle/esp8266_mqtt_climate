# esp8266_mqtt_climate
Climate sensor sketch for ESP8266 publishing values to MQTT broker


## Project status
I am building automation for heating control in my office. Unlike previous similar project, I decided to use ESP8266 based moodules and MQTT to intergate with HomeAssistant. 

This project contain the sketch file I used for this purpose. It is not production tested, and may contain bugs. Feel free to open an issue or reach me if you find one.

## What does it do ?
Sketch is connecting to the wifi network and to local MQTT broker. It sets up the DHT library to read measure from the DHT11 sensor I already had lying around (there are much better alternatives)
In the main loop it will read input from sensor and if there is a difference with previous reading will publish a message to mqtt topic
There are topics for temperature and humidity

## References
https://www.elec-cafe.com/esp8266-temperature-humidity-webserver-with-a-dht11-sensor/
https://github.com/beegee-tokyo/DHTesp
https://www.home-assistant.io/blog/2015/10/11/measure-temperature-with-esp8266-and-report-to-mqtt/
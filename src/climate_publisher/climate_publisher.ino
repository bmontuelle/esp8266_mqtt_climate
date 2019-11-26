#include <DHTesp.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define dht_pin 14    //D5 of NodeMCU is GPIO14

#define wifi_ssid "YOUR_WIFI_SSID"
#define wifi_password "YOUR_WIFI_SECRET"

#define mqtt_server "mqtt.server.lan"
#define mqtt_user "YOUR_MQTT_USER"
#define mqtt_password "YOUR_MQTT_SECRET"

#define humidity_topic "sensors/office/humidity"
#define temperature_topic "sensors/office/temperature"

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  setup_wifi();

  client.setServer(mqtt_server, 1883);

  dht.setup(dht_pin, DHTesp::DHT11);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue <= prevValue - maxDiff || newValue >= prevValue + maxDiff);
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 0.1;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    float newTemp = dht.getTemperature();
    float newHum = dht.getHumidity();

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      client.publish(temperature_topic, String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }
  }
}

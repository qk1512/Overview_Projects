#include <WiFi.h>
#include <PubSubClient.h>
#include <FreeRTOS.h>
#include <task.h>

// WiFi credentials
const char* ssid = "Quang Khanh 2.4";
const char* password = "15122002";

// ThingsBoard MQTT server
const char* mqtt_server = "demo.thingsboard.io";

// ThingsBoard device token
const char* device_token = "kqfnI8lqcakEXBE9mx7X";

// Create WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", device_token, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void TempTask(void *pvParameters) {
  reconnect();
  while (true) {
    client.loop();
    float temperature = random(200, 301) / 10.0;
    String payload = "{\"temperature\": " + String(temperature, 1) + "}";
    client.publish("v1/devices/me/telemetry", payload.c_str());
    Serial.println("Publishing temperature payload: " + payload);
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Publish every 10 seconds
  }
}

void HumidityTask(void *pvParameters) {
  reconnect();
  while (true) {
    client.loop();
    float humidity = random(200, 301) / 10.0;
    String payload = "{\"humidity\": " + String(humidity, 1) + "}";
    client.publish("v1/devices/me/telemetry", payload.c_str());
    Serial.println("Publishing humidity payload: " + payload);
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Publish every 10 seconds
  }
}

void SoilMoistureTask(void *pvParameters) {
  reconnect();
  while (true) {
    client.loop();
    float soil_moisture = random(200, 301) / 10.0;
    String payload = "{\"soil_moisture\": " + String(soil_moisture, 1) + "}";
    client.publish("v1/devices/me/telemetry", payload.c_str());
    Serial.println("Publishing soil moisture payload: " + payload);
    vTaskDelay(10000 / portTICK_PERIOD_MS);  // Publish every 10 seconds
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Create tasks
  xTaskCreate(
    TempTask, "TempTask", 10000, NULL, 2, NULL);

  xTaskCreate(
    HumidityTask, "HumidityTask", 10000, NULL, 2, NULL);

  xTaskCreate(
    SoilMoistureTask, "SoilMoistureTask", 10000, NULL, 2, NULL);
}

void loop() {
  // Empty loop as FreeRTOS tasks handle everything
}

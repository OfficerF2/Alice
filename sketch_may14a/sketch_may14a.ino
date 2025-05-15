#include <WiFi.h>
#include <WebServer.h>

// Параметры Wi-Fi
const char* ssid = "Fuckers-fazenda";
const char* password = "varfik192283";

// Пин для управления светом
#define LED_PIN 18

// Веб-сервер на порту 80
WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // По умолчанию выключено

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Подключение к Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  // Включить свет
  server.on("/light/on", []() {
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/plain", "Свет включен");
    Serial.println("Свет включен");
  });

  // Выключить свет
  server.on("/light/off", []() {
    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/plain", "Свет выключен");
    Serial.println("Свет выключен");
  });

  // Получить статус в формате JSON: { "value": true/false }
  server.on("/light/status", []() {
    bool ledState = digitalRead(LED_PIN) == HIGH;
    String json = "{\"value\": " + String(ledState ? "1" : "0") + "}";
    server.send(200, "application/json", json);
    Serial.println("Получаем данные: " + json);
  });

  // Запуск сервера
  server.begin();
  Serial.println("Сервер запущен");
}

void loop() {
  server.handleClient(); // Обработка входящих запросов
}

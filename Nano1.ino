#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#include <LowPower.h>

/* ===== LED ===== */
#define LED_PIN 6

/* ===== DHT22 ===== */
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* ===== RF24 ===== */
RF24 radio(9, 10);
const byte address[6] = "12345";

/* ===== Buffers ===== */
char text[32];
char tempStr[8];
char humStr[8];

/* ===== Sleep ===== */
const int sleepCycles = 2;   // 16s

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);   // LED OFF

  dht.begin();
  delay(3000);

  if (!radio.begin()) {
    while (1);
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  radio.powerDown();

  Serial.println("--- Node 1 (DHT22) Started ---");
}

void loop() {
  /* ===== ACTIVE MODE START ===== */
  digitalWrite(LED_PIN, HIGH);   // LED ON

  radio.powerUp();
  delay(5);

  delay(2000);
  dht.readTemperature();  // discard
  dht.readHumidity();
  delay(500);

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    dtostrf(temperature, 5, 2, tempStr);
    dtostrf(humidity,    5, 2, humStr);

    snprintf(text, sizeof(text),
             "N1 T=%sC H=%s%%",
             tempStr, humStr);

    radio.write(text, sizeof(text));
    Serial.println(text);
  }

  radio.powerDown();

  /* ===== ACTIVE MODE END ===== */
  digitalWrite(LED_PIN, LOW);    // LED OFF

  Serial.flush();

  for (int i = 0; i < sleepCycles; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

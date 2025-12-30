#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LowPower.h>

/* ===== LED ===== */
#define LED_PIN 6

/* ===== LDR ===== */
#define LDR_PIN A0

/* ===== RF24 ===== */
RF24 radio(9, 10);
const byte address[6] = "12345";

/* ===== Buffers ===== */
char text[32];

/* ===== Sleep ===== */
const int sleepCycles = 3;   // 24s

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(LDR_PIN, INPUT);

  if (!radio.begin()) {
    while (1);
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  radio.powerDown();

  Serial.println("--- Node 2 (LDR) Started ---");
}

void loop() {
  /* ===== ACTIVE MODE START ===== */
  digitalWrite(LED_PIN, HIGH);   // LED ON

  radio.powerUp();
  delay(5);

  int ldrValue = analogRead(LDR_PIN);

  snprintf(text, sizeof(text),
           "N2 LDR=%d",
           ldrValue);

  radio.write(text, sizeof(text));
  Serial.println(text);

  radio.powerDown();

  /* ===== KEEP NODE ACTIVE FOR 2 SECONDS ===== */
  delay(2000);   // <<< ACTIVE TIME = 2s

  /* ===== ACTIVE MODE END ===== */
  digitalWrite(LED_PIN, LOW);    // LED OFF

  Serial.flush();

  for (int i = 0; i < sleepCycles; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 4

#define DHTTYPE DHT11

// LiquidCrystal lcd(9, 8, 13, 12, 11, 10); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

int ldrPin = A1;
int ldrValue = 0;
int iluminacaoInterior = 9;

int fanOnLed = 8;
int fanOffLed = 7;
int fanPin = 6;

int buzzerPin = 10;
int echoPin = 2;
int trigPin = 3;
int redLed = 4;
int greenLed = 5;

int interruptBtn = 13;

long duration;
float distance;
float temperatura;
float humidade;

bool isFanOn = false;
bool isOn = true;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // lcd.begin(16, 2);
  Serial.begin(9600);
  // Serial.println(F("In Setup function"));

  pinMode(iluminacaoInterior, OUTPUT);
  pinMode(fanOnLed, OUTPUT);
  pinMode(fanOffLed, OUTPUT);
  pinMode(fanPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(interruptBtn, INPUT_PULLUP);

  digitalWrite(fanPin, LOW);

  // LCD SETUP
  // lcd.begin(16, 2);
  // lcd.print("Trabalho Pratico");
  // lcd.setCursor(0, 1);
  // lcd.print("21-23/22/29");
  // lcd.noCursor(); // Hides the LCD cursor
  // delay(2000);

  xTaskCreate(system1, "Task1", 100, NULL, 1, NULL);
  // xTaskCreate(system2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(system3, "Task3", 100, NULL, 3, NULL);

  attachInterrupt(digitalPinToInterrupt(interruptBtn), interruptHandler, FALLING);
}

void loop() {}

void interruptHandler()
{
  isOn = !isOn
}

// The following function is Task1. We display the task label on Serial monitor
static void system1(void *pvParameters)
{
  while (1)
  {
    // if (isOn) {

    ldrValue = analogRead(ldrPin);
    Serial.print("LDR value is :");
    Serial.print(ldrValue);
    Serial.print("\n");
    if (ldrValue < 600)
    {
      digitalWrite(iluminacaoInterior, HIGH);
    }
    else
    {
      digitalWrite(iluminacaoInterior, LOW);
    }
    Serial.println(F("readLDR"));
    //}
    vTaskDelay(120 / portTICK_PERIOD_MS);
  }
}

static void system2(void *pvParameters)
{
  for (;;)
  {
    // if (isOn) {
    temperatura = dht.readTemperature();

    if (temperatura >= 19)
    {
      isFanOn = true;
      digitalWrite(fanPin, HIGH);
    }
    else if (temperatura <= 18)
    {
      isFanOn = false;
      digitalWrite(fanPin, LOW);
    }

    if (isFanOn)
    {
      digitalWrite(fanOnLed, HIGH);
      digitalWrite(fanOffLed, LOW);
    }
    else
    {
      digitalWrite(fanOnLed, LOW);
      digitalWrite(fanOffLed, HIGH);
    }

    Serial.print("Temperatura:");
    Serial.print(temp);
    Serial.print("*C | Fan: ");
    if (isFanOn)
      Serial.print("ON");
    else
      Serial.print("OFF");
    // writeTempFanState(isFanOn);

    //}
    vTaskDelay(140 / portTICK_PERIOD_MS);
  }
}

void sound(int distance)
{
  if (distance <= 20 && distance >= 16)
  {

    analogWrite(redLed, 50);

    tone(buzzerPin, 2000);
    delay(800);
    noTone(buzzerPin);
    delay(300);
  }
  else if (distance <= 15 && distance >= 11)
  {

    analogWrite(redLed, 100);

    tone(buzzerPin, 2000);
    delay(500);
    noTone(buzzerPin);
    delay(300);
  }
  else if (distance <= 10 && distance >= 6)
  {

    analogWrite(redLed, 150);

    tone(buzzerPin, 2000);
    delay(200);
    noTone(buzzerPin);
    delay(100);
  }
  else if (distance <= 5 && distance >= 3)
  {

    analogWrite(redLed, 255);

    tone(buzzerPin, 2000);
  }
  else
  {
    noTone(buzzerPin);
  }
}

static void system3(void *pvParameters)
{
  while (1)
  {
    // if (isOn) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, OUTPUT);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");

    if (distance <= 20 && distance > 3.0)
    {
      sound(distance);
      digitalWrite(greenLed, LOW);
      delay(500);
    }
    else if (distance <= 3)
    {
      noTone(buzzerPin);
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      delay(500);
    }
    else
    {

      noTone(buzzerPin);
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, LOW);
    }
    Serial.println(F("distance"));
    //}
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include "DHT.h"

#define DHTPIN A0 // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11 // DHT 11

LiquidCrystal lcd(9, 8, 13, 12, 11, 10); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

int ldrPin = A1;
int ldrValue = A2;
int iluminacaoInterior = 9;

int fanOnLed = 8;
int fanOffLed = 7;
int fanPin = 6;

int buzzerPin = 1;
int echoPin = 2;
int trigPin = 3;
int redLed = 4;
int greenLed = 5;

long duration;
float distance;

bool isFanOn = false;

void setup()
{
  // Initialize the Serial Monitor with 9600 baud rate
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println(F("In Setup function"));
  // Set the digital pins 8 to 11 as output
  dht.begin();

  pinMode(iluminacaoInterior, OUTPUT);
  pinMode(fanOnLed, OUTPUT);
  pinMode(fanOffLed, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(tempPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(fanPin, LOW);

  // LCD SETUP
  lcd.begin(16, 2);              // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  lcd.print("Trabalho Pratico"); // Prints "Arduino" on the LCD
  lcd.setCursor(0, 1);
  lcd.print("21-23/22/29");
  lcd.noCursor(); // Hides the LCD cursor
  delay(2000);
  // Create three tasks with labels Task1, Task2 and Task3 and assign the priority as 1, 2 and 3
  // respectively. We also create fourth task labeled as IdelTask when the there is no task in
  // operation and it has the highest priority.
  xTaskCreate(system1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(system2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(system3, "Task2", 100, NULL, 3, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
}

void loop()
{
  // There is no instruction in loop section of the code
}

// The following function is Task1. We display the task label on Serial monitor
static void system1(void *pvParameters)
{
  while (1)
  {
    value = analogRead(ldrPin);     // Reads the Value of LDR(light).
    Serial.print("LDR value is :"); // Prints the value of LDR to Serial Monitor.
    Serial.print(value);
    Serial.print("\n");
    if (value > 600)
    {
      digitalWrite(iluminacaoInterior, HIGH); // Makes the LED glow in Dark.
    }
    else
    {
      digitalWrite(iluminacaoInterior, LOW); // Turns the LED OFF in Light.
    }
    Serial.println(F("readLDR"));
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void writeTempFanState(float temp, bool fanState)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print("*C");

  lcd.setCursor(0, 1);
  lcd.print("Fan:");
  if (fanState)
    lcd.print("ON");
  else
    lcd.print("OFF");
  lcd.noCursor();
}

float readTemperature()
{
  // Wait a few seconds between measurements.
  delay(2000);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Temperatura: "));
  Serial.print(t);
  Serial.print(F("C \n"));

  return t;
}

// Simlilarly this is task2
static void system2(void *pvParameters)
{
  while (1)
  {
    float cel = readTemperature();
    Serial.println(cel);

    if (cel >= 19)
    {
      isFanOn = true;
      digitalWrite(fanPin, HIGH);
    }
    else if (cel <= 18)
    {
      isFanOn = false;
      digitalWrite(fanPin, LOW);
    }

    if (isFanOn)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
    }
    else
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    }

    writeTempFanState(cel, isFanOn);

    delay(1000);
    Serial.println(F("Task2"));
    vTaskDelay(110 / portTICK_PERIOD_MS);
  }
}

void sound(int distance)
{
  if (distance <= 200 && distance >= 150)
  {

    analogWrite(redLed, 50);

    digitalWrite(buzzerPin, HIGH);
    delay(800);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  }
  else if (distance <= 150 && distance >= 100)
  {

    analogWrite(redLed, 100);

    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  }
  else if (distance <= 100 && distance >= 50)
  {

    analogWrite(redLed, 150);

    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
  else
  {

    analogWrite(redLed, 255);

    digitalWrite(buzzerPin, HIGH);
    delay(50);
    digitalWrite(buzzerPin, LOW);
    delay(50);
  }
}

// Simlilarly this is task3
static void system3(void *pvParameters)
{
  while (1)
  {
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

    if (distance <= 200 && distance > 5.0)
    {
      sound(distance);
      digitalWrite(greenLed, LOW);
      delay(500);
    }
    else if (distance <= 5)
    {
      digitalWrite(buzzerPin, LOW);
      Serial.println("foder home");
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      delay(500);
    }
    else
    {
      digitalWrite(buzzerPin, LOW);
      Serial.println("esta longe");
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, LOW);
    }
    Serial.println(F("readLDR"));
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// This is the idle task wich has the most higher priority and calls when no task is running
static void MyIdleTask(void *pvParameters)
{
  while (1)
  {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    Serial.println(F("Idle state"));
    delay(50);
  }
}
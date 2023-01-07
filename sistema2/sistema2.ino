#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include "DHT.h"

#define DHTPIN 4 // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11 // DHT 11

// LiquidCrystal lcd(9, 8, 13, 12, 11, 10); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

int fanOnLed = 8;
int fanOffLed = 7;
int fanPin = 6;

float temperatura;

bool isFanOn = false;
bool isOn = true;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);

  dht.begin();

  pinMode(fanPin, OUTPUT);
  pinMode(fanOffLed, OUTPUT);
  pinMode(fanOnLed, OUTPUT);
  digitalWrite(fanPin, LOW);

  // LCD SETUP
  /*lcd.begin(16, 2);
  lcd.print("Trabalho Pratico");
  lcd.setCursor(0, 1);
  lcd.print("21-23/22/29");
  lcd.noCursor();*/
  delay(2000);
}

void writeTempFanState(bool fanState)
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

void loop()
{

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

  delay(1000);
}

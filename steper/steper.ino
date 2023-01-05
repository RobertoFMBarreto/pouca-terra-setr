#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11


LiquidCrystal lcd(9, 8, 13, 12, 11, 10); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 

int fanPin = 7;
int tempPin = A0;
int val;
int redLed = 3;
int greenLed = 2;

bool isFanOn = false;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  pinMode(fanPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(fanPin, LOW);

  // LCD SETUP
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  lcd.print("Trabalho Pratico"); // Prints "Arduino" on the LCD 
  lcd.setCursor(0,1);
  lcd.print("21-23/22/29"); 
  lcd.noCursor(); // Hides the LCD cursor 
  delay(2000);
}

void writeTempFanState(float temp, bool fanState){
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Temp:");
  lcd.print(temp); 
  lcd.print("*C");

  lcd.setCursor(0,1); 
  lcd.print("Fan:");
  if(fanState)
    lcd.print("ON"); 
  else
    lcd.print("OFF");
  lcd.noCursor(); 
}


float readTemperature(){
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));

  return t;
}

void loop()
{
  
  float cel= readTemperature();
  Serial.println(cel);

  if(cel >= 19){
    isFanOn = true;
    digitalWrite(fanPin, HIGH);
    
  }
  else if(cel <= 18){
    isFanOn = false;
    digitalWrite(fanPin, LOW);
  }

  if(isFanOn){
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }else{
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }

  writeTempFanState(cel,isFanOn);

  delay(1000);
}



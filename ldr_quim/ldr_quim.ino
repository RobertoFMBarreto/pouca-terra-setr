int ldr=A0;
int value=0;
void setup() {
Serial.begin(9600);
pinMode(3,OUTPUT);
}

void loop() {
value=analogRead(ldr);//Reads the Value of LDR(light).
Serial.print("LDR value is :");//Prints the value of LDR to Serial Monitor.
Serial.print(value);
Serial.print("\n");
if(value>600)
  {
    digitalWrite(3,HIGH);//Makes the LED glow in Dark.
  }
  else
  {
    digitalWrite(3,LOW);//Turns the LED OFF in Light.
  }
}
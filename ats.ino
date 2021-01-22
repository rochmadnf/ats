#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

#define RelayGenset 5
#define RelayPLN 4

PZEM004Tv30 ats(2,3);
float tegangan, arus;

void setup() {
  Serial.begin(115200);
  pinMode(RelayGenset, OUTPUT);
  pinMode(RelayPLN, OUTPUT);
  lcd.begin();
  lcd.backlight();
}
void showTegangan(){
  lcd.setCursor(0, 0);
  lcd.print("TEGANGAN");  
}

void loop() {
  tegangan = ats.voltage();
  arus = ats.current();

  if(tegangan > 0){
    Serial.println("PLN Nyala");
    digitalWrite(RelayGenset, LOW);
    digitalWrite(RelayPLN, HIGH);

    // OUTPUT
    lcd.clear();
    showTegangan();
    lcd.setCursor(17, 0);
    lcd.print(tegangan);
    lcd.setCursor(0,1);
    lcd.print("PLN");
    lcd.setCursor(17, 1);
    lcd.print("ON");
    lcd.setCursor(0, 2);
    lcd.print("GENSET");
    lcd.setCursor(17, 2);
    lcd.print("OFF");
  }else{
    Serial.println("Genset Nyala");
    digitalWrite(RelayPLN, LOW);
    digitalWrite(RelayGenset, HIGH);
    
    // OUTPUT 
    lcd.clear();
    showTegangan();
    lcd.setCursor(19, 0);
    lcd.print(0);
    lcd.setCursor(0,1);
    lcd.print("PLN");
    lcd.setCursor(17, 1);
    lcd.print("OFF");
    lcd.setCursor(0, 2);
    lcd.print("GENSET");
    lcd.setCursor(18, 2);
    lcd.print("ON");
  }
  delay(3500);
}

/*
  code by Rochmad (https://www.github.com/rochmadnf/ats.git)
*/

#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

#define RelayGenset 5
#define RelayPLN 4

PZEM004Tv30 ats(2, 3);
float tegangan, arus;

void setup()
{
  Serial.begin(115200);
  pinMode(RelayGenset, OUTPUT);
  pinMode(RelayPLN, OUTPUT);
  lcd.begin();
  lcd.backlight();
}
void showTegangan()
{
  lcd.setCursor(0, 0);
  lcd.print("TEGANGAN");
}

/*
  Keterangan Array DATA
  0 => Nilai Tegangan
  1 => Kolom Tegangan
  2 => Kolom ON/OFF GENSET
  3 => Status PLN
  4 => Status GENSET
*/

void showData(int DATA[])
{
  lcd.setCursor(DATA[1], 0);
  lcd.print(DATA[0]);
  lcd.setCursor(0, 1);
  lcd.print("PLN");
  lcd.setCursor(17, 1);
  (DATA[3] == 1) ? lcd.print("ON") : lcd.print("OFF");
  lcd.setCursor(0, 2);
  lcd.print("GENSET");
  lcd.setCursor(DATA[2], 2);
  (DATA[4] == 1) ? lcd.print("ON") : lcd.print("OFF");
}

void switchPG(int pln, int genset)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SWITCH");
  lcd.setCursor(0, 1);
  (pln == 1) ? lcd.print("PLN") : lcd.print("GENSET");
  lcd.setCursor(0, 2);
  lcd.print("TO");
  lcd.setCursor(0, 3);
  (genset == 0) ? lcd.print("GENSET") : lcd.print("PLN");
  delay(2000);
}

void noKedip(int volt, int stat)
{
  int kedip = volt;
  /*
  0 => PLN
  1 => GENSET
  */

  if (stat = 0)
  {
    while (kedip = 0)
    {
      kedip = ats.voltage();
      delay(500);
    }
  }
  else
  {
    while (kedip > 0)
    {
      kedip = ats.voltage();
      delay(500);
    }
  }
}

void loop()
{
  tegangan = ats.voltage();
  arus = ats.current();

  // > 0
  if (tegangan > 0)
  {
    Serial.println("PLN Nyala");
    digitalWrite(RelayGenset, LOW);
    digitalWrite(RelayPLN, HIGH);

    // OUTPUT
    lcd.clear();
    showTegangan();
    int dataArray[5] = {tegangan, 17, 17, 1, 0};
    showData(dataArray);
    noKedip(tegangan, 0);
    // switchPG(1, 0);
  }

  // NAN
  else
  {
    Serial.println("Genset Nyala");
    digitalWrite(RelayPLN, LOW);
    digitalWrite(RelayGenset, HIGH);

    // OUTPUT
    lcd.clear();
    showTegangan();
    int dataArray[5] = {0, 19, 18, 0, 1};
    showData(dataArray);
    noKedip(tegangan, 1);
    // switchPG(0, 1);
  }
  delay(1000);
}

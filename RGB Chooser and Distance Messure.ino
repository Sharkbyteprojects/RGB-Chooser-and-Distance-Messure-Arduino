#define Echo_EingangsPin 4
#define Trigger_AusgangsPin 3
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
boolean choosermode = true;
int eintrag = 0;
int modeset = 0;
int msd = 0;
int mode = 0;
int chang = 0;
int chmodeb = true;
int changb = 0;
int maximumRange = 660;
int minimumRange = 2;
long Abstand;
long Dauer;
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("Starting up");
  Serial.begin(9600);
  delay(3000);
  pinMode(2, INPUT);
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(Trigger_AusgangsPin, OUTPUT);
  pinMode(Echo_EingangsPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  chang = digitalRead(7);
  modeset = digitalRead(2);
  lcd.backlight();
  //Serial.print(modeset);
  if (modeset != msd && !choosermode) {
    if (modeset == HIGH) {
      choosermode = true;
    } msd = modeset;
  }
  if (chmodeb != choosermode) {
    lcd.clear();
  } chmodeb = choosermode;
  if (choosermode) {
    lcd.setCursor(0, 0);
    if (eintrag == 0) {
      lcd.print("> RGB Chooser");
      lcd.setCursor(0, 1);
      lcd.print(" Distance  ");
      if (chang != changb) {
        if (chang == HIGH) {
          eintrag = 1;
        }
      }
    } else {
      lcd.print(" RGB Chooser ");
      lcd.setCursor(0, 1);
      lcd.print("> Distance "); if (chang != changb) {
        if (chang == HIGH) {
          eintrag = 0;
        }
      }
    }
    if (modeset != msd ) {
      if (modeset == HIGH) {
        choosermode = false;
        mode = eintrag;
      }
    }
  } else {
    lcd.setCursor(0, 0);
    if (mode == 0) {
      int max = 1023;
      int r = 0; //11
      int g = 0; //9
      int b = 0; //10
      r = analogRead(A1);
      g = analogRead(A2);
      b = analogRead(A3);
      int rm = map(r, 0, max, 0, 255);
      int gm = map(g, 0, max, 0, 255);
      int bm = map(b, 0, max, 0, 255);
      lcd.print("RGB Chooser");
      lcd.setCursor(0, 1);
      lcd.print("R");
      lcd.print(rm);
      lcd.print(" G");
      lcd.print(gm);
      lcd.print(" B");
      lcd.print(bm);
      lcd.print("      ");
      analogWrite(9, gm);
      analogWrite(10, bm);
      analogWrite(11, rm);
      Serial.print(rm);
      Serial.print("\t");
      Serial.print(gm);
      Serial.print("\t");
      Serial.print(bm);
      Serial.print("\n");
    } else {
      digitalWrite(Trigger_AusgangsPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trigger_AusgangsPin, LOW);
      Dauer = pulseIn(Echo_EingangsPin, HIGH);
      Abstand = Dauer / 58.2;
      lcd.setCursor(0, 0);
      lcd.print("Ultra Distance");
      lcd.setCursor(0, 1);
      if (Abstand >= maximumRange || Abstand <= minimumRange) {
        lcd.print("To far away      ");
      } else {
        lcd.print(Abstand);
        lcd.print(" cm          ");
        }
    }
  }
  changb = chang;
  msd = modeset;
}

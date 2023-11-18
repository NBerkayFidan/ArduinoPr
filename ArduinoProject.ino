#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo motor;

int basamak = 0;
const byte satir = 4;
const byte sutun = 4;
String girilen_sifre = "";
String sifre = "";
char tus;
boolean ok, ds = false;

char tus_takimi[satir][sutun] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte satirpin[satir] = { 13, 12, 11, 10 };
byte sutunpin[sutun] = { 9, 8, 7, 6 };

Keypad tus_takimim = Keypad(makeKeymap(tus_takimi), satirpin, sutunpin, satir, sutun);

void setup() {
  motor.attach(5);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(1, OUTPUT);
  digitalWrite(3, HIGH);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fid<a>n Software");
  delay(3000);
  lcd.clear();
}
void verwijderen() {
  if (tus == 'B') {
    if (basamak > 0) {
      basamak--;
      lcd.setCursor(basamak, 1);
      lcd.print(" ");
      girilen_sifre = girilen_sifre.substring(0, basamak);
    }
  }
}



String invoer(char blngrgtl) {
  tus = tus_takimim.getKey();
  while (tus != blngrgtl) {
    if (tus != NO_KEY && tus != 'B') {
      lcd.setCursor(basamak, 1);
      lcd.print('*');
      girilen_sifre += tus;
      basamak++;
    }
    verwijderen();
    tus = tus_takimim.getKey();
  }
  basamak = 0;
  return girilen_sifre;
}
String wachtwoordwzg() {
  sifre = invoer('D');
  girilen_sifre = "";
  ok = true;
  return sifre;
}

void controleer(String cntrl) {
  if (girilen_sifre == sifre) {
    lcd.setCursor(0, 1);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(1,HIGH);
    delay(3000);
    digitalWrite(1,LOW);
    lcd.print("Correct Password");
    ds = true;
    motor.write(180);
    delay(1000);
    digitalWrite(2, LOW);
    motor.write(80);
    delay(1000);
    lcd.clear();
    girilen_sifre = "";
    basamak = 0;
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Wrong Password");
     for (int i = 0; i < 3; i++) {
      digitalWrite(3, LOW);
      delay(300);
      digitalWrite(3, HIGH);
      delay(300);
    }
    ds = false;
    delay(1000);
    lcd.clear();
    girilen_sifre = "";
    basamak = 0;
  }
}

void loop() {
  lcd.setCursor(0, 0);
  if (ok == false) {
    lcd.print("Nieuw Wachtwoord:");
    wachtwoordwzg();
    ok = true;
    lcd.clear();
  }
  lcd.print("Wachtwoord:");
  controleer(invoer('#'));
  lcd.clear();
  if (ds == true) {
    char yeniSifreTus;
    lcd.print("Wilt u een nieuw");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("wachtwoord?");
    delay(1000);
    lcd.clear();
    do {
      lcd.setCursor(0, 0);
      lcd.print("1:voor ja");
      lcd.setCursor(0, 1);
      lcd.print("0:voor nee");
      yeniSifreTus = tus_takimim.getKey();
      delay(100);
    } while (yeniSifreTus == NO_KEY || (yeniSifreTus != '1' && yeniSifreTus != '0'));
    lcd.clear();
    if (yeniSifreTus == '1') {
      ok = false;
    } else {
      ok = true;
      ds = false;
    }
  }
}

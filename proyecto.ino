#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

Servo cerrojo;

#define SERVO_PIN 8
#define LED_VERDE 6
#define LED_ROJO 7

void abrirPuerta() {

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO, LOW);

  lcd.clear();
  lcd.print("Acceso concedido");

  cerrojo.write(180);

  delay(2100);

  cerrojo.write(0);

  digitalWrite(LED_VERDE, LOW);

  lcd.clear();
  lcd.print("Puerta Cerrada");

  delay(900);

  lcd.clear();
 lcd.setCursor(0, 0);
  lcd.print("Acerque la ");
  lcd.print("    ");

 lcd.setCursor(0, 1);
  lcd.print("Targeta o llavero");
  lcd.print("    ");
}

void accesoDenegado() {

  digitalWrite(LED_ROJO, HIGH);

  lcd.clear();
  lcd.print("Acceso Denegado");

  delay(1000);

  digitalWrite(LED_ROJO, LOW);

lcd.clear();
 lcd.setCursor(0, 0);
  lcd.print("Acerque la ");
  lcd.print("    ");

 lcd.setCursor(0, 1);
  lcd.print("Targeta o llavero");
  lcd.print("    ");
}

String obtenerUID() {

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (i > 0) uid += " ";

    if (rfid.uid.uidByte[i] < 0x10)
      uid += "0";

    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();

  return uid;
}

void setup() {

  Serial.begin(9600);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();

  cerrojo.attach(SERVO_PIN);
  cerrojo.write(0);

  lcd.begin(16, 2);

  lcd.clear();
  lcd.print("Sistema Listo");

  delay(500);

    lcd.clear();
 lcd.setCursor(0, 0);
  lcd.print("Acerque la ");
  lcd.print("    ");

 lcd.setCursor(0, 1);
  lcd.print("Targeta o llavero");
  lcd.print("    ");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  String uid = obtenerUID();

  Serial.print("UID: ");
  Serial.println(uid);

  // Tarjetas autorizadas
  if (
      uid == "E3 B8 F8 33") {

    abrirPuerta();

  } else {

    accesoDenegado();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
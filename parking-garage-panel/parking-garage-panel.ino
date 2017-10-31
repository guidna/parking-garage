#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int lcdPin = A0;

String msgStart      = "Park-Garage ACG";
String msgNoVacancy  = "NAO HA VAGAS";
String msgVacancy    = "Vagas disp: ";
String msgCnxErro    = "System out";

void setup()
{
  serialSetup();
  lcdSetup();
}

void loop()
{
  int qtdVacancy = getVacancy();
  showVacancy(qtdVacancy);
  lcdBlink(qtdVacancy);

}

void serialSetup() {
  Serial.begin(9600);
  while (!Serial) {    
  }
  Serial.println("Inicio");
}

void lcdSetup() {
   lcd.begin(16, 2);
   analogWrite(lcdPin, 0);
   lcd.setCursor(0, 0);
   lcd.print(msgStart);
   delay(3000);
}

void showVacancy(int vagas) {
  String msg;

  // estabelecer qual mensagem 'principal' a ser exibida
  if(vagas > 0) {
    msg = msgVacancy;
  } else if (vagas == 0) {
    msg = msgNoVacancy;
  } else {
    msg = msgCnxErro;
  }

  lcd.setCursor(0,1);
  lcd.print(msg);

  // escrever mensagens adicionais
  if(vagas > 0) {
    lcd.setCursor(13, 1);
    lcd.print(vagas);   
  }
  
  delay(3000);
}

int getVacancy() {
  return 0;
}

void lcdBlink(int vagas) {

  if(vagas >= 0) {
    analogWrite(lcdPin, 1023);
    delay(1000);
    analogWrite(lcdPin, 0);
    delay(1000);
  }

}


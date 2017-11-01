#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int lcdPin = A0;

String msgStart      = "Park-Garage ACG";
String msgNoVacancy  = "NAO HA VAGAS";
String msgVacancy    = "Vagas disp: ";
String msgCnxErro    = "System out";

#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13);

boolean previousStatusVacancy;
boolean currentStatusVacancy;
String statusVacancy = "";

const int vacancyStatusLED = 7;

const int mqttStatusOnLED = 6;
const int mqttStatusOffLED = 5;

void setup()
{
  serialSetup();
  lcdSetup();

  //LEDs vaga disponível e MQTT status
  pinMode(vacancyStatusLED, OUTPUT);
  pinMode(mqttStatusOnLED, OUTPUT);
  pinMode(mqttStatusOffLED, OUTPUT);
  
}

void loop()
{
  int qtdVacancy = getVacancy();
  showVacancy(qtdVacancy);
  lcdBlink(qtdVacancy);

//A função abaixo recebe o boolean mqttStatus
  showMqttStatusLED(0);
  
  verifyStatusVacancy();
      
  if(currentStatusVacancy != previousStatusVacancy){
    
    printStatusVacancy();

    previousStatusVacancy = currentStatusVacancy;
    
  }
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

void printStatusVacancy() {

  if (currentStatusVacancy){
    statusVacancy = "livre";
    digitalWrite(vacancyStatusLED,HIGH);
  }else{
    statusVacancy = "ocupada";
    digitalWrite(vacancyStatusLED,LOW);
  }
  Serial.print("Vaga: ");
  Serial.println(statusVacancy);

}

void verifyStatusVacancy() {

  int distancia = ultrasonic.distanceRead();

  if (distancia > 50) {
    currentStatusVacancy = true;
  } else {
    currentStatusVacancy = false;
  }
  
}

void showMqttStatusLED (boolean mqttStatus){

  digitalWrite(mqttStatusOnLED, mqttStatus);
  digitalWrite(mqttStatusOffLED, !mqttStatus);
  
}


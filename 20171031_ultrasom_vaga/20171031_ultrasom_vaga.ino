#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 13);

boolean previousStatusVacancy;
boolean currentStatusVacancy;
String statusVacancy = "";

const int vacancyStatusLED = 7;

const int mqttStatusOnLED = 6;
const int mqttStatusOffLED = 5;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  pinMode(vacancyStatusLED, OUTPUT);
  pinMode(mqttStatusOnLED, OUTPUT);
  pinMode(mqttStatusOffLED, OUTPUT);
}

void loop() {
  //A função abaixo recebe o boolean mqttStatus
  showMqttStatusLED(0);
  
  verifyStatusVacancy();
      
  if(currentStatusVacancy != previousStatusVacancy){
    
    printStatusVacancy();

    previousStatusVacancy = currentStatusVacancy;
    
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


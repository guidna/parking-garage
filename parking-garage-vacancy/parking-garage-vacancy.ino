#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Ultrasonic.h>

/******************************************
                   MQTT
*******************************************/

char msgQTTWill[]    = "Client Panel #02 off";

char topicPub[]  = "vagas/02";
char topicWill[] = "vagas/02";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0x02 };
IPAddress ip(192,168,3,186);
int port = 1883;

char clientMQTTID[] = "MQTT-senai-sp-kit02";

EthernetClient ethClient;
PubSubClient client(ip,port, ethClient);


/******************************************
              Ultrassom
*******************************************/

Ultrasonic ultrasonic(8, 9);

boolean previousStatusVacancy;
boolean currentStatusVacancy;
String statusVacancy = "";

const int vacancyStatusLED = 7;

const int mqttStatusOnLED = 6;
const int mqttStatusOffLED = 5;

void setup() {
  serialSetup();
  
  //client.setClient(ethClient);
  
  ethernetSetup();
  
  setupLEDS ();
}

void loop() {

  reconnectMQTT();

  //A função abaixo recebe o boolean mqttStatus
  showMqttStatusLED(client.connected());

  verifyStatusVacancy();

  if (currentStatusVacancy != previousStatusVacancy) {

    printStatusVacancy();

    previousStatusVacancy = currentStatusVacancy;

  }
}

void printStatusVacancy() {

  if (currentStatusVacancy) {
    statusVacancy = "livre";
    digitalWrite(vacancyStatusLED, HIGH);

    if (client.connected()) {
      client.publish(topicPub, "1");
    }
    Serial.println("MQTT - Conectado");
    Serial.println(currentStatusVacancy);
  } else {
    statusVacancy = "ocupada";
    digitalWrite(vacancyStatusLED, LOW);

    if (client.connected()) {
      client.publish(topicPub, "0");
    }
    Serial.println("MQTT - Não conectado");
    Serial.println(!currentStatusVacancy);
    
  }
  Serial.print("Vaga: ");
  Serial.println(statusVacancy);

}

void verifyStatusVacancy() {

  int distancia = ultrasonic.distanceRead();

  if (distancia > 30) {
    currentStatusVacancy = true;
  } else {
    currentStatusVacancy = false;
  }

}

void showMqttStatusLED (boolean mqttStatus) {

  digitalWrite(mqttStatusOnLED, mqttStatus);
  digitalWrite(mqttStatusOffLED, !mqttStatus);
  Serial.println(mqttStatus);

}

void serialSetup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Inicio");
}

void ethernetSetup() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha em configurar Ethernet usando DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("Ethernet Ok...");
}

void setupLEDS() {
  pinMode(vacancyStatusLED, OUTPUT);
  pinMode(mqttStatusOnLED, OUTPUT);
  pinMode(mqttStatusOffLED, OUTPUT);
  Serial.println("OK");
}

void reconnectMQTT() {

  char messageMQTT[] = "Online";
  
  if (!client.connected()) {

    Serial.print("Conectando MQTT ...");

    //       if (client.connect(clientMQTTID,userMQTT,passMQTT,topicWill,0,false,msgQTTWill)) {
    if (client.connect(clientMQTTID, topicWill, 0, false, msgQTTWill)) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" nova tentativa em 5 segundos");
    }
  }

}

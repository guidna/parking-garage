#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Ultrasonic.h>

/******************************************
                   MQTT
*******************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("callback");
}

//char msgQTTWill[]    = "Client Panel #02 off";

char topicPub[]  = "vagas/02";
//char topicWill[] = "vagas/will";
//char topicSub[]  = "senai-vagas/02";
//char topicWill[] = "senai-vagas/will";


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0xA2 };
IPAddress ip (192, 162, 1, 2);

IPAddress server (192, 168, 3, 186);
//char server[] = "test.mosquitto.org";
int port = 1883;

char clientMQTTID[] = "MQTT-senai-sp-kit02";

EthernetClient ethClient;
PubSubClient client(server,port,callback,ethClient);

/******************************************
              Ultrassom
*******************************************/

Ultrasonic ultrasonic(8, 9);

boolean previousStatusVacancy = 0;
boolean currentStatusVacancy = 1;

String statusVacancy = "";

const int vacancyStatusLED = 7;

const int mqttStatusOnLED = 6;
const int mqttStatusOffLED = 5;

void setup() {

  serialSetup();
  ethernetSetup();
  setupLEDS ();
  
}

void loop() {

  reconnectMQTT();
   
  showMqttStatusLED(client.connected());

  verifyStatusVacancy();

  if (currentStatusVacancy != previousStatusVacancy) {
     printStatusVacancy();
     previousStatusVacancy = currentStatusVacancy;
  }
  
}

void printStatusVacancy() {

  char statusVagas;
  boolean retPublish;
  
  if (currentStatusVacancy) {
    
     statusVacancy = "livre";
     digitalWrite(vacancyStatusLED, HIGH);
     statusVagas = '1';

  } else {
    
     statusVacancy = "ocupada";
     digitalWrite(vacancyStatusLED, LOW);
     statusVagas = '0';
     
  }
  
  Serial.print("Vaga: ");
  Serial.println(statusVacancy);

  if(client.connected()) {
     if(client.publish(topicPub,statusVagas,true)) {
        Serial.println("Publicado com sucesso");
     } else {
        Serial.println("Erro na publicacao");
     }
  }

}

void verifyStatusVacancy() {
/*
  int distancia = ultrasonic.distanceRead();

  if (distancia > 30) {
    currentStatusVacancy = true;
  } else {
    currentStatusVacancy = false;
  }
*/

}

void showMqttStatusLED (boolean mqttStatus) {

  digitalWrite(mqttStatusOnLED, mqttStatus);
  digitalWrite(mqttStatusOffLED, !mqttStatus);

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
  Serial.println("LEDs Ok...");
}

void reconnectMQTT() {

    char messageMQTT[] = "Online";
    if (!client.connected()) {
      
       Serial.print("Conectando MQTT ...");
    
       if (client.connect(clientMQTTID)) {
          Serial.println("conectado"); 
          client.publish(topicPub,"0",true);   
        
       } else {
          Serial.print("falha, rc=");
          Serial.print(client.state());
          Serial.println(" nova tentativa em 5 segundos");        
       }
   }

}

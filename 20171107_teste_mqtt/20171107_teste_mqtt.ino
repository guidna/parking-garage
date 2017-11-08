#include <PubSubClient.h>
#include <Ethernet.h>
#include <SPI.h>

const int mqttStatusOnLED = 6;
const int mqttStatusOffLED = 5;

//=======================================================================
void callback(const char topic, byte* payload, unsigned int length);
//=======================================================================

char msgQTTWill[]    = "Client Panel #03 off";

char topicPub[]  = "senai-code-xp/vagas/03";
char topicWill[] = "senai-code-xp/vagas/03";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0x03 };
IPAddress ip (192, 162, 1, 3);
char server[] = "test.mosquitto.org";
int port = 1883;

char clientMQTTID[] = "MQTT-senai-sp-kit02";
char userMQTT[] = "user";
char passMQTT[] = "pass";

EthernetClient ethClient;
PubSubClient client(server,port,ethClient);

void setup()
{
  serialSetup();
  ethernetSetup();

  //LEDs vaga disponível e MQTT status
  pinMode(mqttStatusOnLED, OUTPUT);
  pinMode(mqttStatusOffLED, OUTPUT);
  
}

void loop()
{
  
  reconnectMQTT();
  
  showMqttStatusLED(client.connected());
 
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


void showMqttStatusLED (boolean mqttStatus){

  digitalWrite(mqttStatusOnLED, mqttStatus);
  digitalWrite(mqttStatusOffLED, !mqttStatus);
  
}


void reconnectMQTT() {
  
    char messageMQTT[] = "Online";
    if (!client.connected()) {
      
       Serial.print("Conectando MQTT ...");
    
//       if (client.connect(clientMQTTID,userMQTT,passMQTT,topicWill,0,false,msgQTTWill)) {
       if (client.connect(clientMQTTID,topicWill,0,false,msgQTTWill)) {
          Serial.println("conectado");    
       } else {
          Serial.print("falha, rc=");
          Serial.print(client.state());
          Serial.println(" nova tentativa em 5 segundos");        
       }
   }

}


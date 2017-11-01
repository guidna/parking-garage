#include <PubSubClient.h>
#include <Ethernet.h>
#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//=======================================================================
void callback(const char topic, byte* payload, unsigned int length);
//=======================================================================

int lcdPin = A0;
int lcdPinBg = A1;

String msgStart      = "Park-Garage ACG";
String msgNoVacancy  = "NAO HA VAGAS";
String msgVacancy    = "Vagas disp: ";
String msgCnxErro    = "System out";
char msgQTTWill[]    = "Client #03 off";

const int mqttStatusOnLED = 9;
const int mqttStatusOffLED = 8;

char topicPub[]  = "Garage_Light";
char topicSub[]  = "Garage_Light";
char topicWill[] = "Garage_Bye";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0x03 };
IPAddress ip (192, 162, 1, 3);
char server[] = "m10.cloudmqtt.com";
int port = 12598;

char clientMQTTID[] = "MQTT-Senai";
char userMQTT[] = "IoT-A";
char passMQTT[] = "S3n41-1o7";

EthernetClient ethClient;
PubSubClient client(server,port,callback,ethClient);

void setup()
{
  serialSetup();
  lcdSetup();
  ethernetSetup();

  //LEDs vaga disponível e MQTT status
  pinMode(mqttStatusOnLED, OUTPUT);
  pinMode(mqttStatusOffLED, OUTPUT);
  
}

void loop()
{
  reconnectMQTT();

  if(client.connected()) {
     client.loop();   
  }
   
  int qtdVacancy = getVacancy();

  showVacancy(qtdVacancy);

  if(qtdVacancy<=0) {
     lcdBlink(1000);  
  }
  
  showMqttStatusLED(client.connected());
 
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
}

void ethernetSetup() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha em configurar Ethernet usando DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("Ethernet Ok...");
}

void showVacancy(int vagas) {
  
  String msg;

  if(vagas > 0) {
    msg = msgVacancy;
  } else if (vagas == 0) {
    msg = msgNoVacancy;
  } else {
    msg = msgCnxErro;
  }

  lcd.setCursor(0,1);
  lcd.print(msg);

  if(vagas > 0) {
    lcd.setCursor(13, 1);
    lcd.print(vagas);   
  }
  delay(1000);
}

int getVacancy() {
  return 0;
}

void lcdBlink(int interval) {

  lcd.noDisplay();
  delay(interval);
  lcd.display();
  delay(interval);

}

void showMqttStatusLED (boolean mqttStatus){

  digitalWrite(mqttStatusOnLED, mqttStatus);
  digitalWrite(mqttStatusOffLED, !mqttStatus);
  
}


void reconnectMQTT() {
  
    char messageMQTT[] = "Online";
    if (!client.connected()) {
      
       Serial.print("Conectando MQTT ...");
    
       if (client.connect(clientMQTTID,userMQTT,passMQTT,topicWill,0,false,msgQTTWill)) {
          Serial.println("conectado");    
          client.publish(topicPub,messageMQTT);
          if (!client.subscribe(topicSub)) {
              Serial.println("Erro na subscrição");
          } else {
              Serial.println("Subscrição realizada com sucesso");
          }
       } else {
          Serial.print("falha, rc=");
          Serial.print(client.state());
          Serial.println(" nova tentativa em 5 segundos");        
       }
   }

}

void callback(const char topic, byte* payload, unsigned int length) {
  
  // handle message arrived
  Serial.print("Callback: ");
  Serial.println(String(topic));

  char* payloadAsChar = payload;

  // Workaround para pequeno bug na biblioteca
  payloadAsChar[length] = 0;

  // Converter em tipo String para conveniência
  String topicStr = String(topic);
  String msg = String(payloadAsChar);
  Serial.print("Topic received: "); Serial.println(String(topic));
  Serial.print("Message: "); Serial.println(msg);

  // Dentro do callback da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  // https://www.arduino.cc/en/Reference/StringToInt
  int msgComoNumero = msg.toInt();

  Serial.print("Numero recebido: "); Serial.println(msgComoNumero);
  Serial.flush();

}

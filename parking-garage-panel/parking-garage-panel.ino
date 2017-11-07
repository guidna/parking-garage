#include <PubSubClient.h>
#include <Ethernet.h>
#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//=======================================================================
void callback(char* topic, byte* payload, unsigned int length);
//=======================================================================

int lcdPin = A0;
int lcdPinBg = A1;

String msgStart      = "Park-Garage ACG";
String msgNoVacancy  = "NAO HA VAGAS     ";
String msgVacancy    = "Vagas disp: ";
String msgCnxErro    = "System out";
char msgQTTWill[]    = "Client Panel #03 off";

const int mqttStatusOnLED = 9;
const int mqttStatusOffLED = 8;

char topicSub[]  = "senai-code-xp/vagas/+";
char topicWill[] = "senai-code-xp/vagas/will";

//char topicSub[]  = "#";
//char topicWill[] = "Garage_Light";

int nVagas = 18;

int *lstVacancy;

int timeMsg;
int timeWithoutMsg;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xA0, 0x03 };
IPAddress ip (192, 162, 1, 3);
char server[] = "test.mosquitto.org";
int port = 1883;
//char server[] = "m10.cloudmqtt.com";
//int port = 12598;

char clientMQTTID[] = "MQTT-senai-sp-kit03";

//char userMQTT[] = "user";
//char passMQTT[] = "pass";
//char userMQTT[] = "IoT-A";
//char passMQTT[] = "S3n41-1o7";

EthernetClient ethClient;
PubSubClient client(server,port,callback,ethClient);

void setup()
{
  serialSetup();
  lcdSetup();
  ethernetSetup();
  setupVacancy();

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

  timeWithoutMsg = (millis() - timeMsg);
  
   
  int qtdVacancy = getVacancy();

  showVacancy(qtdVacancy);

  if(qtdVacancy<=0) {
     lcdBlink(1000);  
  }
  
  showMqttStatusLED(client.connected());

  if (timeWithoutMsg > 10000) {
     lcd.noDisplay();
     analogWrite(lcdPinBg, 0);
  } else {
     lcd.display();
     analogWrite(lcdPinBg, 1023);    
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
   analogWrite(lcdPinBg, 1023);
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

  int yesVacancy = 0;
  
  for ( int i = 0; i < nVagas; i++ ) {
      if (lstVacancy[ i ] == 1) {
        yesVacancy ++;
      }
  }
  
  return yesVacancy;
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
    
///       if (client.connect(clientMQTTID,userMQTT,passMQTT,topicWill,0,false,msgQTTWill)) {
       if (client.connect(clientMQTTID,topicWill,0,false,msgQTTWill)) {
          Serial.println("conectado");    
        
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

void callback(char* topic, byte* payload, unsigned int length) {
  
  char* payloadAsChar = payload;
  char* topicAsChar = topic;

  // Workaround para pequeno bug na biblioteca
  payloadAsChar[length] = 0;

  // Converter em tipo String para conveniência
  String topicStr = String(topicAsChar);
  String msg = String(payloadAsChar);
  String topicNumber = String(topicAsChar[20])+String(topicAsChar[21]);

  timeMsg = millis();  
  
  Serial.print("Topic received: "); Serial.print(topicStr); Serial.print(" - "); Serial.println(topicNumber); 
  Serial.print("Message: "); Serial.println(msg); 
  
  
  Serial.flush();

  lstVacancy[(topicNumber.toInt()-1)] = msg.toInt();
  
  Serial.flush();

}

void setupVacancy() {
  
  lstVacancy = (int *) malloc(nVagas * sizeof(int)); ;

  for ( int i = 0; i < nVagas; i++ ) {
      lstVacancy[ i ] = 1;
   }
}


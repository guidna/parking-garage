Projeto Smart Parking

Objetivo
Criar uma Vaga de estacionamento que detecta se há algum veículo presente
Criar uma Placa que indica a quantidade de vagas livres
FeedBack para o usuário
Os arduinos devem avisar de alguma forma quando o serviço estiver desconectado do broker MQTT
Os arduinos devem avisar de alguma forma quando o serviço estiver conectado ao broker MQTT
Os arduinos devem avisar de alguma forma quando o serviço estiver recebendo uma mensagem MQTT
Os arduinos devem avisar de alguma forma quando estiver mandando uma mensagem para o MQTT
A Vaga deve indicar se está ocupada ou livre localmente
Requisitos
As Vaga deve:
Conter um sensor para detectar se há um veiculo presente (ultrassom, sensor de luz, etc.)
Reportar via MQTT com um intervalo curto se estão Disponíveis ou Ocupadas
Todas as Vagas devem se conectar ao servidor MQTT público test.mosquitto.org (primeiros testes podem ser feitos via SerialPubSubClient ou com brokers individuais)
O tópico MQTT em que cada Vaga deve reportar seu estado deve se chamar senai-code-xp/vagas/{id-kit}
Utilizar a mensagem “1” para informar que está Disponível e mensagem “0” para informar que está Ocupada, esta mensagem deve ser armazenada pelo broker (Retain: true)
Caso perca conexão ou seja desligada abruptamente, a Vaga deve remover mensagens armazenadas em seu tópico de estado (Retain: true, Payload: ‘’)
A Placa deve:
Manter uma contagem interna das Vagas Disponíveis e Ocupadas
Exibir o número de Vagas livres visualmente (LCD, display de 7 segmentos)
Informar especificamente se não há vagas livres (LED vermelho, buzzer, mensagem de texto alternativa no LCD)
Após 10s sem receber nenhuma atualização, o display deve se apagar
Componentes
Visor LCD 16x2
Biblioteca: https://www.arduino.cc/en/Reference/LiquidCrystal
https://www.filipeflop.com/blog/controlando-um-lcd-16x2-com-arduino/
Sensor ultrasônico
Biblioteca: https://github.com/ErickSimoes/Ultrasonic
Sensor de luz
https://github.com/senai-sp/iot-projects/blob/master/aulas/04-primeiros-sensores.md
Módulo ethernet
https://www.filipeflop.com/blog/modulo-ethernet-enc28j60-arduino/
Servidor MQTT
https://www.cloudmqtt.com/ - Host online com plano gratuito
https://mosquitto.org/ - Software para broker MQTT
Conhecimentos necessários
Estruturas de controle/decisão e loop ex.: if, else, case, for...
https://www.arduino.cc/en/Reference/If
https://www.arduino.cc/en/Reference/Else
https://www.arduino.cc/en/Reference/SwitchCase	
https://www.arduino.cc/en/Reference/For	
Comunicação MQTT - http://mqtt.org/	
https://pubsubclient.knolleary.net/	
Criação de funções e variáveis
https://www.arduino.cc/en/Reference/Constants
Escrita analógica e digital
https://www.arduino.cc/en/Reference/PinMode
https://www.arduino.cc/en/Reference/DigitalWrite
https://www.arduino.cc/en/Reference/DigitalRead
https://www.arduino.cc/en/Reference/AnalogRead
https://www.arduino.cc/en/Reference/AnalogWrite
https://www.arduino.cc/en/Reference/AnalogReference	
Comunicação Serial
https://www.arduino.cc/en/Reference/Serial
Ferramentas de controle e organização
Git
Trello
Slack
Prazo
4 Dias
Quantidade de Pessoas
3

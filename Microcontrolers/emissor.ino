#include<Ethernet.h>
#include<EthernetUdp.h>

EthernetUDP Udp;
const int botao=4, fc0=2, fc1= 3;
byte mac[]={0x90,0xA2,0xDA,0x00,0x64,0x44};
byte ip[] = {192,168,1,1};
byte remoteip[] = {192,168,1,2};
char buff[3];
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
unsigned int port = 8888;

void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(port);
  Serial.begin(9600);
  pinMode (fc0, INPUT_PULLUP);
  pinMode (fc1, INPUT_PULLUP);
  pinMode (botao, INPUT_PULLUP);
}

void loop(){
  buff[0]=digitalRead(fc0);
  buff[1]=digitalRead(fc1);
  buff[2]=digitalRead(botao);
  Udp.beginPacket(remoteip, port);
  Udp.write(buff,3);
  Udp.endPacket();
  delay(100);
}

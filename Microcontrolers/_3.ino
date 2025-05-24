
// Comunicação
#include <SoftwareSerial.h>
SoftwareSerial sSerial(10, 11); 
int estado=-1;
uint8_t quadro = 0;
void setup()
{
  delay(1000);
  sSerial.begin(9600);
  Serial.begin(9600);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  delay(1000);
}

// Funções de manipulação das mensagens
const uint8_t TOKEN = 63; // 00111111
// lógica do loop
bool token = false; // true em um dos arduinos,
//falso nos demais
bool esperando = false; // arduino já enviou mensagem, e está
                 //esperando o retorno do quadro
unsigned long tempo_token = 0; // tempo de recebimento do token
const int endereco = 3; // endereço do próprio arduino
void loop()
{
  digitalWrite(13,token);
  if (sSerial.available()) // verifica se algo foi recebido
  {
    uint8_t quadro = sSerial.read();//lê quadro de mensagem
    if (quadro == TOKEN) // recebe token
    {
      token = true;
      esperando = false;
      tempo_token = millis(); // guarda o tempo em que o token foi recebido
      Serial.print("Recebeu token");
    }
    else if (extraiDest(quadro) == endereco) // mensagem para este arduino
    {
      quadro = ligaBitProc(quadro);
      sSerial.write(quadro); // repassa o quadro de mensagem com o bit modificado
    }
    else if (extraiRem(quadro) == endereco) // mensagem originada deste arduino
    {
      esperando = false; // finaliza espera pelo retorno do quadro enviado
      if (!verificaBitProc(quadro)) // verifica se mensagem foi processada
      {
        Serial.print("Errooowww no 3");
      }
    }
    else
    {
      sSerial.write(quadro); // repassa o quadro de mensagem sem modificações
    }
  }
  if (token && !esperando)
  {
    if (millis() - tempo_token < 50) //retém o token por 50ms
    {
      if(digitalRead(6)== 1)
       {
         uint8_t quadro = montaQuadro(3,1,2);
         Serial.print("cilindro recuado");
       }
      if(digitalRead(7)== 1)
       {
         uint8_t quadro = montaQuadro(3,1,4);
         Serial.print("cilindro avançado");
       }
      if(estado != quadro)
       {
        sSerial.write(quadro); // envia quadro de
        //mensagem
        esperando = true; // inicia espera pelo
        //retorno do quadro enviado
        estado = quadro;                                                                                                                                            
        Serial.print("Enviou mensagem");
       }
    }
    else
    {
      sSerial.write(TOKEN); // passa o token para o
      //próximo arduino
      token = false;
    }
  }
}
                                                  
uint8_t montaQuadro(uint8_t r, uint8_t d, uint8_t msg)
// monta o quadro, posicionando as informações no quadro de  mensagem através de operações de deslocamento de bits
{
  return ((r & 3) << 6) | ((d & 3) << 4) | (msg & 7);
}
uint8_t extraiRem(uint8_t quadro)
// extrai o remetente do quadro de mensagem
{
  return (quadro >> 6) & 3;
}
uint8_t extraiDest(uint8_t quadro)
// extrai o destinatário do quadro de mensagem
{
  return (quadro >> 4) & 3;
}
uint8_t extraiMsg(uint8_t quadro)
// extrai o corpo da mensagem do quadro de mensagem
{
  return quadro & 7;
}
uint8_t ligaBitProc(uint8_t quadro)
// liga o bit que sinaliza que a mensagem foi processada
{
  return quadro | 8; // 1000
}
bool verificaBitProc(uint8_t quadro)
// verifica que o bit que sinaliza que a mensagem está ligado
{
  return (quadro & 8) != 0;
}

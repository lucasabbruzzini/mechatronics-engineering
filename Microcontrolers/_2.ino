
// Comunicação
#include <SoftwareSerial.h>
SoftwareSerial sSerial(10, 11); 
int mensagem=-1;
void setup()
{
  delay(1000);
  sSerial.begin(9600);//comunicação em anel dos dispositivos
  Serial.begin(9600);// comunicação com o monitor serial
  delay(1000);
  pinMode(5,INPUT_PULLUP);
}

// Funções de manipulação das mensagens
const uint8_t TOKEN = 63; // 00111111
// lógica do loop
bool token = true; // true em um dos arduinos,
//falso nos demais
bool esperando = false; // arduino já enviou mensagem, e está
                 //esperando o retorno do quadro
unsigned long tempo_token = 0; // tempo de recebimento do token
const int endereco = 2; // endereço do próprio arduino

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
        Serial.print("Errooowww no 2");
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
      boolean mensagem = digitalRead(5);  // lê o botão
      Serial.println(mensagem);//visualiza no monitor serial o valor do botão
      // monta mensagem: IMPLEMENTAR
      if (mensagem) {
      uint8_t quadro = montaQuadro(2,1,1);
      //
      sSerial.write(quadro); // envia quadro de
      //mensagem
      esperando = true; }// inicia espera pelo
      //retorno do quadro enviado
    }
    else
    {
      sSerial.write(TOKEN); // passa o token para o
      //próximo arduino
      Serial.print("Enviou Token");
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

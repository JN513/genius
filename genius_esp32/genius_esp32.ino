#include <Arduino.h>

// Definições para utilizar a saida pwm do esp32

#define PWM1_Ch 0      // escolho o canal pwm 0
#define PWM1_Res 8     // seto a resolução do canal pwm como 8 bits 2⁸ = 256 no caso um intervalo de 0 a 255
#define PWM1_Freq 1000 // defino a frequencia da saida pwm como 1000hz

const int buzzer = 14; // gpio do buzzer

const int ledAmarelo = 15;  // gpio do led amarelo
const int ledAzul = 16;     // gpio do led azul
const int ledVerde = 17;    // gpio do led verde
const int ledVermelho = 18; // gpio do led vermelho

const int btnAmarelo = 19;  // gpio do botão do led amarelo
const int btnAzul = 21;     // gpio do botão do led azul
const int btnVerde = 22;    // gpio do botão do led verde
const int btnVermelho = 23; // gpio do botão do led vermelho

void alter_all_leds(int estado) // função para deixar todos os leds em um mesmo estado (ligados ou apagados)
{
  digitalWrite(ledAmarelo, estado); // defino o mesmo estado para todos os leds
  digitalWrite(ledAzul, estado);
  digitalWrite(ledVerde, estado);
  digitalWrite(ledVermelho, estado);
}

void setup()
{
  Serial.begin(115200); // Configuro a porta serial para trabalhar emum baudrate de 115200 bits

  pinMode(ledAmarelo, OUTPUT); // configuro a gpio do led amarelo como saida, faço isso para todos os outros leds
  pinMode(ledAzul, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  alter_all_leds(0); // apago todos os leds

  ledcAttachPin(buzzer, PWM1_Ch);          // Relaciono o canal pwm 0 ao pino do buzzer
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res); // configuro o canal 0 do pwm
  ledcWrite(PWM1_Ch, 0);                   // Defino como 0 o valor da saida do buzzer
}

void loop()
{
  int inicio = 0; // Variavel para controlar se o jogador iniciou o jogo

  while (inicio == 0) // enquanto inicio for igual a zero, mantenho os leds ligados e verifico se o botao verde não foi pressionado
  {
    inicio = digitalRead(btnVerde); // leio o valor do botão do led verde
    alter_all_leds(1);              // acendo todos os leds
    delay(1);
  }

  // inicia o jogo

  alter_all_leds(0); // apago todos os leds

  delay(1000); // espero um segundo
}

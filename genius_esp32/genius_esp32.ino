#include <Arduino.h>

// Definições para utilizar a saida pwm do esp32

#define PWM1_Ch 0      // escolho o canal pwm 0
#define PWM1_Res 10    // seto a resolução do canal pwm como 10 bits 2^10 = 1024 no caso um intervalo de 0 a 1023
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

    ledcAttachPin(buzzer, PWM1_Ch);  // Relaciono o canal pwm 0 ao pino do buzzer
    ledcSetup(PWM1_Ch, 0, PWM1_Res); // configuro o canal 0 do pwm
}

void loop()
{
    Serial.println("Bora Jogar?");
    Serial.println("Caso queira jogar aperte o botão verde.");
    int inicio = 0; // Variavel para controlar se o jogador iniciou o jogo

    while (inicio == 0) // enquanto inicio for igual a zero, mantenho os leds ligados e verifico se o botao verde não foi pressionado
    {
        inicio = digitalRead(btnVerde); // leio o valor do botão do led verde
        alter_all_leds(1);              // acendo todos os leds
        delay(1);
    }

    // inicia o jogo

    alter_all_leds(0); // apago todos os leds

    int pontos = 0;
    bool acertou = 1;

    int index = 0;
    int luzes[200]; // qual luz vai acender de acordo com cada index

    while (acertou)
    {
        Serial.print("Pontuação: ");
        Serial.println(pontos);

        luzes[index] = random(1, 5); // escolho um led aleatorio para acender

        index++;

        for (int i = 0; i < index; i++)
        {
            if (luzes[i] == 1) // 1: amarelo, 2: azul, 3: verde. 4: vermelho
            {
                digitalWrite(ledAmarelo, 1);
                ledcWriteTone(PWM1_Ch, 330); // arrumar essa função depois, se nao me engano nativamente ela só dura 1 ms, o ideal e 500
                delay(500);
                digitalWrite(ledAmarelo, 0);
                delay(200);
            }
            else if (luzes[i] == 2)
            {
                digitalWrite(ledAzul, 1);
                ledcWriteTone(PWM1_Ch, 349);
                delay(500);
                digitalWrite(ledAzul, 0);
                delay(200);
            }
            else if (luzes[i] == 3)
            {
                digitalWrite(ledVerde, 1);
                ledcWriteTone(PWM1_Ch, 262);
                delay(500);
                digitalWrite(ledVerde, 0);
                delay(200);
            }
            else if (luzes[i] == 4)
            {
                digitalWrite(ledVermelho, 1);
                ledcWriteTone(PWM1_Ch, 294);
                delay(500);
                digitalWrite(ledVermelho, 0);
                delay(200);
            }
        }

        bool terminou_sequencia = 0; // variavel para verificar se o jogador terminou a sequencia
        int aux = 0;                 // variavel de index para percorrer a sequencia de luzes

        while (terminou_sequencia && acertou) // verifica se ele ainda não terminou a sequencia e se ainda não errou
        {
            // rbxx = resultado botão <sigla da cor>
            int rbam = digitalRead(btnAmarelo);
            int rbaz = digitalRead(btnAzul);
            int rbvd = digitalRead(btnVerde);
            int rbvm = digitalRead(btnVermelho);

            if (rbam)
            {
                digitalWrite(ledAmarelo, 1);
                ledcWriteTone(PWM1_Ch, 330);
                if (luzes[aux] == 1)
                {
                    aux++;
                }
                else
                {
                    acertou = 0;
                }

                while (rbam)
                {
                    rbam = digitalRead(btnAmarelo);
                }

                delay(500);
                digitalWrite(ledAmarelo, 0);
                delay(500);
            }

            // Fazer para os outros leds

            if (aux >= index)
            {
                terminou_sequencia = 1;
                pontos++;
            }
        }

        if (index == 200)
        {
            break;
        }
    }

    ledcWriteTone(PWM1_Ch, 415); // O buzzer toca numa frequência diferente das luzes, ajustar essa função para 3000 ms depois

    alter_all_leds(0);

    if (acertou)
    {
        Serial.println("Parabens, você completou o jogo!!! :D");

        for (int i = 0; i < 3; i++) // Pisca 3 vezes o led verde se o jogador zerou o jogo, easter egg kkk
        {
            digitalWrite(ledVerde, 1);
            delay(500);
            digitalWrite(ledVerde, 0);
            delay(500);
        }
    }
    else
    {
        Serial.println("Puts, parece que você errou. :(");
    }

    Serial.print("Pontuação total: ");
    Serial.println(pontos);

    delay(1000); // espero um segundo
}

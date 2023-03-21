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

    int pontos = 0;   // pontuação
    bool acertou = 1; // variavel para terminar o loop quando o jogador errar

    int index = 0;  // index para controle do tamanho da sequencia, a cada rodada ele cresce e a sequencia aumenta
    int luzes[201]; // qual luz vai acender de acordo com cada index

    while (acertou) // enquanto o jogador não errar ou zerar a sequencia
    {
        Serial.print("Pontuação: "); // mostra a pontuação a cada rodada
        Serial.println(pontos);

        luzes[index] = random(1, 5); // escolho um led aleatorio para acender no novo indice da sequencia

        index++; // incrementa o index

        for (int i = 0; i < index; i++) // acendo todos os leds de acordo com a sequencia
        {
            if (luzes[i] == 1) // 1: amarelo, 2: azul, 3: verde. 4: vermelho
            {
                digitalWrite(ledAmarelo, 1); // acendo o led
                // toco um tom de acordo com a cor
                ledcWriteTone(PWM1_Ch, 330); // arrumar essa função depois, se nao me engano nativamente ela só dura 1 ms, o ideal e 500
                delay(500);                  // espero meio segundo
                digitalWrite(ledAmarelo, 0); // apago o led
                delay(200);                  // espero 200 ms
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
            int rbam = digitalRead(btnAmarelo); // verifico todos os botoes se algum foi apertado
            int rbaz = digitalRead(btnAzul);
            int rbvd = digitalRead(btnVerde);
            int rbvm = digitalRead(btnVermelho);

            if (rbam) // se o botao do amarelo foi apertado
            {
                digitalWrite(ledAmarelo, 1); // acendo o led amarelo
                ledcWriteTone(PWM1_Ch, 330); // toco o tom da cor amarela
                if (luzes[aux] == 1)         // verifico se o jogador acertou a cor
                {
                    aux++; // acrescento mais um no indice de acerto
                }
                else // caso ele tenha errado
                {
                    acertou = 0; // marco que ele errou
                }

                while (rbam) // enquanto ele continaur apertando o botão
                {
                    rbam = digitalRead(btnAmarelo);
                }

                delay(500);                  // espero meio segundo
                digitalWrite(ledAmarelo, 0); // apago o led
                delay(500);                  // espero mais meio segundo
            }

            // Fazer para os outros leds

            if (aux >= index) // se ele acertou toda sequencia o while não tera parado e o aux sera igual ao index que o tamanho da sequencia atual
            {
                terminou_sequencia = 1; // marco que ele completou a sequencia
                pontos++;               // adicono mais um ponto
            }
        }

        if (index == 200) // se o index for igual a 200 ele zerou a sequencia e o jogo
        {
            break;
        }
    }

    ledcWriteTone(PWM1_Ch, 415); // O buzzer toca numa frequência diferente das luzes, ajustar essa função para 3000 ms depois

    alter_all_leds(0); // apago todos os leds

    if (acertou) // verifica se ele zerou ou errou
    {
        Serial.println("Parabens, você completou o jogo!!! :D"); // Mensagem fofa caso o jogador tenha zerado o jogo

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

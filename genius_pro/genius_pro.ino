#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// definições do tamanho do display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Definições para utilizar a saida pwm do esp32

#define PWM1_Ch 1      // escolho o canal pwm 0
#define PWM1_Res 10    // seto a resolução do canal pwm como 10 bits 2^10 = 1024 no caso um intervalo de 0 a 1023
#define PWM1_Freq 1000 // defino a frequencia da saida pwm como 1000hz

const int buzzer = 14; // gpio do buzzer

const int ledAmarelo = 15;  // gpio do led amarelo
const int ledAzul = 16;     // gpio do led azul
const int ledVerde = 17;    // gpio do led verde
const int ledVermelho = 18; // gpio do led vermelho

#ifdef CONFIG_IDF_TARGET_ESP32S2

const int btnAmarelo = 1;  // gpio do botão do led amarelo
const int btnAzul = 2;     // gpio do botão do led azul
const int btnVerde = 3;    // gpio do botão do led verde; esp32: 22 eps32s2: 26
const int btnVermelho = 4; // gpio do botão do led vermelho; esp32: 23 esp32s2: 33
const int sda = 33;
const int scl = 34;

#elif CONFIG_IDF_TARGET_ESP32

const int btnAmarelo = 19;  // gpio do botão do led amarelo
const int btnAzul = 21;     // gpio do botão do led azul
const int btnVerde = 22;    // gpio do botão do led verde; esp32: 22 eps32s2: 26
const int btnVermelho = 23; // gpio do botão do led vermelho; esp32: 23 esp32s2: 33
const int sda = 25;
const int scl = 26;

#endif

#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void alter_all_leds(int estado) // função para deixar todos os leds em um mesmo estado (ligados ou apagados)
{
    digitalWrite(ledAmarelo, estado); // defino o mesmo estado para todos os leds
    digitalWrite(ledAzul, estado);
    digitalWrite(ledVerde, estado);
    digitalWrite(ledVermelho, estado);
}

void setup()
{
    pinMode(ledAmarelo, OUTPUT); // configuro a gpio do led amarelo como saida, faço isso para todos os outros leds
    pinMode(ledAzul, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledVermelho, OUTPUT);

    pinMode(btnAmarelo, INPUT); // configuro a gpio do botão do led amarelo como entrada, faço isso para todos os outros botões
    pinMode(btnAzul, INPUT);
    pinMode(btnVerde, INPUT);
    pinMode(btnVermelho, INPUT);

    alter_all_leds(0); // apago todos os leds

    Serial.begin(115200); // Configuro a porta serial para trabalhar em um baud rate de 115200 bits

    ledcAttachPin(buzzer, PWM1_Ch);     // Relaciono o canal pwm 0 ao pino do buzzer
    ledcSetup(PWM1_Ch, 1000, PWM1_Res); // configuro o canal 0 do pwm.
    randomSeed(analogRead(0));          // Garante que os numeros serão aleatorios quando o programa reiniciar

    Wire.begin(sda, scl);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    display.clearDisplay(); // Limpa o display
    display.display();      // mostra as coisas no display
    delay(2000);            // espera 2 segundos
}

void loop()
{
    display.clearDisplay();
    display.setTextSize(1.1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println("Bora Jogar?");
    display.setCursor(0, 20);
    display.println("Caso queira jogar \naperte o botao verde.");
    display.display();

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

    delay(2000);

    int pontos = 0;   // pontuação
    bool acertou = 1; // variavel para terminar o loop quando o jogador errar

    int index = 0;  // index para controle do tamanho da sequencia, a cada rodada ele cresce e a sequencia aumenta
    int luzes[201]; // qual luz vai acender de acordo com cada index

    int time_note = 1000;

    unsigned long timeout;

    while (acertou) // enquanto o jogador não errar ou zerar a sequencia
    {
        display.clearDisplay();
        display.setTextSize(1.1);
        display.setCursor(8, 3);
        display.println("Pontuacao: ");
        display.setTextSize(2);
        display.setCursor(40, 15);
        display.println(pontos);
        display.display();
        Serial.print("Pontuação: "); // mostra a pontuação a cada rodada
        Serial.println(pontos);

        luzes[index] = random(1, 5); // escolho um led aleatorio para acender no novo indice da sequencia

        index++; // incrementa o index

        for (int i = 0; i < index; i++) // acendo todos os leds de acordo com a sequencia
        {
            if (luzes[i] == 1) // 1: amarelo, 2: azul, 3: verde. 4: vermelho
            {
                // Serial.println("Amarelo");
                digitalWrite(ledAmarelo, 1); // acendo o led
                // toco um tom de acordo com a cor
                ledcWriteTone(PWM1_Ch, 330); // arrumar essa função depois, se nao me engano nativamente ela só dura 1 ms, o ideal e 500

                delay(time_note);            // espero meio segundo
                digitalWrite(ledAmarelo, 0); // apago o led
                ledcWrite(PWM1_Ch, 0);
                delay(200); // espero 200 ms
            }
            else if (luzes[i] == 2)
            {
                // Serial.println("Azul");
                digitalWrite(ledAzul, 1);
                ledcWriteTone(PWM1_Ch, 349);

                delay(time_note);
                digitalWrite(ledAzul, 0);
                ledcWrite(PWM1_Ch, 0);
                delay(200);
            }
            else if (luzes[i] == 3)
            {
                // Serial.println("Verde");
                digitalWrite(ledVerde, 1);
                ledcWriteTone(PWM1_Ch, 262);

                delay(time_note);
                digitalWrite(ledVerde, 0);
                ledcWrite(PWM1_Ch, 0);
                delay(200);
            }
            else if (luzes[i] == 4)
            {
                // Serial.println("Vermelho");
                digitalWrite(ledVermelho, 1);
                ledcWriteTone(PWM1_Ch, 294);

                delay(time_note);
                digitalWrite(ledVermelho, 0);
                ledcWrite(PWM1_Ch, 0);
                delay(200);
            }
        }

        bool terminou_sequencia = 0; // variavel para verificar se o jogador terminou a sequencia
        int aux = 0;                 // variavel de index para percorrer a sequencia de luzes

        int checkpoint = 10;

        timeout = millis();

        while (!terminou_sequencia && acertou) // verifica se ele ainda não terminou a sequencia e se ainda não errou
        {
            if (millis() - timeout > 5000)
            {
                acertou = 0;
                Serial.println("Tempo expirado");
                ledcWriteTone(PWM1_Ch, 390);

                display.clearDisplay();
                display.setTextSize(1.5);
                display.setTextColor(WHITE);
                display.setCursor(0, 10);
                // Display static text
                display.println("Tempo expirado. :(");
                display.display();
                delay(2000);
                ledcWrite(PWM1_Ch, 0);
            }
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

                delay(time_note / 2);        // espero meio segundo
                digitalWrite(ledAmarelo, 0); // apago o led
                ledcWrite(PWM1_Ch, 0);
                delay(200); // espero mais meio segundo
                timeout = millis();
            }
            if (rbaz) // se o botao do amarelo foi apertado
            {
                digitalWrite(ledAzul, 1);    // acendo o led amarelo
                ledcWriteTone(PWM1_Ch, 349); // toco o tom da cor amarela
                if (luzes[aux] == 2)         // verifico se o jogador acertou a cor
                {
                    aux++; // acrescento mais um no indice de acerto
                }
                else // caso ele tenha errado
                {
                    acertou = 0; // marco que ele errou
                }

                while (rbaz) // enquanto ele continaur apertando o botão
                {
                    rbaz = digitalRead(btnAzul);
                }

                delay(time_note / 2);     // espero meio segundo
                digitalWrite(ledAzul, 0); // apago o led
                ledcWrite(PWM1_Ch, 0);
                delay(200); // espero mais meio segundo
                timeout = millis();
            }
            if (rbvd) // se o botao do amarelo foi apertado
            {
                digitalWrite(ledVerde, 1);   // acendo o led amarelo
                ledcWriteTone(PWM1_Ch, 262); // toco o tom da cor amarela
                if (luzes[aux] == 3)         // verifico se o jogador acertou a cor
                {
                    aux++; // acrescento mais um no indice de acerto
                }
                else // caso ele tenha errado
                {
                    acertou = 0; // marco que ele errou
                }

                while (rbvd) // enquanto ele continaur apertando o botão
                {
                    rbvd = digitalRead(btnVerde);
                }

                delay(time_note / 2);      // espero meio segundo
                digitalWrite(ledVerde, 0); // apago o led
                ledcWrite(PWM1_Ch, 0);
                delay(200); // espero mais meio segundo
                timeout = millis();
            }
            if (rbvm) // se o botao do amarelo foi apertado
            {
                digitalWrite(ledVermelho, 1); // acendo o led amarelo
                ledcWriteTone(PWM1_Ch, 294);  // toco o tom da cor amarela
                if (luzes[aux] == 4)          // verifico se o jogador acertou a cor
                {
                    aux++; // acrescento mais um no indice de acerto
                }
                else // caso ele tenha errado
                {
                    acertou = 0; // marco que ele errou
                }

                while (rbvm) // enquanto ele continaur apertando o botão
                {
                    rbvm = digitalRead(btnVermelho);
                }

                delay(time_note / 2);         // espero meio segundo
                digitalWrite(ledVermelho, 0); // apago o led
                ledcWrite(PWM1_Ch, 0);
                delay(200); // espero mais meio segundo
                timeout = millis();
            }

            if (aux >= index) // se ele acertou toda sequencia o while não tera parado e o aux sera igual ao index que o tamanho da sequencia atual
            {
                terminou_sequencia = 1; // marco que ele completou a sequencia
                pontos++;               // adicono mais um ponto
            }
        }

        if (pontos - checkpoint == 5)
        {
            if (time_note <= 600)
            {
                time_note -= 5;
            }
            else
            {
                time_note -= 100;
            }

            checkpoint = pontos;
        }

        if (index == 200) // se o index for igual a 200 ele zerou a sequencia e o jogo
        {
            break;
        }

        delay(100);
    }

    ledcWriteTone(PWM1_Ch, 415); // O buzzer toca numa frequência diferente das luzes, ajustar essa função para 3000 ms depois
    delay(2000);
    ledcWrite(PWM1_Ch, 0);
    alter_all_leds(0); // apago todos os leds

    if (acertou) // verifica se ele zerou ou errou
    {
        display.clearDisplay();
        display.setTextSize(1.1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        // Display static text
        display.println("Parabens, você completou o jogo!!! :D");
        display.display();
        Serial.println("Parabens, você completou o jogo!!! :D"); // Mensagem fofa caso o jogador tenha zerado o jogo

        for (int i = 0; i < 3; i++) // Pisca 3 vezes o led verde se o jogador zerou o jogo, easter egg kkk
        {
            digitalWrite(ledVerde, 1);
            delay(500);
            digitalWrite(ledVerde, 0);
            delay(500);
        }

        delay(10000);
    }
    else
    {
        display.clearDisplay();
        display.setTextSize(1.1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        // Display static text
        display.println("Puts, parece que voce errou. : (");
        display.display();
        Serial.println("Puts, parece que você errou. :(");

        delay(5000);
    }

    display.clearDisplay();
    display.setTextSize(1.1);
    display.setCursor(8, 3);
    display.println("Pontuacao total: ");
    display.setTextSize(2);
    display.setCursor(40, 15);
    display.println(pontos);
    display.display();
    Serial.print("Pontuação total: ");
    Serial.println(pontos);

    delay(10000); // espero um segundo
}

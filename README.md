# Jogo Genius

![Imagem do Genius](https://estrela.vtexassets.com/arquivos/ids/163355/Jogo-Genius-Produto-Estrela.jpg?v=636661399595430000)

Quer testar se sua memória está afiada? Se sim, que tal jogar Genius?
Uma sugestão ainda melhor, bora fazer um Genius.

## Tópicos

- [Jogo Genius](#jogo-genius)
  - [Tópicos](#tópicos)
  - [O que é o Genius](#o-que-é-o-genius)
  - [O que vamos precisar](#o-que-vamos-precisar)
  - [Como funciona o dispositivo do Genius](#como-funciona-o-dispositivo-do-genius)
  - [Algoritmo](#algoritmo)
  - [Circuito](#circuito)
    - [Esquemático do Circuito:](#esquemático-do-circuito)
    - [Visão da protoboard do circuito](#visão-da-protoboard-do-circuito)
  - [Problemas e sugestões](#problemas-e-sugestões)

## O que é o Genius

Para quem não sabe ( até alguns dias antes de escrever esse artigo eu também não sabia o que era rs) Genius é um jogo de memória, onde um dispositivo acende um sequência aleatória de luzes e o jogador precisa após o dispositivo reproduzir a sequência, apertar sobre a sequência de luzes, na medida que o jogador acerta uma sequencia ele ganha um ponto e o jogo gera outra sequência maior, esse processo se repete até o jogador errar uma sequência ou zerar o jogo.

## O que vamos precisar

![Imagem do prototipo ](https://raw.githubusercontent.com/JN513/genius/main/imagens/imagem.jpeg)

Para construir o dispositivo do jogo precisaremos de:
- 1 ESP 32 ou 1 ESP32S2
- 1 buzzer
- 4 resistores de 220 ohms
- 4 resistores de 10k ohms
- 4 push buttons
- 1 led amarelo
- 1 led azul
- 1 led verde
- 1 led amarelo
-  jumpers
- protoboard

## Como funciona o dispositivo do Genius

O dispositivo do Genius precisa fazer dois tipos de procedimentos principais, o primeiro deles é sortear uma sequência aleatória de acionamento das luzes e  acionar as luzes de acordo com essa sequência, o segundo procedimento que ele precisa fazer e após acionar as luzes e verificar a ordem que o usuário acionou os botões e verificar se esta ordem é igual a ordem correta ou não, caso esse verificação seja verdadeira ele aumenta a pontuação do usuário, caso contrário encerra o jogo.

## Algoritmo

O algoritmo para o funcionamento do jogo consiste de várias partes, a primeira e definir quais portas [GPIO](https://pt.wikipedia.org/wiki/General_Purpose_Input/Output) do nosso microcontrolador sejá o ESP32 ou ESP32S2 iremos utilizar, para fazer isso na Linguagem C++ utilizada pela Arduino IDE, podemos salvar os numeros das portas em constantes e nomealas de acordo com o que está conectado a ela:

```cpp
const int buzzer = 14; // gpio do buzzer
const int ledAmarelo = 15;  // gpio do led amarelo
const int ledAzul = 16;     // gpio do led azul
const int ledVerde = 17;    // gpio do led verde
const int ledVermelho = 18; // gpio do led vermelho
const int btnAmarelo = 19; // gpio do botão do led amarelo
const int btnAzul = 21;    // gpio do botão do led azul

#ifdef CONFIG_IDF_TARGET_ESP32S2

const int btnVerde = 26;    // gpio do botão do led verde; esp32: 22 eps32s2: 26
const int btnVermelho = 33; // gpio do botão do led vermelho; esp32: 23 esp32s2: 33

#elif CONFIG_IDF_TARGET_ESP32

const int btnVerde = 22;    // gpio do botão do led verde; esp32: 22 eps32s2: 26
const int btnVermelho = 23; // gpio do botão do led vermelho; esp32: 23 esp32s2: 33

#endif

```

Como neste tutorial podemos usar dois microcontroladores diferentes algumas portas vão diferenciar entre eles, para evitar a utilização de dois códigos podemos incluir algumas condições que serão executadas durante a compilação que são feitas com base em definições feitas pela arduino IDE. Após a definição de quais pinos usaremos precisamos definir como cada um deles vai funcionar, se vai operar como entrada ou saída, para fazermos isso utilizamos a função ``` pinMode()```, dentro da nossa função void *setup* que é executada apenas uma vez após a inicialização do “sistema”.
```cpp
    pinMode(ledAmarelo, OUTPUT); // configuro a gpio do led amarelo como saída, faço isso para todos os outros leds
    pinMode(ledAzul, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledVermelho, OUTPUT);

    pinMode(btnAmarelo, INPUT); // configuro a gpio do botão do led amarelo como entrada, faço isso para todos os outros botões
    pinMode(btnAzul, INPUT);
    pinMode(btnVerde, INPUT);
    pinMode(btnVermelho, INPUT);

    alter_all_leds(0); // apago todos os leds

```

A função *alter_all_leds* É uma função personalizada que terá seu funcionamento mais explicado adiante. Como não vamos utilizar nenhum tipo de display e interessante que nosso microcontrolador nos envie informações via serial, para que isso ocorra devemos setar o iniciar a Serial e configurar o seu baudrate, realizamos isso utilizando o método begin do objeto Serial
```cpp
    Serial.begin(115200); // Configuro a porta serial para trabalhar em um baud rate de 115200 bits
```

… em breve continua …

## Circuito

### Esquemático do Circuito:

![Imagem do esquematico ](https://raw.githubusercontent.com/JN513/genius/main/esquematico/genius_esp32_Esquem%C3%A1tico.jpg)

### Visão da protoboard do circuito

![Imagem da protoboard ](https://raw.githubusercontent.com/JN513/genius/main/protoboard/genius_esp32_bb.jpg)


## Problemas e sugestões

Fique à vontade para dar sua sugestão ou relatar algum problema na sessão [Issues do Github](https://github.com/JN513/genius/issues), caso queira contribuir fique a vontade para criar um fork e realizar um Pull-Request.


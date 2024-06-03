// DS18B20 -> Sensor de temperatura
#include <OneWire.h> //Incluindo biblioteca
#include <DallasTemperature.h> //Incluindo biblioteca
#define DS18B20 2 //Pino do DS18B20
OneWire ourWire(DS18B20); // Configurando uma instância onewire para se comunicar com o sensor
DallasTemperature sensor(&ourWire); // Biblioteca DallasTemperature UTILIZA a OneWire

// LDR -> Sensor de luminosidade
int LDRPin = A0; // Pino do LDR
const float gama = 0.7; // Valor gama para cálculos
const float rl10 = 50;  // Valor da resistência de luz em lux (lux) a 10 lux

// HC-SR04 -> Sensor de distância ultrassônico
int HCTrig = 13; // Pino do HC
int HCEcho = 12; // Pino do HC
float profundidade; //Variável que armazenara o valor do HC

// POTENCIOMETRO -> Simulador de pH
int potenciometro = A1; // Pino do potenciômetro 
float valorPotenciometro = 0; //Variável que armazenara o valor do potenciometro

// PIR -> Sensor de movimento
const int movimento = 7; // Pino do PIR

void setup(){
  Serial.begin(9600); // Inicia a serial

  sensor.begin(); // Inicia o sensor DS18B20

  pinMode(LDRPin, INPUT); // LDR - porta de entrada 

  pinMode(potenciometro, INPUT); // Potenciometro - porta de entrada

  pinMode(HCTrig, OUTPUT); // HCTring - porta de saida

  pinMode(HCEcho, INPUT); // HCEcho - porta de entrada

  pinMode(movimento, INPUT); // PIR - porta de entrada

  delay(1000); // Intervalo de 1 segundo
}

void loop(){
  /* 
  +-----------------------+
  | TEMPERATURA (DS18B20) |
  +-----------------------+
  */
  sensor.requestTemperatures(); // Informa a temperatura do sensor
  int temperaturaAgua = sensor.getTempCByIndex(0); // Armazena a temperatura na variável

  // Mostra a temperatura
  Serial.print("Temperatura: "); 
  Serial.print(sensor.getTempCByIndex(0)); // Imprime na serial o valor de temperatura medido
  Serial.println("ºC"); 

  // TEMPERATURA IDEAL DA ÁGUA
  Serial.print("Temperatura da água: ");
  if (temperaturaAgua < 5) // Se a temperatura da água esteja abaixo de 5ºC
  {
    Serial.println("BAIXA!");
    delay(250); 
  }
  else if (temperaturaAgua < 30) // Se a temperatura da água esteja abaixo de 30ºC
  {
    Serial.println("OK!"); =
    delay(250); 
  }
  else // Se a temperatura da água esteja acima de 30ºC
  {
    Serial.println("ALTA!"); 
    delay(250); 
  } 

  // TEMPERATURA IDEAL DA ÁGUA PARA OS CORAIS
  Serial.print("Temperatura da água para os CORAIS: ");
  if (temperaturaAgua < 23)  // Se a temperatura da água esteja abaixo de 23ºC
  {
    Serial.println("BAIXA!"); 
    delay(250); 
  }
  else if (temperaturaAgua < 26) // Se a temperatura da água esteja abaixo de 26ºC
  {
    Serial.println("OK!"); 
    delay(250); 
  }
  else // Se a temperatura da água esteja acima de 26ºC
  {
    Serial.println("ALTA!"); 
    delay(250); 
  } 
  

  /* 
  +--------------+
  | PROFUNDIDADE |
  +--------------+
  */
  digitalWrite(HCTrig, LOW); // HCTring - pino LOW
  delay(0005); // Tempo de 5 milissegundos
  digitalWrite(HCTrig, HIGH); // HCTring - pino HIGH
  delay(0010); // Tempo de 10 milissegundos
  digitalWrite(HCTrig, LOW); // HCTring - pino LOW

  profundidade = pulseIn(HCEcho,HIGH); // Mede a duração do pulso de retorno do sensor quando ele detecta o eco do sinal enviado.
  // O valor retornado é o tempo em microssegundos que leva para o pulso ser detectado
  profundidade = profundidade/58; // Converte o tempo medido em microssegundos para a distância em centímetros

  // Como a unidade de medida do HC é em cm e o máximo é 400 = 4m. Consideraremos que 1cm = 1m 

  // Mostra a profundidade em metros
  Serial.print("Profundidade: ");
  Serial.print(profundidade);
  Serial.println(" m");


  /* 
  +--------------------+
  | LUMINOSIDADE (LDR) |
  +--------------------+
  */
 
  int LDR = analogRead(LDRPin); // Lê o LDR
  float tensao = LDR / 1024.0 * 5;  // Calcula a tensão com base na leitura do LDR
  float resistencia = 2000 * tensao / (1 - tensao / 5);  // Calcula a resistência do LDR
  float LDRBrilho = pow(rl10 * 1e3 * pow(10, gama) / resistencia, (1 / gama));  // Calcula a luminosidade em lux

  // Mostra o valor da luminosidade em lux
  Serial.print("Luminosidade(lux): ");
  Serial.println(LDRBrilho); 

  // CORAIS DE ÁGUAS RASAS
  if (profundidade < 30) // Se a profundidade for menor que 30 metros 
  {
    Serial.print("Corais de águas RASAS: ");  
    if (LDRBrilho < 2000) // Se o brilho for menor que 2000 lux
    {
      Serial.println("luminosidade BAIXA!");
      delay(250); 
    }
    else if (LDRBrilho < 5500) // Se o brilho for menor que 5500 lux
    {
      Serial.println("luminosidade OK!");
      delay(250);
    }
    else // Se o brilho for maior que 5500 lux
    {
      Serial.println("luminosidade ALTA!");
      delay(250); 
    }
  }
  // CORAIS DE ÁGUAS PROFUNDAS
  else // Se a profundidade for maior que 30 metros 
  {
    Serial.print("Corais de águas PROFUNDAS: ");
    if (LDRBrilho < 5500) // Se o brilho for menor que 5500 lux
    {
      Serial.println("luminosidade BAIXA!"); 
      delay(250); 
    }
    else if (LDRBrilho < 13200) // Se o brilho for menor que 13200 lux
    {
      Serial.println("luminosidade OK!");
      delay(250); 
    }
    else // Se o brilho for maior que 13200 lux
    {
      Serial.println("luminosidade ALTA!");
      delay(250); 
    }
  }


  /* 
  +------------------------------+
  | POTENCIOMETRO (Leitor de pH) |
  +------------------------------+
  */

  valorPotenciometro = map(analogRead(potenciometro),0,1023,0,14); // Lê o valor do potenciômetro e o mapeia de um intervalo de 0 a 1023 para um novo intervalo de 0 a 14

  // Mostra o pH
  Serial.print("pH: ");
  Serial.print(valorPotenciometro);

  if (valorPotenciometro < 7) // Se o valor do potenciômetro for menor que 7
  {
    Serial.println(" (ácido)");
  }
  else if (valorPotenciometro == 7) // Se o valor do potenciômetro for igual 7
  {
    Serial.println(" (neutro)");
  }
  else // Se o valor do potenciômetro for maior que 7
  {
    Serial.println(" (básico)");
  }


  /* 
  +-----------------+
  | MOVIMENTO (PIN) |
  +-----------------+
  */

  byte movimento = digitalRead(movimento); // Lê o pino da variável movimento e transforma em tipo byte.
  // O estado pode ser HIGH(1) ou LOW(0)
  if (movimento) // Se for HIGH (1)
  {
    Serial.println("Movimento captado!");
  }
  else
  {
    Serial.println("Nenhum movimento captado!");
  }
  
  Serial.println(" ");
}

// DS18B20 -> Sensor de temperatura
#include <OneWire.h>
#include <DallasTemperature.h>
#define DS18B20 2 // Pino digital utilizado pelo sensor DS18B20
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
  if (temperaturaAgua < 5)
  {
    Serial.println("BAIXA!");
    delay(250); 
  }
  else if (temperaturaAgua < 30)
  {
    Serial.println("OK!");
    delay(250); 
  }
  else
  {
    Serial.println("ALTA!");
    delay(250); 
  } 

  // TEMPERATURA IDEAL DA ÁGUA PARA OS CORAIS
  Serial.print("Temperatura da água para os CORAIS: ");
  if (temperaturaAgua < 23)
  {
    Serial.println("BAIXA!");
    delay(250); 
  }
  else if (temperaturaAgua < 26)
  {
    Serial.println("OK!");
    delay(250); 
  }
  else
  {
    Serial.println("ALTA!");
    delay(250); 
  } 
  

  /* 
  +--------------+
  | PROFUNDIDADE |
  +--------------+
  */
  digitalWrite(HCTrig, LOW);
  delay(0005);
  digitalWrite(HCTrig, HIGH);
  delay(0010);
  digitalWrite(HCTrig, LOW);

  profundidade = pulseIn(HCEcho,HIGH);
  profundidade = profundidade/58;

  // Como a unidade de medida do HC é em cm e o máximo é 400 = 4m. Consideraremos que 1cm = 1m 

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
  if (profundidade < 30)
  {
    Serial.print("Corais de águas RASAS: ");
    if (LDRBrilho < 2000)
    {
      Serial.println("luminosidade BAIXA!");
      delay(250); 
    }
    else if (LDRBrilho < 5500)
    {
      Serial.println("luminosidade OK!");
      delay(250);
    }
    else
    {
      Serial.println("luminosidade ALTA!");
      delay(250); 
    }
  }
  // CORAIS DE ÁGUAS PROFUNDAS
  else
  {
    Serial.print("Corais de águas PROFUNDAS: ");
    if (LDRBrilho < 5500)
    {
      Serial.println("luminosidade BAIXA!");
      delay(250); 
    }
    else if (LDRBrilho < 13200)
    {
      Serial.println("luminosidade OK!");
      delay(250); 
    }
    else
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

  valorPotenciometro = map(analogRead(potenciometro),0,1023,0,14);
  Serial.print("pH: ");
  Serial.print(valorPotenciometro);
  if (valorPotenciometro < 7)
  {
    Serial.println(" (ácido)");
  }
  else if (valorPotenciometro == 7)
  {
    Serial.println(" (neutro)");
  }
  else
  {
    Serial.println(" (básico)");
  }

  /* 
  +-----------------+
  | MOVIMENTO (PIN) |
  +-----------------+
  */

  byte movimento = digitalRead(movimento);
  if (movimento){
    Serial.println("Movimento captado!");
  }
  else{
    Serial.println("Nenhum movimento captado!");
  }

  Serial.println(" ");
}
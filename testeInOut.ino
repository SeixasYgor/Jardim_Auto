#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Adafruit_Sensor.h>  
#include <DHT.h>
#include <DHT_U.h>

const int ledInformativo = 13;
const int inBtEnter = 9;
const int inBtVoltar = 10;
const int inBtMais = 11;
const int inBtMenos = 12;
const int outReleLampada01;
const int outReleLampada02;
const int outReleBomba01;
const int outReleBomba02;
const int outReleBomba03;
const int inLDR = A3;

// selecione um sensor, retirando o comentário - duas barras
#define DHTTYPE    DHT11                           // Sensor DHT11
//#define DHTTYPE      DHT22                       // Sensor DHT22 ou AM2302

#define DHTPIN 2                                   // Pino do Arduino conectado no Sensor(Data) 
DHT_Unified dht(DHTPIN, DHTTYPE);                  // configurando o Sensor DHT - pino e tipo
uint32_t delayMS;                                  // variável para atraso no tempo

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

float tempAtual = 0;
float umidAtual = 0;
int   ldrAtual = 0;
bool  btEnter = false;
bool  btVoltar = false;
bool  btMais = false;
bool  btMenos = false;

int Menu;

int aux1;
int aux2;
String auxString;

void setup() {
  //Inicialização do LCD I2C
  lcd.init();

  //Inicialização para Usuario
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Ola!!!");
  lcd.setCursor(0,1);
  lcd.print("Iniciando");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  
  //Definindo Portas i/o
  pinMode(ledInformativo, OUTPUT);
  pinMode(inBtEnter, INPUT_PULLUP); // Configura pino 13 como entrada e habilita pull up interno;
  pinMode(inBtVoltar, INPUT_PULLUP);
  pinMode(inBtMais, INPUT_PULLUP);
  pinMode(inBtMenos, INPUT_PULLUP);
  pinMode(inLDR, INPUT);
  

  //Inicialização da comunicação serial e DHT
  Serial.begin(9600);
  dht.begin();     

  
  lcd.clear();
  lcd.setCursor(0,0);
  //lcd.print("Groow Autonomo!!");
  delay(3000);

  Serial.println("Usando o Sensor DHT");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);           // imprime os detalhes do Sensor de Temperatura
  Serial.println("------------------------------------");
  Serial.println("Temperatura");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");
  dht.humidity().getSensor(&sensor);            // imprime os detalhes do Sensor de Umidade
  Serial.println("------------------------------------");
  Serial.println("Umidade");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;            // define o atraso entre as leituras



  
  lcd.setBacklight(LOW);
  delay(5000);
  lcd.clear();
  lcd.setBacklight(HIGH);
}

void verificaEntradas () {
  ldrAtual = analogRead(inLDR);
  btEnter = digitalRead(inBtEnter);
  btVoltar = digitalRead(inBtVoltar);
  btMais = digitalRead(inBtMais);
  btMenos = digitalRead(inBtMenos);



  //Area para testar variaveis
  if (digitalRead(inBtEnter) == LOW) { // Botão Pressionado;
    Serial.println("Botao Enter Pressionado");
    digitalWrite(ledInformativo, HIGH);
   //TESTAR AS DUAS LAMPADAS AQUI
  } else {
    //Serial.println("Botões Não Pressionado");
    digitalWrite(ledInformativo, LOW);
  }

  if (digitalRead(inBtVoltar) == LOW) { // Botão Pressionado;
    Serial.println("Botao Enter Pressionado");
    digitalWrite(ledInformativo, HIGH);
    //TESTAR BOMBA 01 AQUI
   
  } else {
    //Serial.println("Botões Não Pressionado");
    digitalWrite(ledInformativo, LOW);
  }
  if (digitalRead(inBtMais) == LOW) { // Botão Pressionado;
    Serial.println("Botao Enter Pressionado");
    digitalWrite(ledInformativo, HIGH);
   //TESTAR BOMBA 02 AQUI
  } else {
    //Serial.println("Botões Não Pressionado");
    digitalWrite(ledInformativo, LOW);
  }
    if (digitalRead(inBtMenos) == LOW) { // Botão Pressionado;
    Serial.println("Botao Enter Pressionado");
    digitalWrite(ledInformativo, HIGH);
    //TESTAR BOMBA 03 OU VENTONINHA(IMPORTANTE!!) AQUI
   
  } else {
    //Serial.println("Botões Não Pressionado");
    digitalWrite(ledInformativo, LOW);
  }
  
  
  if(ldrAtual > 200){ //SE O VALOR LIDO FOR MAIOR QUE 600, FAZ
    Serial.print("Valor LDR: ");
    Serial.println(ldrAtual);
    Serial.println("Lampada Desligada!");
    
  }  
  else{ //SENÃO, FAZ
    Serial.print("Valor LDR: ");
    Serial.println(ldrAtual);
    Serial.println("Lampada Ligada!");  
    
  }  
  //FALTA IMPRIMIR AS LEITURAS DO HIGROMETRO<<<<<<<<<<<<<<-----------------------------------
}


void verificaUmidTemp () {
  sensors_event_t event;                        // inicializa o evento da Temperatura
  dht.temperature().getEvent(&event);           // faz a leitura da Temperatura
  if (isnan(event.temperature))                 // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Temperatura!");
  }
  else                                          // senão
  {
    Serial.print("Temperatura: ");              // imprime a Temperatura
    Serial.print(event.temperature);
    Serial.println(" *C");
    tempAtual = event.temperature;   
  }
  dht.humidity().getEvent(&event);              // faz a leitura de umidade
  if (isnan(event.relative_humidity))           // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Umidade!");
  }
  else                                          // senão
  {
    Serial.print("Umidade: ");                  // imprime a Umidade
    Serial.print(event.relative_humidity);
    Serial.println("%");
    umidAtual = event.relative_humidity;
  }
}


void exibeLCD () {
 
  lcd.setCursor(0,0);
  lcd.print("TesTE DE FUNCIOnamneto");
  lcd.print(": ");
  
  
  lcd.setCursor(0,1);
  lcd.print("teste de funciONAMENnetneos...");
}

void loop() {
  verificaEntradas ();
  verificaUmidTemp ();
    exibeLCD ();
  

  Serial.println();
  delay(1500);
}
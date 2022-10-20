#include <BluetoothSerial.h>
#define LED1_GPIO 4 //pin 4 Led grande, pin 33 led rojo, encendido con LOW, apaga con HIGH
#define boton 14

BluetoothSerial SerialBT;
void ConfigByBluetooth(void);

int cont=0;
int bot;

//bool BTHControl;

void setup() {
  Serial.begin(115200);
  Serial.println("Inicio de codigo");
  pinMode(bot, INPUT);
  pinMode(LED1_GPIO, OUTPUT);
}

void loop() {
    ConfigByBluetooth();
}

void ConfigByBluetooth(void){
  //Inicializamos bluetooth
  String message = "";
  String message2 = "";
  char incomingChar; 
  char incomingChar2;
  bool BTHControl;
  SerialBT.begin("ESP32BTDavid");
  //Serial.println("Bluetooth disponible");

  //config boton
  bot=digitalRead(boton);
  Serial.println(bot);
   delay(200);
   if(bot==0)
  {
    cont=cont+1;
  }
  if(cont>1){
    cont=0;
  }
  
  if(cont==1){
    BTHControl = true;
  //while(BTHControl == true){
  
    if(SerialBT.available()){
      char incomingChar = SerialBT.read();
      if(incomingChar != '\n'){
        message += String(incomingChar);
      }
      else {
        message = "";
      }
      Serial.write(incomingChar);
    }
  if(message =="Reset"){
    BTHControl = false; 
    SerialBT.end();
  }
  if(message=="LED ON"){
    digitalWrite(LED1_GPIO,HIGH);
  }
  else if(message=="LED OFF"){
    digitalWrite(LED1_GPIO,LOW);
  }
  if(Serial.available()){
    char incomingChar2 = Serial.read();
    if(incomingChar2 != '\n'){
      message2 += String(incomingChar2);
    }
    else {
      message2 ="";
    }
    SerialBT.write(incomingChar2);
  }
 }
 if(cont==0){
    BTHControl = false;
    SerialBT.end();
    Serial.println("Bluetooth disponible");
 }
}

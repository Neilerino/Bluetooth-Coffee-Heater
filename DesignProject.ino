#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal LCD(10,9,7,6,5,4);

int LED = 4;
int recvChar;

#define RxD         3
#define TxD         2
#define ONE_WIRE_BUS 11
SoftwareSerial BTSerial(RxD,TxD);
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

int increaseTemp = 13;
int decreaseTemp = 12;
int increaseTempState = LOW;
int decreaseTempState = LOW;

double desiredTemp = 40; //Degrees Celcius
int heat = 11;
double temp = 0;

void setup() {
  
  pinMode(increaseTemp,INPUT);
  pinMode(decreaseTemp,INPUT);
  pinMode(13,INPUT);

  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  pinMode(LED, OUTPUT);
  setupBlueToothConnection();
  pinMode(8,OUTPUT);
  
  LCD.begin(16,2); 
  LCD.setCursor(0,0);
  sensors.begin();
}

void loop() {
  
  sensors.requestTemperatures();

  temp = sensors.getTempCByIndex(0);

  updateLCD();
    
  if (desiredTemp >= temp) {
    digitalWrite(8,HIGH);
  } else {
    digitalWrite(8,LOW);
  }
  
  increaseTempState = digitalRead(increaseTemp);
  decreaseTempState = digitalRead(decreaseTemp);

  if (increaseTempState == LOW) {
    desiredTemp++;
  }
  if (decreaseTempState == LOW) {
    desiredTemp--;
  }
  
  BTSerial.println(temp);
  
  updateLCD();
}

void setupBlueToothConnection()
{  
  BTSerial.begin(9600);  
  
  BTSerial.print("AT");
  delay(400); 

  BTSerial.print("AT+DEFAULT");             // Restore all setup value to factory setup
  delay(2000); 
  
  BTSerial.print("AT+NAMESmartCoaster");    // set the bluetooth name
  
  BTSerial.print("AT+PIN0000");             // set the pair code to connect 
  delay(400);
  
  BTSerial.print("AT+AUTH1");             //
    delay(400);    

  BTSerial.flush();

}

void updateLCD() {
    LCD.setCursor(0,0);
    LCD.print("CurrentTemp:");
    LCD.println(temp);
    LCD.setCursor(0,1);
    LCD.print("WantedTemp:");
    LCD.println(desiredTemp);
    delay(100);
}

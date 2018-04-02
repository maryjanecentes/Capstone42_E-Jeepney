#include <Wire.h>
#include <LiquidCrystal.h>


//declarations

LiquidCrystal lcd(
12,11,6,5,4,3);

volatile int coins = 0;
const int coinslotPin = 2;
const int coinHopperPin = 7;
const int relayPin1 = 8;//coin hopper
const int relayPin2 = 9;//lock
const int sevenSegmentPin = 10;
const int greenLedPinPassenger = 14;
const int redLedPinPassenger = 15;
const int greenLedPinDriver = 16;
const int redLedPinDriver = 17;

volatile int totalInsertedCoins = 0;
int previousCoins = 0;
volatile int balance = 0;
volatile int fare = 0;
volatile int permFare;
volatile int change = 0;
int coinHopperPinState = 0;
int flag = 0;
int lockOpen = 0;

//setting up all input/output pins
void setup()
{
  pinMode(coinslotPin, INPUT);
  pinMode(coinHopperPin, INPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(sevenSegmentPin, OUTPUT);
  pinMode(greenLedPinPassenger, OUTPUT);
  pinMode(redLedPinPassenger, OUTPUT);
  pinMode(greenLedPinDriver, OUTPUT);
  pinMode(redLedPinDriver, OUTPUT);


  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  digitalWrite(greenLedPinPassenger,LOW);
  digitalWrite(sevenSegmentPin, HIGH);


  //start of the recieving event
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  
  pinMode(coinslotPin,INPUT_PULLUP);
  attachInterrupt(0, coinsInserted, FALLING);
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" PRESS TYPE OF");
  lcd.setCursor(0,1);
  lcd.print("   PASSENGER");
  
  Serial.begin(9600);
}
void coinsInserted()//method in calculating the change and the coins inserted
{
  totalInsertedCoins = totalInsertedCoins + 1;
  fare = fare - 1;
  change = change + 1;
  lcd.setCursor(0,1);
  lcd.print("Change: ");
  lcd.print(change);
  lcd.print(".00 Php");

}

void coinHopper(int fareParam,int totalInsertedCoinsParam)//method in dispensing coins
{
  balance = fareParam - totalInsertedCoinsParam;

  int prev = 0;

  while(fare < 0)
  {
    coinHopperPinState = digitalRead(coinHopperPin);

    digitalWrite(relayPin1, LOW);
    if(coinHopperPinState == HIGH && coinHopperPinState != prev)
    {
      fare = fare + 1;

    }
    prev = coinHopperPinState;
  }
  digitalWrite(relayPin1, HIGH);
}
void openLock()//method for opening the lock
{
  lcd.clear();
  lcd.print("LOCK IS OPEN");
  lcd.setCursor(0,1);
  lcd.print("PULL THE BARRIER");
  digitalWrite(relayPin2, LOW);
  delay(6000);
  digitalWrite(relayPin2, HIGH);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" PRESS TYPE OF");
  lcd.setCursor(0,1);
  lcd.print("   PASSENGER");
}

void loop()//the infinite loop in order for the process to be continuous
{
  if(lockOpen){
    lockOpen = 0;
    openLock();
  }
  previousCoins = totalInsertedCoins;
  delay(500);
  if(previousCoins == totalInsertedCoins && totalInsertedCoins != 0)
  { 
    if(fare !=0 && fare <= totalInsertedCoins && flag)
    {
      coinHopper(fare,totalInsertedCoins);
    }
    if(fare == 0 && flag){

      digitalWrite(greenLedPinPassenger,HIGH);
      digitalWrite(redLedPinPassenger,LOW);
      
      openLock();
      digitalWrite(sevenSegmentPin,LOW);
      digitalWrite(sevenSegmentPin,HIGH);
      digitalWrite(greenLedPinPassenger,LOW);
      digitalWrite(greenLedPinDriver, LOW);
      digitalWrite(redLedPinDriver, LOW);
      fare = 0;
      totalInsertedCoins = 0;
      flag=0;
      lcd.clear();
      lcd.print(" PRESS TYPE OF");
      lcd.setCursor(0,1);
      lcd.print("   PASSENGER");
    }
  }
}

void receiveEvent(int howMany)//method in recieving the signals from arduino 1
{
  int data;

  lcd.clear();
  while(1 < Wire.available())
  {
    int data = Wire.read();

  }
  data = Wire.read();
  Serial.println(data);
  if(data == 1)
  {
    digitalWrite(redLedPinPassenger, HIGH);
    digitalWrite(redLedPinDriver, LOW);
    digitalWrite(greenLedPinDriver, LOW);

    fare = 6;
    permFare = 6;
    flag =1;
    change = -6;

    lcd.clear();
    lcd.print("Fare: ");
    lcd.print(permFare);
    lcd.print(".00 Php");

  }
  else if (data == 2)
  {
    digitalWrite(redLedPinPassenger, HIGH);
    digitalWrite(redLedPinDriver, LOW);
    digitalWrite(greenLedPinDriver, LOW);
    permFare = 6;
    fare = 6;
    flag =1;
    change = -6;

    lcd.clear();
    lcd.print("Fare: ");
    lcd.print(permFare);
    lcd.print(".00 Php");
  }
  else if (data == 3)
  {
    digitalWrite(redLedPinPassenger, HIGH);
    digitalWrite(redLedPinDriver, LOW);
    digitalWrite(greenLedPinDriver, LOW);

    permFare = 7;
    fare = 7;
    flag = 1;
    lcd.clear();
    lcd.print("Fare:");
    lcd.print(permFare);
    lcd.print(".00 Php");
    change = -7;
  }
  else if (data == 4)
  {
    lockOpen = 1;
  }
  else if (data == 5)
  {
    lcd.print(" PRESS TYPE OF");
    lcd.setCursor(0,1);
    lcd.print("   PASSENGER");

  }
  else if (data == 6)
  {
    digitalWrite(greenLedPinDriver,HIGH);
    digitalWrite(redLedPinDriver,HIGH);
    lcd.clear();
    lcd.print("Waiting for ");
    lcd.setCursor(0,1);
    lcd.print("confirmation ...");


  }
  else if (data == 7)
  {
    digitalWrite(greenLedPinDriver, LOW);
    digitalWrite(redLedPinDriver, LOW);
    digitalWrite(greenLedPinPassenger, LOW);
    digitalWrite(redLedPinPassenger, LOW);
    lcd.clear();
    lcd.print("Waiting to");
    lcd.setCursor(0,1);
    lcd.print("Open ...");
  }
  else if (data == 8)
  {
  //null
  }
}








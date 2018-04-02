#include <Wire.h>

//declaration of constant variables

const int studentButton = 3;
const int seniorpwdButton = 4;
const int regularButton = 5;
const int nocoinsButton = 6;
const int yesButton = 7;
const int noButton = 8;
const int openButton = 9;
const int clearButton = 10;
const int buzzerPin = 11;

const int studentFare = 1;
const int seniorpwdFare = 2;
const int regularFare = 3;
const int openLock = 4;
const int delayValue = 5;
const int confirmation = 6;
const int requestOpen = 7;
const int clearCounter = 8;

//declaration of non constant variables

int studentButtonState = 0;
int seniorpwdButtonState = 0;
int regularButtonState = 0;
int nocoinsButtonState = 0;
int yesButtonState = 0;
int noButtonState = 0;
int openButtonState = 0;
int clearButtonState = 0;
int lastButtonState = 0;
int tempFare = 0;
int counter = 0;

//setting up all input/output pins
void setup()
{

  pinMode(studentButton, INPUT);
  pinMode(seniorpwdButton, INPUT);
  pinMode(regularButton, INPUT);
  pinMode(nocoinsButton, INPUT);
  pinMode(yesButton, INPUT);
  pinMode(noButton, INPUT);
  pinMode(openButton, INPUT);
  pinMode(clearButton, INPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(buzzerPin,HIGH);

  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  //reading of inputs
  
  studentButtonState = digitalRead(studentButton);
  seniorpwdButtonState = digitalRead(seniorpwdButton);
  regularButtonState = digitalRead(regularButton);

  nocoinsButtonState = digitalRead(nocoinsButton);  
  openButtonState = digitalRead(openButton);
  clearButtonState = digitalRead(clearButton);


  if(studentButtonState != lastButtonState)//if student button is pressed
  {
    if(studentButtonState == HIGH)
    {
      //turning on the buzzer for 1 sec
      digitalWrite(buzzerPin, LOW);
      delay(1000);
      digitalWrite(buzzerPin,HIGH);

      Wire.beginTransmission(8);
      Wire.write(confirmation);
      Wire.endTransmission();

      while(counter <= 1000){

        noButtonState = digitalRead(noButton);
        yesButtonState = digitalRead(yesButton);

        counter = counter + 1;
        
        //confirmation if passenger is discounted
        if(yesButtonState == HIGH)
        {
          Wire.beginTransmission(8);
          Wire.write(studentFare);//transmitting the signal of the variable studentFare
          Wire.endTransmission();
          break;
        }
        else if(noButtonState == HIGH)
        {
          Wire.beginTransmission(8);
          Wire.write(regularFare);//transmitting the signal of the variable regularFare
          Wire.endTransmission();
          break;
        }
        else if(counter == 1000)
        {
          Wire.beginTransmission(8);
          Wire.write(delayValue);
          Wire.endTransmission();
          break;
        }
      }
    }
    lastButtonState = studentButtonState;
  }
  else if (seniorpwdButtonState != lastButtonState)//if senior/pwd button is pressed
  {
    if(seniorpwdButtonState == HIGH)
    {
      digitalWrite(buzzerPin,LOW);
      delay(1000);
      digitalWrite(buzzerPin,HIGH);
      Wire.beginTransmission(8);
      Wire.write(confirmation);
      Wire.endTransmission();


      while(counter <= 1000){

        noButtonState = digitalRead(noButton);
        yesButtonState = digitalRead(yesButton);

        counter = counter + 1;
        //confirmation if the passenger is discounted
        if(yesButtonState == HIGH)
        {

          Wire.beginTransmission(8);
          Wire.write(seniorpwdFare);//transmitting the signal of the variable seniorpwdFare
          Wire.endTransmission();
          break;

        }
        else if(noButtonState == HIGH)
        {

          Wire.beginTransmission(8);
          Wire.write(regularFare);//transmitting the signal of the variable regularFare
          Wire.endTransmission();
          break; 
        }
        else if(counter == 1000)
        {
          Wire.beginTransmission(8);
          Wire.write(delayValue);
          Wire.endTransmission();
          break;
        }

      }
    }
    lastButtonState = seniorpwdButtonState;
  }
  else if (regularButtonState != lastButtonState)//if regular button is pressed
  {
    if(regularButtonState == HIGH)
    {

      Wire.beginTransmission(8);
      Wire.write(regularFare);//transmitting the signal of the variable regularFare
      Wire.endTransmission();
      delay(500);
    }

  }
  else if (nocoinsButtonState != lastButtonState)//if no coins button is pressed
  {
    if(nocoinsButtonState == HIGH)
    {
      //turning on the buzzer for 1 sec
      digitalWrite(buzzerPin,LOW);
      delay(1000);
      digitalWrite(buzzerPin,HIGH);

      Wire.beginTransmission(8);
      Wire.write(requestOpen);//transmitting the signal of the variable requestOpen
      Wire.endTransmission();
      delay(500);

    }
    lastButtonState = nocoinsButtonState;
  }
  else if (openButtonState != lastButtonState)//if open lock button is pressed
  {
    if(openButtonState == HIGH)
    {
      Wire.beginTransmission(8);
      Wire.write(openLock);//transmitting the signal of openLock
      Wire.endTransmission();
      delay(500);
    }
    lastButtonState = openButtonState;
  }
  else if (clearButtonState != lastButtonState)//if clear button is pressed
  {
    if(clearButtonState == HIGH)
    {
      Wire.beginTransmission(8);
      Wire.write(clearCounter);//transmitting the signal of clearCounter
      Wire.endTransmission();
      delay(500);

    }
    lastButtonState = clearButtonState;
  }

}















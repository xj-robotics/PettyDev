#include <PID_v1.h>
#include <Servo.h>
#include <assert.h>

int SpeedToPWM[256];
double lSpeed,rSpeed;
double lPWM,rPWM;
const int TargetSpeed = 233;//FIXME
double Kp=0.4, Ki=5.5, Kd=0;

const int shootPort = 52;
const int lSpeedPort = 17;
const int lControlPortA = 2;
const int lControlPortB = 3;
const int lReturnPort = 6;
const int rSpeedPort = 18;
const int rControlPortA = 4;
const int rControlPortB = 5;
const int rReturnPort = 7;

#define left true;
#define right false;

int mode=0;
int argument=100;//input speed (if have any)

// PID PID_L(&lSpeed, &lPWM, &TargetSpeed, Kp, Ki, Kd, DIRECT);
// PID PID_R(&rSpeed, &rPWM, &TargetSpeed, Kp, Ki, Kd, DIRECT);

void letForward(bool isLeftPort){
  if (isLeftPort){
    digitalWrite(lControlPortA,HIGH);
    digitalWrite(lControlPortB,LOW);
  }
  else{
    digitalWrite(rControlPortA,HIGH);parseArguments
    digitalWrite(rControlPortB,LOW);
  }
}

void letBackward(bool isLeftPort){
  if (isLeftPort){
    digitalWrite(lControlPortB,HIGH);
    digitalWrite(lControlPortA,LOW);
  }
  else{
    digitalWrite(rControlPortB,HIGH);
    digitalWrite(rControlPortA,LOW);
  }
}

void letHalt(bool isLeftPort){
  if (isLeftPort){
    digitalWrite(lControlPortA,HIGH);
    digitalWrite(lControlPortB,HIGH);
  }
  else{
    digitalWrite(rControlPortA,HIGH);
    digitalWrite(rControlPortB,HIGH);
  }
}

void changeSpeed(bool isLeftPort,int speed){
  assert((speed>0) && (speed<255));
  if (isLeftPort) analogWrite(lSpeedPort,speed);
  else analogWrite(rSpeedPort,speed);
}



void accumulateLSpeed(){lSpeed++;}
void accumulateRSpeed(){rSpeed++;}


void parseArguments();//TODO


void setup()
{
    Serial.begin(9600);
    while(Serial.read()>= 0) {} //clear serial port
    pinMode(shootPort,OUTPUT);//init()
    pinMode(lControlPortA,OUTPUT);
    pinMode(lControlPortB,OUTPUT);
    pinMode(rControlPortA,OUTPUT);
    pinMode(rControlPortB,OUTPUT);
    pinMode(lSpeedPort,OUTPUT);
    pinMode(rSpeedPort,OUTPUT);
    pinMode(lReturnPort,INPUT);
    pinMode(rReturnPort,INPUT);
    attachInterrupt(digitalPinToInterrupt(lReturnPort) , accumulateLSpeed, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rReturnPort) , accumulateRSpeed, CHANGE);
    // PID_L.SetMode(AUTOMATIC);
    // PID_L.SetSampleTime(50);
    // PID_R.SetMode(AUTOMATIC);
    // PID_R.SetSampleTime(50);
  }
void loop()
{
    currentMillis = millis ();
    if(Serial.available()>0)
    {
        parseArguments();//TODO
    }
    else
    {
        if( currentMillis - previousMillis >= 50 )
        {
            previousMillis = currentMillis ;
            // PID_L.Compute();PID_R.Compute();
            if (true)
            {
                switch(mode)
                {
                case 0:
                    STOP();
                    break;
                case 1:
                    FORWARD();
                    break;
                case 2:
                    BACK();;
                    break;
                case 3:
                    TURNLEFT();
                    delay(45);
                    STOP();
                    break;
                case 4:
                    TURNRIGHT();
                    delay(45);
                    STOP();
                    break;
                case 5:
                    SHOOT();
                    break;
                default :
                    STOP();
                    break;
                }
                // 0->STOP  1->FORWARD  2->BACK   3->LEFT   4->RIGHT   5->TURNLEFT  6->TURNRIGHT
                lSpeed = 0;
                rSpeed = 0;
            }
            if();
            // Serial.println("FIN");
        }
    }
}

void STOP(){
  letHalt(left);
  letHalt(right);
  mode=0;
}
void FORWARD(){
  letForward(left);
  letForward(right);
  analogWrite(lSpeedPort,lPWM);
  analogWrite(rSpeedPort,rPWM);
}
void BACKWARD(){
  letBackward(left);
  letBackward(right);
  analogWrite(lSpeedPort,lPWM);
  analogWrite(rSpeedPort,rPWM);
}
void TURNLEFT(){
  letHalt(left);
  letForward(right);
  analogWrite(lSpeedPort,lPWM);
  analogWrite(rSpeedPort,rPWM);
}
void TURNRIGHT(){
  letHalt(right);
  letForward(left);
  analogWrite(lSpeedPort,lPWM);
  analogWrite(rSpeedPort,rPWM);
}
void SHOOT(){
  digitalWrite(shootPort,HIGH);
  delay(600);//WITHOUT PID;
  digitalWrite(shootPort,LOW);
  delay(40);
  mode = 0;
}

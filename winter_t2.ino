#include "application.h"
#include <stdio.h>
#include <stdlib.h>


/////////////////////////////////////////// The setting for Servo /////////////////////////////////////////////////
Servo myservo1;
Servo myservo2;
char x[256];
int pos1 = 90;    // variable to store the servo position
int pos2 = 90;    
int change1 = D0; //pin settings for servo 1 and 2
int change2 = D1;

//For lightdata
int lightData;
int lightDataNum = 0;
int lightTotal = 0;
int lightAve = 0;
int LIGHT_MIN = 0;
int LIGHT_RANGE = 1000;

//For webTemp data
int webTemp;
int webTempNum = 0;
int webTempTotal = 0;
int webTempAvg = 0;
int TEMP_MIN = 30;
int TEMP_RANGE = 50;

//For motion data
int ifMotion;
int ifMotionNum = 0;
int ifMotionTotal = 0;
int ifMotionAvg = 0;
int MOTION_MIN = 0;
int MOTION_RANGE = 10;

//The parameter for original state
int ORIGIN_POS = 90;
//The parameter for the range of servo
int SERVO_LARGE = 80;
int SERVO_MIN = 10;
//The parameter for the range of each source



/////////////////////////////////////////// The setting for motor /////////////////////////////////////////////////
///////////////////////////Set the gears number//////////////////////////////
int STEPFORCIRCLE = 4076;//4096 or 4076 for a circle
//number of teeth in large/small gear
int HOUR_LGEARNUM = 120;
int HOUR_SGEARNUM = 16;
int MIN_LGEARNUM = 45;
int MIN_SGEARNUM = 16;
/////////////////////////////// Set the pin for motors//////////////////////////////////
int HOUR_MOTOR1 = A0;
int HOUR_MOTOR2 = A1;
int HOUR_MOTOR3 = A2;
int HOUR_MOTOR4 = A3;
int MIN_MOTOR1 = D2;
int MIN_MOTOR2 = D3;
int MIN_MOTOR3 = D4;
int MIN_MOTOR4 = D5;


/////////// Change the time interval &&& Moving steps each time ///////////////

//12 minute, 120000*HOUR_DEGREE

/////////////////////////////////////////////////// Begin to Run ///////////////////////////////////////////////////////
void setup() {
    //Set pin mode
    pinMode(change1, OUTPUT);
    pinMode(change2, OUTPUT);
    pinMode(HOUR_MOTOR1, OUTPUT);
    pinMode(HOUR_MOTOR2, OUTPUT);
    pinMode(HOUR_MOTOR3, OUTPUT);
    pinMode(HOUR_MOTOR4, OUTPUT);
    pinMode(MIN_MOTOR1, OUTPUT);
    pinMode(MIN_MOTOR2, OUTPUT);
    pinMode(MIN_MOTOR3, OUTPUT);
    pinMode(MIN_MOTOR4, OUTPUT);
    //set the servo
    myservo1.attach(change1);
    myservo2.attach(change2);
    //set internet
    Particle.subscribe("light1991pwpw", lightHandler, "2****800054734333937*****");
    Particle.subscribe("motion", motionHandler, "2****80005473433393*****");
    Particle.function("temp",tempChange);
    //handle the setting webpage
    Particle.function("webInterface",webHandler);
    Particle.function("webTuning",moveClockHandler);

}
void loop() {
    if(Time.second()==0){//trun the minute hand when each minute, when the Time.second() is 0.
        moveMinute(6);
        
        int m=Time.minute();//after "0seconds" of the each minute, check if the hour plate should trun
        if(m==0||m==12||m==24||m==36||m==48){
            moveHour(6);
        }
    }

}



////////////////////////////////////////////////////// Helper Function ///////////////////////////////////////////////////////////
///////////////////////////////////// Motor Function //////////////////////////////////////////

int moveHour(int hour_degree){//just give the degree and hour plate will trun
    int moveSteps = (-1)*STEPFORCIRCLE*hour_degree*HOUR_LGEARNUM/HOUR_SGEARNUM/360; 
    move(moveSteps, HOUR_MOTOR1, HOUR_MOTOR2, HOUR_MOTOR3, HOUR_MOTOR4);
    return moveSteps;
}

int moveMinute(int minute_degree){//same, minute plate
    int moveSteps = STEPFORCIRCLE*minute_degree*MIN_LGEARNUM/MIN_SGEARNUM/360;
    move(moveSteps, MIN_MOTOR1, MIN_MOTOR2, MIN_MOTOR3, MIN_MOTOR4);
    return moveSteps;
}

//takes the number of steps and the motor pin number, drive the motor
int move(int steps, int MOTOR1, int MOTOR2, int MOTOR3, int MOTOR4){
  int eachStep = 0;
  if(steps > 0){
    eachStep = -1;
  }
  if(steps < 0){
    eachStep = 1;
  }
  int thisStep;
  int i = 0;
  int steps_left = abs(steps);

  while(i < abs(steps)){
    i++;
    steps_left = steps_left + eachStep;
    thisStep = ( steps_left % 8 ) + 1;
    switch (thisStep) {
      case 1:
      digitalWrite(MOTOR1, HIGH);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(MOTOR3, LOW);
      digitalWrite(MOTOR4, LOW);
      break;
      case 2:
      digitalWrite(MOTOR1, HIGH);
      digitalWrite(MOTOR2, HIGH);
      digitalWrite(MOTOR3, LOW);
      digitalWrite(MOTOR4, LOW);
      break;
      case 3:
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, HIGH);
      digitalWrite(MOTOR3, LOW);
      digitalWrite(MOTOR4, LOW);
      break;
      case 4:
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, HIGH);
      digitalWrite(MOTOR3, HIGH);
      digitalWrite(MOTOR4, LOW);
      break;
      case 5:
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(MOTOR3, HIGH);
      digitalWrite(MOTOR4, LOW);
      break;
      case 6:
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(MOTOR3, HIGH);
      digitalWrite(MOTOR4, HIGH);
      break;
      case 7:
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(MOTOR3, LOW);
      digitalWrite(MOTOR4, HIGH);
      break;
      case 8:
      digitalWrite(MOTOR1, HIGH);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(MOTOR3, LOW);
      digitalWrite(MOTOR4, HIGH);
      break;
    }
    delay(8);
  }
  //shutdown the motor signal to save some power
  digitalWrite(MOTOR1, LOW);
  digitalWrite(MOTOR2, LOW);
  digitalWrite(MOTOR3, LOW);
  digitalWrite(MOTOR4, LOW);
  return 1;
}
/////////////////////////////////////Setting webpage handler/////////////////////////////////////
int webHandler(String command){
    int webTuningCommand;
    if (command=="what is the time, Photon"){
        return Time.now();
    }
    else if (command=="syncTime"){
        Particle.syncTime();
        return Time.now();
    }
    else{
        return 0;
    }
}

int moveClockHandler(String command){
    int com=command.toInt();
    if(com<=1000){
        int motorsteps=moveMinute(com-500);
        return motorsteps;
        }
    else if(com>1000){
        int motorsteps=moveHour(com-1500);
        return motorsteps;
    }
    else{
        return 0;
    }
}

///////////////////////////////////// Internet Function //////////////////////////////////////////
void lightHandler(const char *event, const char *data)
{
  lightDataNum++;
  if (data){
    //the range of the light
    lightData = atoi(data) - LIGHT_MIN;
    lightTotal = lightTotal + lightData;
    lightAve = lightTotal/lightDataNum;
    
  }
  else
    Serial.println("NULL");
}

void motionHandler(const char *event, const char *data)
{
    ifMotionNum++;
    if (data){
        //the range of the light
        ifMotion = atoi(data)*10 - MOTION_MIN;
        ifMotionTotal = ifMotionTotal + ifMotion;
        ifMotionAvg = ifMotionTotal/ifMotionNum;
    }
    else
        Serial.println("NULL");
}

int tempChange(String command) {
    /* Spark.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */
    webTempNum++;
    Serial.print("temperature data from the web is coming: ");
    Serial.print(command);

    webTemp = atoi(command) - TEMP_MIN;

    webTempTotal = webTempTotal + webTemp;
    Serial.print(", The total is: ");
    Serial.print(webTempTotal);
    webTempAvg = webTempTotal/webTempNum;
    Serial.print(", The average is: ");
    Serial.println(webTempAvg);
    return 1;
}

//clock adjustment function

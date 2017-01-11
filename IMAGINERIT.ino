//Clock 240 code recovered from Particle.io 
//For powering the Photon micro controller 
//Yebai Zhao, Summer 2016w
Servo myservoGB;//servo for Green and Blue
Servo myservoRY;//for Red and Yellow
//Tag names for each color
String redTag="null";
String yellowTag="null";
String greenTag="null";
String blueTag="null";

//every 12 mins' data collected from the cloud
int R_data_history[60];
int Y_data_history[60];
int G_data_history[60];
int B_data_history[60];
int pointer_time=0;

int red_data;
int yellow_data;
int blue_data;
int green_data;

//For webTemp data
int webTemp;
int webTempNum = 0;
int webTempTotal = 0;
int webTempAvg = 0;
int TEMP_MIN = 30;
int TEMP_RANGE = 50;

/////////////////////////////////////////// The setting for motor /////////////////////////////////////////////////
///////////////////////////Set the gears number//////////////////////////////
int STEPFORCIRCLE = 4086;//4096 or 4076 for a circle
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
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(HOUR_MOTOR1, OUTPUT);
    pinMode(HOUR_MOTOR2, OUTPUT);
    pinMode(HOUR_MOTOR3, OUTPUT);
    pinMode(HOUR_MOTOR4, OUTPUT);
    pinMode(MIN_MOTOR1, OUTPUT);
    pinMode(MIN_MOTOR2, OUTPUT);
    pinMode(MIN_MOTOR3, OUTPUT);
    pinMode(MIN_MOTOR4, OUTPUT);
    //set the servo pins
    myservoGB.attach(D0);
    myservoRY.attach(D1);
    //set internet
    Particle.subscribe("motion1236799", redHandler, "2f0038000547343339373536");
    Particle.subscribe("light1991pwpw", yellowHandler, "2f0038000547343339373536");

    Particle.function("temp",tempChange);
    //handle the requesets from the setting webpage
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
        if(m==59||m==11||m==23||m==35||m==47){
            //moving blocks
            servoMover(0,red_data);
            servoMover(1,yellow_data);
            servoMover(2,green_data);
            servoMover(3,blue_data);
            //saving the current data to history arrays
            R_data_history[pointer_time]=red_data;
            Y_data_history[pointer_time]=yellow_data;
            G_data_history[pointer_time]=green_data;
            B_data_history[pointer_time]=blue_data;
            //move the array pointer to the next
            if(pointer_time==59){pointer_time=0;}
            else{pointer_time++;}
            
        }
    }

}

//END OF THE MAIN PROGRAM//


////////////////////////////////////////////////////// Functions ///////////////////////////////////////////////////////////

/////////////////////////////////////Setting webpage handler/////////////////////////////////////
int webHandler(String command){
    String cmd_head= command.substring(0,5);//cut the top 5
    int cmd_body= command.substring(5).toInt();
    if (cmd_head=="get T"){
        return Time.now();
    }
    else if (cmd_head=="syncT"){
        Particle.syncTime();
        return Time.now();
    }
    else if(cmd_head=="DataA"){
        
    }
    
    else {
        return 0;
    }
}

int moveClockHandler(String command){   // this function takes the first five letters of the cmd string as the command header
                                        //and select proper movement by the 6th and following string.
    String cmd_head= command.substring(0,5);//cut the top 5
    int cmd_body= command.substring(5).toInt();//convert the left string to int
    if(cmd_head=="minTr"){
        moveMinute(cmd_body);
    }
    else if(cmd_head=="houTr"){
        moveHour(cmd_body);
    }
    else if(cmd_head=="ser 0"){//red
        cmd_body=servoMover(0,cmd_body);
    }
    else if(cmd_head=="ser 1"){//yellow
        cmd_body=servoMover(1,cmd_body);
    }
    else if(cmd_head=="ser 2"){//green
        cmd_body=servoMover(2,cmd_body);
    }
    else if(cmd_head=="ser 3"){//blue
        cmd_body=servoMover(3,cmd_body);
    }
    else{
        cmd_body=404;    
    }
    return cmd_body; // return a debug reference number
}

/////////////////////////////////////Servo////////////////////////////////////////////////
//Tell it which servo to move (servo 0 to servo 3, center to rim), and the range of movement from 0 to 99. the function will do its best
int servoMover(int track_number, int percent){
    //the contact limitation degree values, in servo output degree, max=push min=contact postition
    //this function is quite fucked up, so don't fell sad if it doesnt make sense to you.
    int redMax=10;
    int redMin=75;
    int yelMax=175;
    int yelMin=120;
    int greMax=10;
    int greMin=80;
    int bluMax=170;
    int bluMin=100;
    double percentage=percent/100.0;
    int destination;
    int static_RY=(redMin+yelMin)/2;
    int static_GB=(greMin+bluMin)/2;
    if(percent!=0){
        switch(track_number){
            case 0:
                destination=(int)round(redMin-(redMin-redMax)*percentage);
                for(int toservo=static_RY; toservo>destination; toservo-=30){//the servo move in mutiple times, avoid jamming in this for loop
                    myservoRY.write(toservo);
                    delay(500);
                    myservoRY.write(toservo+5);
                    delay(500);
                }
                myservoRY.write(destination);
                delay(500);
                myservoRY.write(static_RY);
                
                break;
            case 1:
                destination=(int)round(yelMin+percentage*(yelMax-yelMin));
                for(int toservo=static_RY;toservo<destination;toservo+=30){
                    myservoRY.write(toservo);
                    delay(500);
                    myservoRY.write(toservo-5);
                    delay(500);
                }
                myservoRY.write(destination);
                delay(500);
                myservoRY.write(static_RY);
                break;
            case 2:
                destination=(int)round(greMin-percentage*(greMin-greMax));
                for(int toservo=static_GB; toservo>destination; toservo-=30){
                    myservoGB.write(toservo);
                    delay(500);
                    myservoGB.write(toservo+5);
                    delay(500);
                }
                myservoGB.write(destination);
                delay(500);
                myservoGB.write(static_GB);
                break;
            case 3:
                destination=(int)round(bluMin+(bluMax-bluMin)*percentage);
                for(int toservo=static_GB; toservo<destination; toservo+=30){
                    myservoGB.write(toservo);
                    delay(500);
                    myservoGB.write(toservo-5);
                    delay(500);
                }
                myservoGB.write(destination);
                delay(500);
                myservoGB.write(static_GB);
                break;
        }
    }
    return destination;// the servo destination degree
}


///////////////////////////////////// Internet Function //////////////////////////////////////////


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
/////////////////////////////////////RYGB blocks moving/////////////////////////////////////////

void redHandler(String name, String command){
    redTag= command.substring(0,15);//cut the top 15
    int cmd_body= command.substring(15).toInt();//convert the left string to int
    if(cmd_body>0&&cmd_body<100){
        red_data=cmd_body;
    }
    else if(cmd_body>=100){
        red_data=99;
    }
    else{
        red_data=0;
    }
    Particle.publish("gotred", redTag+","+String(red_data));
}
void yellowHandler(String name, String command){
    yellowTag= command.substring(0,15);
    int cmd_body= command.substring(15).toInt();
    if(cmd_body>0&&cmd_body<100){
        yellow_data=cmd_body;
    }
    else if(cmd_body>=100){
        yellow_data=99;
    }
    else{
        yellow_data=0;
    }
    Particle.publish("gotyellow", yellowTag+","+String(yellow_data));
}



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


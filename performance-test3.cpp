#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#define OPTOTHRESH 2.0
#define ENCODERINCH 10


//declarations for encoders (right encoder (re) / left encoder (le) )
DigitalEncoder re(FEHIO::P3_5);
DigitalEncoder le(FEHIO::P3_0);
//declaration for motors (right motor (rm) / left motor (lm) )
FEHMotor rm(FEHMotor::Motor1,7.2);
FEHMotor lm(FEHMotor::Motor0,7.2);
//declaration for CdS cell
AnalogInputPin cds(FEHIO::P2_1);
//declaration for optosensors (left optosensor (lo). . .)
DigitalInputPin lo(FEHIO::P1_0);
DigitalInputPin mo(FEHIO::P1_2);
DigitalInputPin ro(FEHIO::P1_4);

//declares a servo on servo port 3
FEHServo arm_servo(FEHServo::Servo0);
//declares a servo on servo port 2
FEHServo ticket_servo(FEHServo::Servo7);
//declares a servo on servo port 5
FEHServo burger_servo(FEHServo::Servo5);

//function to stop motors
void stopMotors() {
lm.SetPercent(0);
rm.SetPercent(0);
Sleep(500);
}

//move function
void move(int percent, int counts) {
//reset encoder
re.ResetCounts();
le.ResetCounts();
if (percent > 0) {
//set motors to percent
rm.SetPercent(percent + 3);
lm.SetPercent(percent);
} else if (percent < 0) {
  rm.SetPercent(percent);
  lm.SetPercent(percent + 3);
}
//While the average of the left and right encoder is less than counts,
//keep running motors
while((le.Counts() + re.Counts()) / 2. < counts);

stopMotors();
}

//turn Right function
void turnRight (double angle) {
//reset encoder
re.ResetCounts();
le.ResetCounts();

//encoder count for 90 degrees turn
double leftFull = 120;
double rightFull = 110;

//encoder count for specified angle parameter
double actualLeft = (leftFull/90.0) * angle;
double actualRight = (rightFull/90.0) * angle;

//turn servo for specified angle
rm.SetPercent(-30);
lm.SetPercent(30);
while(le.Counts() < actualLeft && re.Counts() < actualRight){}

stopMotors();
}

//turn Left function
void turnLeft (double angle) {
//reset encoder
re.ResetCounts();
le.ResetCounts();

//sleep duration for 90 degrees turn
double leftFull = 95;
double rightFull = 105;

//sleep duration for specified angle parameter
double actualLeft = (leftFull/90) * angle;
double actualRight = (rightFull/90) * angle;

//turn servo for specified angle
rm.SetPercent(30);
lm.SetPercent(-30);
while(le.Counts() < actualLeft && re.Counts() < actualRight){}

stopMotors();
}

//Detect Light
int detectLight() {
double lightLevel = cds.Value();
int value;
if(lightLevel > 3) {
  value = 0;
} else if (lightLevel > 1.8) {
  value = 2;
} else {
  value = 1;
}

return value;
}

//Algorithm for jukebox on preformance test 1, to be edited for future use
void jukebox(int color) {
//Backup to line up center
turnLeft(90.0);
turnLeft(45.0);
move(-30, ENCODERINCH * 3);
turnRight(45.0);
move(-30, ENCODERINCH * 2);

//If color = 1, run for red, otherwise run for blue
if (color == 1) {
  LCD.SetFontColor(RED);
  LCD.FillRectangle(0, 0, 359, 239);
  turnRight(45.0);
  move(50, ENCODERINCH * 2 + 6);
  turnLeft(47.0);
  move(30, ENCODERINCH * 7);
  move(-30, ENCODERINCH * 4);
  turnLeft(95.0);
  move(50, ENCODERINCH * 9 + 4);
} else {
  LCD.SetFontColor(BLUE);
  LCD.FillRectangle(0, 0, 359, 239);
  turnLeft(45.0);
  move(50, ENCODERINCH * 2 + 2);
  turnRight(40.0);
  move(30, ENCODERINCH * 7);
  move(-30, ENCODERINCH * 4);
  turnLeft(95.0);
  move(50, ENCODERINCH * 5 + 4);
}
}

//followLine function for specified time in seconds
void followLine (double time) {
//state variable
int state = 0;
//time measure
float timeNow;
timeNow = TimeNow();
while(TimeNow()-timeNow < time) {
 //line at left
 if (lo.Value() > OPTOTHRESH && ro.Value() < OPTOTHRESH && mo.Value() < OPTOTHRESH) {
     lm.SetPercent(0);
     rm.SetPercent(20);
     state = 1;
     Sleep(100);
 }else if (ro.Value() > OPTOTHRESH && lo.Value() < OPTOTHRESH && mo.Value() < OPTOTHRESH) {
     //line at right
     lm.SetPercent(20);
     rm.SetPercent(0);
     state = 2;
     Sleep(100);
 }else if (mo.Value() > OPTOTHRESH && lo.Value() < OPTOTHRESH && ro.Value() < OPTOTHRESH) {
     //on line
     lm.SetPercent(20);
     rm.SetPercent(20);
     Sleep(100);
 } else if (lo.Value() > OPTOTHRESH && ro.Value() > OPTOTHRESH && mo.Value() > OPTOTHRESH) {
     //on line
     lm.SetPercent(20);
     rm.SetPercent(20);
     Sleep(100);
 } else if (lo.Value() > OPTOTHRESH && ro.Value() < OPTOTHRESH && mo.Value() > OPTOTHRESH){
     //line slightly at right
     lm.SetPercent(20);
     rm.SetPercent(10);
     state = 2;
     Sleep(100);
 } else if (lo.Value() < OPTOTHRESH && ro.Value() > OPTOTHRESH && mo.Value() > OPTOTHRESH) {
     //line slightly at left
     lm.SetPercent(10);
     rm.SetPercent(20);
     state = 1;
     Sleep(100);
 } else {
     //nothing detected
     if(state == 0) {
         lm.SetPercent(20);
         rm.SetPercent(20);
         Sleep(100);
     } else if(state == 1) {
         lm.SetPercent(0);
         rm.SetPercent(20);
         Sleep(100);
     } else if(state == 2) {
         lm.SetPercent(20);
         rm.SetPercent(0);
         Sleep(100);
     }
 }
}//end while loop
stopMotors();
}// end method

void testCds() {
double time = TimeNow();
while(TimeNow() - time < 500){
  LCD.Clear();
  LCD.Write(cds.Value());
  Sleep(500);
}
}

//Servo expand arm function//
//15 degrees is the ticket arm startig position
//150 degrees is the ticket arm before pushing ticket
//180 degrees is the ticket arm after pushing ticket
void ticket(int angle){
ticket_servo.SetDegree(angle);
Sleep(1.0);
}

//Second Servo vertical movment//
//180 degree is when slider is up
//135 is perpendicular to ground
//
void tray(int angle)
{
arm_servo.SetDegree(angle);
Sleep(200);
}
//Move arm Servo push ice cream lever//
void iceCream(int angle)
{
 arm_servo.SetDegree(angle);
 Sleep(200);
}
//Function to flip burger//
void BurgerFlip(int angle)
{
 burger_servo.SetDegree(angle);
 Sleep(200);


}
//function for wiggling towards ticket
void wiggle (double time)
{
while(TimeNow()-time < 5)
{
    rm.SetPercent(20);
    Sleep(0.1);
    rm.SetPercent(0);
    lm.SetPercent(10);
    Sleep(0.1);
    lm.SetPercent(0);
}
}

int main(void)
{
//Calibration code to set servo motors//
arm_servo.SetMin(725);
arm_servo.SetMax(2450);
ticket_servo.SetMin(747);
ticket_servo.SetMax(2424);
burger_servo.SetMin(515);
burger_servo.SetMax(2450);
tray(120);
ticket(10.0);
burger_servo.SetDegree(0);

//Wait until red light//
while(detectLight() == 0){}

//Move forward to mid ramp//
move(40, ENCODERINCH * 16 - 6);
//Turn left to face parallel to ramp to move backwards//
turnLeft(142.5);
//Move up the ramp slightly past kitchen//
move(-50, ENCODERINCH * 43);
//Turn to face burger//
turnLeft(90.0);
//Move forward
//Flip burger//

move(40, ENCODERINCH * 2);
BurgerFlip(110);
move(-20, ENCODERINCH);
Sleep(1.0);
BurgerFlip(180);
move(40, ENCODERINCH * 2 + 5);
BurgerFlip(90);
move(-40, ENCODERINCH * 3 + 5);
BurgerFlip(180);
//turnRight(30);
//Sleep(1.0);
//BurgerFlip(180);
//move(40, ENCODERINCH * 2);
//turnLeft(30);
//move(40, ENCODERINCH * 2 + 5);
//turnLeft(30);
//move(40, ENCODERINCH * 2);
//turnRight(30);
//BurgerFlip(90);
//move(-40, ENCODERINCH * 2);
//BurgerFlip(180);



move(-40, ENCODERINCH * 3);

//turn left
turnRight(220);
BurgerFlip(0);
Sleep(200);
move(20, ENCODERINCH);

//Flip arm tray up to flip ice cream//
iceCream(55);
}




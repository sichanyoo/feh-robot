#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#define OPTOTHRESH 2.0
#define ENCODERINCH 20


//declarations for encoders (right encoder (re) / left encoder (le) )
DigitalEncoder re(FEHIO::P3_5);
DigitalEncoder le(FEHIO::P0_0);
//declaration for motors (right motor (rm) / left motor (lm) )
FEHMotor rm(FEHMotor::Motor1,9.0);
FEHMotor lm(FEHMotor::Motor0,9.0);
//declaration for CdS cell
AnalogInputPin cds(FEHIO::P2_1);
//declaration for optosensors (left optosensor (lo). . .)
DigitalInputPin lo(FEHIO::P1_0);
DigitalInputPin mo(FEHIO::P1_2);
DigitalInputPin ro(FEHIO::P1_4);

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

  //set motors to percent
  rm.SetPercent(percent);
  lm.SetPercent(percent);

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
  double leftFull = 135;
  double rightFull = 115;

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
  double leftFull = 115;
  double rightFull = 135;

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
   if(lightLevel > 2.8) {
       value = 0;
   } else if (lightLevel > 1.8) {
       value = 2;
   } else if (lightLevel > 1.1) {
       value = 3;
   } else {
       value = 1;
   }

   return value;
}

//Algoritm for jukebox on preformance test 1, to be edited for future use
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
          rm.SetPercent(0);
          state = 2;
          Sleep(100);
      } else if (lo.Value() < OPTOTHRESH && ro.Value() > OPTOTHRESH && mo.Value() > OPTOTHRESH) {
          //line slightly at left
          lm.SetPercent(0);
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

void testCDS() {
   double time = TimeNow();
   while(TimeNow() - time < 500){
       LCD.Clear();
       LCD.Write(cds.Value());
       Sleep(500);
   }
}

int main(void)
{

//   //touch screen position variables
//   float x,y;

  //initialize the screen
  LCD.Clear(FEHLCD::Black);
  LCD.SetFontColor(FEHLCD::White);

//   //wait for screen to be pressed
//   while(!LCD.Touch(&x,&y));
//   //wait for screen to be unpressed
//   while(LCD.Touch(&x,&y));

//   testCDS();
  //wait for start light
//   while(detectLight() == 0) {
//   }
   while(cds.Value() > 2.0) {}

  //move forward for 10 inches
   move(40, ENCODERINCH*10);
  //turn left 42 degrees
   turnLeft(42.0);
  //move until detect light
   move(40, ENCODERINCH*8);

   //Detect Color for Jukebox button
   int color = detectLight();

   //Oscilate until a light is found
   int backup = 2;
   while(color == 0) {
         move(10, backup);
         backup = backup * -1 + 2;
   }

   //Run jukebox algorithm
   jukebox(color);

   //turn left towards ramp
   turnLeft(90.0);

   //move up and down ramp
   move(50, ENCODERINCH * 25);
   move(-30, ENCODERINCH * 25);



//   //move forward or backward depending on detected light color


//   //turn left 90 degrees
//    turnLeft(90.0);

//   //move forward to push button
//    move(40, ENCODERINCH*3);

//   //back up
//    move(-40, ENCODERINCH*4);

//   //turn left 90 degrees
//    turnLeft(90.0);

//   //move forward 4 inches
//    move(40, ENCODERINCH*5);

//   //turn left 90 degrees
//    turnLeft(90.0);

//   //move forward 30 inches
//    move(40, ENCODERINCH*30);

//   //turn 180 degrees left
//    turnLeft(180.0);

//   //move forward 30 inches
//    move(40, ENCODERINCH*30);


//   //cds value test code
//   while (true) {
//      LCD.Clear();
//      LCD.Write(cds.Value());
//      Sleep(0.3);
//   }

}


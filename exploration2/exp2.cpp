#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#define THRESHOLD 2.0

int main(void)
{
   float x,y;

   LCD.Clear(FEHLCD::Black);
   LCD.SetFontColor(FEHLCD::White);

   AnalogInputPin left(FEHIO::P1_2);
   AnalogInputPin middle(FEHIO::P1_1);
   AnalogInputPin right(FEHIO::P1_0);
   FEHMotor leftWheel(FEHMotor::Motor0, 9);
   FEHMotor rightWheel(FEHMotor::Motor1, 9);

   int state = 0;

   while(true)
   {
       //line at left
       if (left.Value() > THRESHOLD && right.Value() < THRESHOLD && middle.Value() < THRESHOLD) {
           leftWheel.SetPercent(0);
           rightWheel.SetPercent(20);
           state = 1;
           Sleep(100);
       }else if (right.Value() > THRESHOLD && left.Value() < THRESHOLD && middle.Value() < THRESHOLD) {
           //line at right
           leftWheel.SetPercent(20);
           rightWheel.SetPercent(0);
           state = 2;
           Sleep(100);
       }else if (middle.Value() > THRESHOLD && left.Value() < THRESHOLD && right.Value() < THRESHOLD) {
           //on line
           leftWheel.SetPercent(20);
           rightWheel.SetPercent(20);
           Sleep(100);
       } else if (left.Value() > THRESHOLD && right.Value() > THRESHOLD && middle.Value() > THRESHOLD) {
           //on line
           leftWheel.SetPercent(20);
           rightWheel.SetPercent(20);
           Sleep(100);
       } else if (left.Value() > THRESHOLD && right.Value() < THRESHOLD && middle.Value() > THRESHOLD){
           //line slightly at right
           leftWheel.SetPercent(20);
           rightWheel.SetPercent(0);
           state = 2;
           Sleep(100);
       } else if (left.Value() < THRESHOLD && right.Value() > THRESHOLD && middle.Value() > THRESHOLD) {
           //line slightly at left
           leftWheel.SetPercent(0);
           rightWheel.SetPercent(20);
           state = 1;
           Sleep(100);
       } else {
           //nothing detected
           if(state == 0) {
               leftWheel.SetPercent(20);
               rightWheel.SetPercent(20);
               Sleep(100);
           } else if(state == 1) {
               leftWheel.SetPercent(0);
               rightWheel.SetPercent(20);
               Sleep(100);
           } else if(state == 2) {
               leftWheel.SetPercent(20);
               rightWheel.SetPercent(0);
               Sleep(100);
           }
       }

//        LCD.Write("Left: ");
//        LCD.Write(left.Value());
//        LCD.Write(", ");
//        LCD.Write("Middle: ");
//        LCD.Write(middle.Value());
//        LCD.Write(", ");
//        LCD.Write("Right: ");
//        LCD.Write(right.Value());
//        LCD.Write(", ");
//        Sleep(2.0);
//        LCD.Clear();
   }

}

///

#include <FEHLCD.h>

#include <FEHIO.h>

#include <FEHUtility.h>

#include <FEHMotor.h>

#include <FEHRPS.h>



//Declarations for encoders & motors

DigitalEncoder right_encoder(FEHIO::P0_0);

DigitalEncoder left_encoder(FEHIO::P0_1);

FEHMotor right_motor(FEHMotor::Motor0,9.0);

FEHMotor left_motor(FEHMotor::Motor1,9.0);


void move_forward(int percent, int counts) //using encoders

{

   //Reset encoder counts

   right_encoder.ResetCounts();

   left_encoder.ResetCounts();



   //Set both motors to desired percent

   right_motor.SetPercent(percent);

   left_motor.SetPercent(percent);



   //While the average of the left and right encoder is less than counts,

   //keep running motors

   while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);



   //Turn off motors

   right_motor.Stop();

   left_motor.Stop();

}



int main(void)

{

   int motor_percent = 60; //Input power level here

   int expected_counts = 242; //Input theoretical counts here



   float x, y; //for touch screen



   //Initialize the screen

   LCD.Clear(BLACK);

   LCD.SetFontColor(WHITE);



   LCD.WriteLine("Shaft Encoder Exploration Test");

   LCD.WriteLine("Touch the screen");

   while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed

   while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed



   move_forward(motor_percent, expected_counts); //see function



   Sleep(2.0); //Wait for counts to stabilize

   //Print out data

   LCD.Write("Theoretical Counts: ");

   LCD.WriteLine(expected_counts);

   LCD.Write("Motor Percent: ");

   LCD.WriteLine(motor_percent);

   LCD.Write("Actual LE Counts: ");

   LCD.WriteLine(left_encoder.Counts());

   LCD.Write("Actual RE Counts: ");

   LCD.WriteLine(right_encoder.Counts());

}

StepEncoder
#include <FEHLCD.h>

#include <FEHIO.h>

#include <FEHUtility.h>

#include <FEHMotor.h>

#include <FEHRPS.h>



//Declarations for encoders & motors

DigitalEncoder right_encoder(FEHIO::P0_0);

DigitalEncoder left_encoder(FEHIO::P0_1);

FEHMotor right_motor(FEHMotor::Motor0,9.0);

FEHMotor left_motor(FEHMotor::Motor1,9.0);


void move_forward(int percent, int counts) //using encoders

{

   //Reset encoder counts

   right_encoder.ResetCounts();

   left_encoder.ResetCounts();



   //Set both motors to desired percent

   right_motor.SetPercent(percent);

   left_motor.SetPercent(percent);



   //While the average of the left and right encoder is less than counts,

   //keep running motors

   while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);



   //Turn off motors

   right_motor.SetPercent(0);

   left_motor.SetPercent(0);

}

void turnLeft(int percent) {
   left_motor.SetPercent(percent* -1);
   right_motor.SetPercent(0);

   Sleep(1.0);

   left_motor.SetPercent(0);
}

void turnRight(int percent) {
   right_motor.SetPercent(percent * -1);
   left_motor.SetPercent(0);

   Sleep(1.0);

   right_motor.SetPercent(0);
}



int main(void)

{

   int motor_percent = 40; //Input power level here

   int expected_counts = 567; //Input theoretical counts here



   float x, y; //for touch screen



   //Initialize the screen

   LCD.Clear(BLACK);

   LCD.SetFontColor(WHITE);



   LCD.WriteLine("Shaft Encoder Exploration Test");

   LCD.WriteLine("Touch the screen");

   while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed

   while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed



   move_forward(40, 567); //see function
   Sleep(200);
   turnLeft(50);
   Sleep(200);
   move_forward(40, 405);
   Sleep(200);
   turnRight(50);
   Sleep(200);
   move_forward(40, 162);



   Sleep(2.0); //Wait for counts to stabilize

   //Print out data

   LCD.Write("Theoretical Counts: ");

   LCD.WriteLine(expected_counts);

   LCD.Write("Motor Percent: ");

   LCD.WriteLine(motor_percent);

   LCD.Write("Actual LE Counts: ");

   LCD.WriteLine(left_encoder.Counts());

   LCD.Write("Actual RE Counts: ");

   LCD.WriteLine(right_encoder.Counts());

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#define OPTOTHRESH 2.0

//declarations for encoders (right encoder (re) / left encoder (le) )
DigitalEncoder re(FEHIO::P3_5);
DigitalEncoder le(FEHIO::P0_0);
//declaration for motors (right motor (rm) / left motor (lm) )
FEHMotor rm(FEHMotor::Motor1,9.0);
FEHMotor lm(FEHMotor::Motor0,9.0);
//declaration for CdS cell
AnalogInputPin cds(FEHIO::P3_1);
//declaration for optosensors (left optosensor (lo). . .)
DigitalInputPin lo(FEHIO::P1_0);
DigitalInputPin mo(FEHIO::P1_2);
DigitalInputPin ro(FEHIO::P1_4);

//function to stop motors
void stopMotors() {
   lm.SetPercent(0);
   rm.SetPercent(0);
   Sleep(0.5);
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

int main(void)
{

   //touch screen position variables
   float x,y;

   //initialize the screen
   LCD.Clear(FEHLCD::Black);
   LCD.SetFontColor(FEHLCD::White);

   //wait for screen to be pressed
   while(!LCD.Touch(&x,&y));
   //wait for screen to be unpressed
   while(LCD.Touch(&x,&y));

}

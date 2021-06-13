#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

void move_forward(int percent, int counts) //using encoders
{
   //Reset encoder counts
   right_encoder.ResetCounts();
   left_encoder.ResetCounts();

   //Set both motors to desired percent
   right_motor.SetPercent(percent);
   left_motor.SetPercent(percent);

   //While the average of the left and right encoder are less than counts,
   //keep running motors
   while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

   //Turn off motors
   right_motor.Stop();
   left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
   //Reset encoder counts
   right_encoder.ResetCounts();
   left_encoder.ResetCounts();

   //Set both motors to desired percent
   right_motor.SetPercent(percent);
   left_motor.SetPercent(-percent);

   //While the average of the left and right encoder are less than counts,
   //keep running motors
   while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

   //Turn off motors
   right_motor.Stop();
   left_motor.Stop();
}

void check_x_minus(float x_coordinate) //using RPS while robot is in the -x direction
{
   //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
   while((RPS.X() != -1 && RPS.X() != -2) && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
   {
       if(RPS.X() > x_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

          move_forward(30,2);
          Sleep(100);
       }
       else if(RPS.X() < x_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

           move_forward(-30,2);
           Sleep(100);
       }
   }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
   //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
   while((RPS.Y() != -1 && RPS.Y() != -2) && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
   {
       if(RPS.Y() > y_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

           move_forward(30,2);
           Sleep(100);
       }
       else if(RPS.Y() < y_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

           move_forward(-30,2);
           Sleep(100);
       }
   }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
   //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
   while(((RPS.Y() != -1 && RPS.Y() != -2)) && (RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1))
   {
       if(RPS.Y() > y_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

           move_forward(-30,2);
           Sleep(100);
       }
       else if(RPS.Y() < y_coordinate)
       {
           //pulse the motors for a short duration in the correct direction

           move_forward(30,2);
           Sleep(100);
       }
   }
}

void check_heading(float heading) //using RPS
{
   while((RPS.Heading() != -1 && RPS.Heading() != -2) && (RPS.Heading() < heading - 1 || RPS.Y() > heading + 1))
   {
       if(RPS.Heading() > heading)
       {
           //pulse the motors for a short duration in the correct direction

           turn_left(30, 5);
           Sleep(100);
       }
       else if(RPS.Heading() < heading)
       {
           //pulse the motors for a short duration in the correct direction

           turn_left(-30, 5);
           Sleep(100);
       }
   }
}

int main(void)
{
   float touch_x,touch_y;
   float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
   float A_heading, B_heading, C_heading, D_heading;
   int power, B_C_counts, C_D_counts, turn_90_counts;
   float x [10];
   float y [10];
   RPS.InitializeTouchMenu();

   LCD.WriteLine("RPS & Data Logging Test");
   LCD.WriteLine("Press Screen To Start");
   while(!LCD.Touch(&touch_x,&touch_y));
   while(LCD.Touch(&touch_x,&touch_y));

   //WRITE CODE HERE TO READ SD CARD FOR LOGGED X AND Y DATA POINTS

   FEHFile *ifptr = SD.FOpen("Nums.txt","r");
   SD.FScanf(ifptr,"%f%f", x,y);
   SD.FClose(ifptr);
   //WRITE CODE HERE TO SET THE HEADING DEGREES, MOTOR POWER, AND COUNTS VALUES
   power = 30.0;
   B_heading = 180;
   B_y = 62.699;
   //A --> B
   check_y_plus(B_y);
   check_heading(B_heading);

   C_heading = 270;
   B_C_counts = 648;
   C_x = -2.0;
   //B --> C
   move_forward(power, B_C_counts);
   check_x_minus(C_x);
   turn_left(power, turn_90_counts);
   check_heading(C_heading);

   D_heading = 0;
   C_D_counts = 405;
   D_y = 51.0;
   //C --> D
   move_forward(power, C_D_counts);
   check_y_minus(D_y);
   turn_left(power, turn_90_counts);
   check_heading(D_heading);

   return 0;
}

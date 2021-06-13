#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHIO.h>
#include <FEHServo.h>
#include <FEHMotor.h>
int main(void)
{
   float x,y;

   LCD.Clear(FEHLCD::Black);
   LCD.SetFontColor(FEHLCD::White);
//    AnalogInputPin cds(FEHIO::P1_4);
//    FEHServo servo(FEHServo::Servo0);
//    servo.SetMin(500);
//    servo.SetMax(2400);

//    while (true) {
//        if (cds.Value() < 0.3)
//        servo.SetDegree(4);
//        else if(cds.Value() > 3.0)
//        {
//            servo.SetDegree(178);
//        }
//        else
//        {
//            servo.SetDegree(cds.Value() * 53);
//        }
//    }

    DigitalInputPin frontRight(FEHIO::P0_0);
    DigitalInputPin frontLeft(FEHIO::P3_7);
    DigitalInputPin backRight(FEHIO::P0_1);
    DigitalInputPin backLeft (FEHIO::P3_6);
   FEHMotor leftWheel (FEHMotor::Motor0, 9.0);
   FEHMotor rightWheel (FEHMotor::Motor1, 9.0);


   rightWheel.SetPercent(40);
   leftWheel.SetPercent(40);

   while (frontLeft.Value() && frontRight.Value()) {

   }

   rightWheel.SetPercent(0);
   leftWheel.SetPercent(0);

   Sleep(1500);


   leftWheel.SetPercent(-50);
   rightWheel.SetPercent(0);

   Sleep(1000);

   rightWheel.SetPercent(25);
   leftWheel.SetPercent(25);
   while (frontLeft.Value() && frontRight.Value()) {

   }

   rightWheel.SetPercent(0);
   leftWheel.SetPercent(0);

   Sleep(1500);


   leftWheel.SetPercent(0);
   rightWheel.SetPercent(-50);

   Sleep(1000);

   rightWheel.SetPercent(25);
   leftWheel.SetPercent(25);

   while (frontLeft.Value() && frontRight.Value()) {

   }

   rightWheel.SetPercent(0);
   leftWheel.SetPercent(0);

   Sleep(10.0);
//    while(frontRight.Value() && frontLeft.Value() && backRight.Value() && backLeft.Value()) {

//    }

//    rightWheel.SetPercent(0);
//    leftWheel.SetPercent(0);


//    Sleep(200);

//    rightWheel.SetPercent(-40);

//    float now = TimeNow();

//    while(TimeNow() - now  > 0.5) {

//    }

//    while(frontRight.Value() && frontLeft.Value() && backRight.Value() && backLeft.Value()) {

//    }

//    rightWheel.SetPercent(0);

//    Sleep(200);

//    rightWheel.SetPercent(40);
//    leftWheel.SetPercent(40);

//    now = TimeNow();

//    while(TimeNow() - now  > 0.5) {

//    }

//    while(frontRight.Value() && frontLeft.Value() && backRight.Value() && backLeft.Value()) {

//    }


//    rightWheel.SetPercent(0);
//    leftWheel.SetPercent(0);

//    Sleep(200);

//    leftWheel.SetPercent(-40);

//    now = TimeNow();

//    while(TimeNow() - now  > 0.5) {

//    }

//    while(frontRight.Value() && frontLeft.Value() && backRight.Value() && backLeft.Value()) {

//    }

//    leftWheel.SetPercent(0);

//    Sleep(200);

//    rightWheel.SetPercent(40);
//    leftWheel.SetPercent(40);

//    now = TimeNow();

//    while(now - TimeNow()  > 0.5) {

//    }

//    while(frontRight.Value() && frontLeft.Value() && backRight.Value() && backLeft.Value()) {

//    }

//    rightWheel.SetPercent(0);
//    leftWheel.SetPercent(0);

}

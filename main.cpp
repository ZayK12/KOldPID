#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
controller Controller1 = controller(primary);
motor FrontLeft = motor(PORT20, ratio18_1, false);

motor BackLeft = motor(PORT10, ratio18_1, false);

motor FrontRight = motor(PORT11, ratio18_1, true);

motor BackRight = motor(PORT2, ratio18_1, false);

inertial Inertial4 = inertial(PORT4);

motor MotorGroup18MotorA = motor(PORT18, ratio18_1, false);
motor MotorGroup18MotorB = motor(PORT17, ratio18_1, false);
motor_group MotorGroup18 = motor_group(MotorGroup18MotorA, MotorGroup18MotorB);




// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       {author}                                                  */
/*    Description:  V5 project                                                */
/*    Created:      {date}                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
int input = 0;
int turninput = 45;

int TV = input/(3.14159 * 104.775) * 360;
bool swtch = true; // switch to toggle while loop
bool swtch2 = true; // debug switch



// Allows for easier use of the VEX Library
using namespace vex;
competition Competition;

double Kp = 0.275; // Constant for Proportion
double Ki = 0.0001; // Constant for Integral
double Kd = 0.0001;// Constant for Derivative
float leftError = 0; //error is the distance left from the current position to the TV
float leftIntegral = 0;
float leftDerivative = 0;
float leftPrev_error = 0;
float rightError = 0;
float rightIntegral = 0;
float rightDerivative = 0;
float rightPrev_error = 0;
float prev_error = 0; // This is just the previous error before the next loop
float integral = 0; // 
float derivative = 0; // error - prev_error : 
float averagerot = 0; //


double turnKp = 0.875; // Constant for Proportion
double turnKi = 0.0001; // Constant for Integral
double turnKd = 0.00003;// Constant for Derivative
float turnError = 0; //error is the distance left from the current position to the TV
float turnIntegral = 0;
float turnDerivative = 0;
float turnPrev_error = 0;
float turnV = 0; 

event getVar = event();

int ondriver_drivercontrol_0() 
{
  return 0;
}

void VEXcode_driver_task() {
  // Start the driver control tasks....
  vex::task drive0(ondriver_drivercontrol_0);
  while(Competition.isDriverControl() && Competition.isEnabled()) {this_thread::sleep_for(10);}
  drive0.stop();
  return;
}
float speed = 0; // speed

void onevent_getVar_0()
{
  int FLMP = FrontLeft.position(degrees);
  leftError = TV - FLMP;
  if (leftError == 0)
  {
    leftIntegral = 0;
  }
  leftIntegral += leftError;
  leftPrev_error = leftError;
}


void onevent_getVar_1()
{
  int FRMP = FrontRight.position(degrees);
  rightError = TV - FRMP;
  rightIntegral += rightError;
  rightPrev_error = rightError;
}
void onevent_getVar_2()
{
  int averagerot = (FrontRight.position(degrees) + FrontLeft.position(degrees)/2);
  turnError = (turninput - averagerot) * -1;
  
  turnIntegral += turnError;
  turnPrev_error = turnError;
  turnV = turnKp*turnError + turnKi*turnIntegral + turnKd * (turnError - turnPrev_error);
  if (turnV > float(10))
  {
    turnV = 10;
  }
}
void whenstarted1()
{ 
  while (swtch) 
  {
    float rightlateralMotorPower = ((Kp*rightError + Ki*rightIntegral + Kd * (rightError - rightPrev_error))/12);
    float leftlateralMotorPower = ((Kp*leftError + Ki*leftIntegral + Kd * (leftError - leftPrev_error))/12);
    
    if (rightlateralMotorPower > float(10))
    
    {
      rightlateralMotorPower = 10;
      //return 0;
    }


    if (leftlateralMotorPower > float(10))
    {
      leftlateralMotorPower = 10;
      //return 0;
    }
    
    leftside.spin(forward, leftlateralMotorPower + turnV, voltageUnits::volt);
    rightside.spin(forward, rightlateralMotorPower - turnV, voltageUnits::volt);
    //speed = Kp*error + Ki*integral + Kd * derivative;
    //return 0;
  } 

}

int whenStarted2()
{
  while (swtch2)
  {
    getVar.broadcast();
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(turnError);
  }
  return 0;
  
}


int main() {
  getVar(onevent_getVar_0);
  getVar(onevent_getVar_1);
  getVar(onevent_getVar_2);
  vex::competition::bStopTasksBetweenModes = false;
  Competition.drivercontrol(VEXcode_driver_task);
  swtch = true;
  swtch2 = true;
  vex::task ws1(whenStarted2);
  whenstarted1();
}
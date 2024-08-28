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

motor BackRight = motor(PORT7, ratio18_1, false);

inertial Inertial4 = inertial(PORT4);




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
/*    Created:      {date}                                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"




//auton settings
int TV = 1200;
bool swtch = false; // switch to toggle while loop
bool swtch2 = true; // debug switch



// Allows for easier use of the VEX Library
using namespace vex;
competition Competition;

double Kp = 0.9; // Constant for Proportion
double Ki = 0.0001; // Constant for Integral
double Kd = 0.00003;// Constant for Derivative
float error = 0; //error is the distance left from the current position to the TV
float prev_error = 0; // This is just the previous error before the next loop
float integral = 0; // 
float derivative = 0; // error - prev_error : 

event getVar = event();

int ondriver_drivercontrol_0() {
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
  int FRMP = FrontRight.position(degrees);
  int AVGMP = (FLMP + FRMP) / 2;
  error = TV - AVGMP;
  integral += error;
  prev_error = error;
}



void whenstarted1() 
{
  while (swtch) 
  {
    float lateralMotorPower = ((Kp*error + Ki*integral + Kd * derivative)/12);
    FrontLeft.spin(forward, lateralMotorPower, voltageUnits::volt);
    FrontRight.spin(forward, lateralMotorPower, voltageUnits::volt);
    //speed = Kp*error + Ki*integral + Kd * derivative;
  }

}

int whenStarted2()
{
  while (swtch2)
  {
    getVar.broadcast();
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(error);
  }
  return 0;
}


int main() {
  getVar(onevent_getVar_0);
  vex::competition::bStopTasksBetweenModes = false;
  Competition.drivercontrol(VEXcode_driver_task);
  swtch = true;
  swtch2 = true;
  vex::task ws1(whenStarted2);
  whenstarted1();
  
}
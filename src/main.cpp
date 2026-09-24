/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       bdavi                                                     */
/*    Created:      9/10/2026, 6:25:54 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain TimsCrocsBrain;
controller JakesJ0Y;

motor lrMotor(PORT1, ratio18_1, false);
motor rrmotor(PORT2, ratio18_1, false);
motor lfMotor(PORT3, ratio18_1, false);
motor rfMotor(PORT4, ratio18_1, false);

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  lrMotor.setBrake(brake);
  rrmotor.setBrake(brake);
  lfMotor.setBrake(brake);
  rfMotor.setBrake(brake);
}

void stopFunc(void){
  lrMotor.stop();
  rrmotor.stop();
  lfMotor.stop();
  rfMotor.stop();
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    double ForwardSpeed = JakesJ0Y.Axis3.position(percent);
    double TurnSpeed = JakesJ0Y.Axis1.position(percent);

    if(ForwardSpeed < 10 && ForwardSpeed > -10 && TurnSpeed < 10 && TurnSpeed > -10){
      stopFunc();
    } else {
      rfMotor.spin(fwd, ForwardSpeed - TurnSpeed, pct);
      lfMotor.spin(fwd, ForwardSpeed + TurnSpeed, pct); 
      lrMotor.spin(fwd, ForwardSpeed + TurnSpeed, pct);
      rrmotor.spin(fwd, ForwardSpeed - TurnSpeed, pct);
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

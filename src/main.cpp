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
brain TimsCrocksBrain;
controller JakesJ0Y;

motor lrMotor(PORT1, ratio18_1, false);
motor rrmotor(PORT2, ratio18_1, false);
motor lfMotor(PORT3, ratio18_1, false);
motor rfMotor(PORT4, ratio18_1, false);
motor Blahmotor(PORT8, ratio6_1, false);
gps GPS(PORT5);
aivision Vision(PORT6, aivision::ALL_AIOBJS);

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
  while (1) {
    TimsCrocksBrain.Screen.setCursor(1, 1);
    TimsCrocksBrain.Screen.clearLine(1);
    Vision.takeSnapshot(aivision::ALL_AIOBJS);
    if (Vision.objects[0].exists) {
      TimsCrocksBrain.Screen.print("%d", Vision.largestObject.id);
      if(Vision.objects[0].centerX > 180) {
        rfMotor.spin(reverse, 100, pct);
        rrmotor.spin(reverse, 100, pct);
        lfMotor.spin(fwd, 100, pct);
        lrMotor.spin(fwd, 100, pct);
      } else if(Vision.objects[0].centerX < 140) {
        rfMotor.spin(fwd, 100, pct);
        rrmotor.spin(fwd, 100, pct);
        lfMotor.spin(reverse, 100, pct);
        lrMotor.spin(reverse, 100, pct);
      } else if (Vision.objects[0].width < 100) {
        rfMotor.spin(fwd, 100, pct);
        rrmotor.spin(fwd, 100, pct);
        lfMotor.spin(fwd, 100, pct);
        lrMotor.spin(fwd, 100, pct);
      } else {
        stopFunc();
      }
    } else {
      stopFunc();
    }
    wait(50, msec);
  }
}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    double TurnSpeed = JakesJ0Y.Axis1.position(percent);
    double ForwardSpeed = JakesJ0Y.Axis3.position(percent);

    if(ForwardSpeed < 10 && ForwardSpeed > -10 && TurnSpeed < 10 && TurnSpeed > -10){
      stopFunc();
    } else {
      rfMotor.spin(fwd, TurnSpeed - ForwardSpeed, pct);
      lfMotor.spin(fwd, TurnSpeed + ForwardSpeed, pct);
      lrMotor.spin(fwd, TurnSpeed + ForwardSpeed, pct);
      rrmotor.spin(fwd, TurnSpeed - ForwardSpeed, pct);
    }

    if(JakesJ0Y.ButtonR1.pressing()){
      Blahmotor.spin(fwd, 100, pct);
    } else if(JakesJ0Y.ButtonL1.pressing()){
      Blahmotor.spin(reverse, 100, pct);
    } else {
      Blahmotor.stop();
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

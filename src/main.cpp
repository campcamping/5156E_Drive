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
motor IntakeMotor(PORT8, ratio36_1, false);
motor LiftMotor1(PORT9, ratio6_1, true);
motor LiftMotor2(PORT13, ratio6_1, false);
motor_group LiftMotor(LiftMotor1, LiftMotor2);
motor ClawSpinMotor(PORT6, ratio18_1, false);
motor ClawMotor(PORT7, ratio18_1, false);
aivision Vision(PORT12, aivision::ALL_AIOBJS);
  
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
  LiftMotor1.setBrake(hold);
  LiftMotor2.setBrake(hold);
  ClawSpinMotor.setBrake(hold);
  ClawMotor.setBrake(brake);
  IntakeMotor.setBrake(brake);
  ClawSpinMotor.setPosition(100, degrees);
}

void stopFunc(void){
  lrMotor.stop();
  rrmotor.stop();
  lfMotor.stop();
  rfMotor.stop();
}

void autonomous(void) {
  rfMotor.spinFor(fwd, 1, turns, false);
  lfMotor.spinFor(fwd, 1, turns, false);
  lrMotor.spinFor(fwd, 1, turns, false);
  rrmotor.spinFor(fwd, 1, turns, false);
  /*while (1) {
    TimsCrocksBrain.Screen.setCursor(1, 1);
    TimsCrocksBrain.Screen.clearLine(1);
    Vision.takeSnapshot(aivision::ALL_AIOBJS);
    if (Vision.objects[0].exists) {
      if (Vision.objects[0].id == 1) {
        TimsCrocksBrain.Screen.print("Yellow-Blue Pin Detected");
        if(Vision.objects[0].centerX > 180) {
         FrontRight.spin(reverse, 15, pct);
         FrontLeft.spin(fwd, 15, pct);
        } else if(Vision.objects[0].centerX < 140) {
         FrontRight.spin(fwd, 15, pct);
         FrontLeft.spin(reverse, 15, pct);
        } else if (Vision.objects[0].width < 150) {
          FrontRight.spin(fwd, 15, pct);
          FrontLeft.spin(fwd, 15, pct);
        } else {
          stopFunc();
        }
      }
    } else {
      stopFunc();
    }
    wait(50, msec);
  }*/
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

    if(JakesJ0Y.ButtonL1.pressing()){
      IntakeMotor.spin(fwd, 100, pct);
    } else if(JakesJ0Y.ButtonL2.pressing()){
      IntakeMotor.spin(reverse, 100, pct);
    } else {
      IntakeMotor.stop();
    }

    if(JakesJ0Y.ButtonUp.pressing()) {
      LiftMotor.spin(fwd, 100, pct);
    } else if(JakesJ0Y.ButtonDown.pressing()) {
      LiftMotor.spin(reverse, 100, pct);
    } else {
      LiftMotor.stop();
    }

    if(JakesJ0Y.ButtonRight.pressing()) {
      ClawSpinMotor.spin(fwd, 100, pct);
    } else if (JakesJ0Y.ButtonLeft.pressing()) {
      ClawSpinMotor.spin(, 100, pct);
    } else {
      ClawSpinMotor.stop();
    }

    if(JakesJ0Y.ButtonR2.pressing()) {
      ClawMotor.spin(fwd, 100, pct);
    } else if (JakesJ0Y.ButtonR1.pressing()) {
      ClawMotor.spin(reverse, 100, pct);    
    } else {
      ClawMotor.stop();
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

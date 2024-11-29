#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;
RightFront = motor(PORT8, ratio6_1, false);
RightMiddle = motor(PORT9, ratio6_1, false);
RightBack = motor(PORT10, ratio6_1, false);
LeftFront = motor(PORT1, ratio6_1, true);
LeftMiddle = motor(PORT2, ratio6_1, true);
LeftBack = motor(PORT3, ratio6_1, true);
IntakeRight = motor(PORT6, ratio6_1, false);
IntakeLeft = motor(PORT7, ratio6_1, false)
PistonMogo = digital_out(Brain.ThreeWirePort.A);
PistonWall = digital_out(Brain.ThreeWirePort.C);
PistonDoinker = digital_out(Brain.ThreeWirePort.B);
PistonClaw = digital_out(Brain.ThreeWirePort.D);
OpticalSensor = optical(PORT5);
//The motor constructor takes motors as (port, ratio, reversed), so for example
//motor LeftFront = motor(PORT1, ratio6_1, false);

//Add your devices below, and don't forget to do the same in robot-config.h:


void vexcodeInit( void ) {
  // nothing to initialize
}
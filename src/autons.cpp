#include "vex.h"

/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

bool isSorting = false; // To prevent multiple interruptions at once

int colorSortingTask() {
    while (true) {
        // Enable the optical sensor light
        OpticalSensor.setLightPower(100, percent);
        OpticalSensor.setLight(ledState::on);

        // Detect a ring (wrong color)
        if (!isSorting && OpticalSensor.color() != color::red && OpticalSensor.isNearObject()) {
            isSorting = true;

            // Save current motion (if required)
            double leftMotorSpeed = LeftDrive.velocity(percent);
            double rightMotorSpeed = RightDrive.velocity(percent);

            // Stop all robot movement
            /* LeftDrive.stop();
            RightDrive.stop();
            IntakeLeft.stop();
            IntakeRight.stop();
            */
            // Perform ejection
           //  PistonWall.set(true); // Extend piston
           /* wait(1, seconds);
            
            wait(1, seconds); // Wait to eject ring
            */
            // Stop intake motors and retract piston
            IntakeLeft.spin(reverse, 200, velocityUnits::pct);
            IntakeRight.spin(reverse, 200, velocityUnits::pct);
            // PistonWall.set(false);
            wait(0.5, seconds);
            // Resume previous motion
            LeftDrive.spin(forward, leftMotorSpeed, percent);
            RightDrive.spin(forward, rightMotorSpeed, percent);

            isSorting = false; // Mark sorting as complete
        }

        // Prevent CPU overload
        wait(20, msec);
    }
    return 0;
}

double inchesToRotations(double inches) {
    const double wheelDiameter = 2.75; // Diameter of the wheel in inches
    const double wheelCircumference = wheelDiameter * 3.141592653; // Circumference of the wheel
    return inches / wheelCircumference;
}

double inchesToDegrees(double inches) {
    return inchesToRotations(inches) * 360; // 1 rotation = 360 degrees
}

// Wrapper functions for autonomous movement
void driveInches(double inches, int speed) {
    double rotations = inchesToRotations(inches);
    chassis.driveFor(rotations, rotationUnits::rev, speed, velocityUnits::pct);
}

void turnDegrees(double degrees, int speed) {
    chassis.turnToAngle(degrees, rotationUnits::deg, speed, velocityUnits::pct);
}


/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */
// VERY IMPORTANT. COPY THIS FOR ALL CODE
void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

/**
 * The expected behavior is to return to the start position.
 */

void drive_test(){
  chassis.drive_distance(6);
  chassis.drive_distance(12);
  chassis.drive_distance(18);
  chassis.drive_distance(-36);
}

/**
 * The expected behavior is to return to the start angle, after making a complete turn.
 */

void turn_test(){
  chassis.turn_to_angle(5);
  chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(225);
  chassis.turn_to_angle(0);
}

/**
 * Should swing in a fun S shape.
 */

void swing_test(){
  chassis.left_swing_to_angle(90);
  chassis.right_swing_to_angle(0);
}

/**
 * A little of this, a little of that; it should end roughly where it started.
 */

void full_test(){
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}

/**
 * Doesn't drive the robot, but just prints coordinates to the Brain screen 
 * so you can check if they are accurate to life. Push the robot around and
 * see if the coordinates increase like you'd expect.
 */

void odom_test(){
  chassis.set_coordinates(0, 0, 0);
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5,80, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(5,100, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    task::sleep(20);
  }
}

/**
 * Should end in the same place it began, but the second movement
 * will be curved while the first is straight.
 */

void tank_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.turn_to_point(24, 24);
  chassis.drive_to_point(24,24);
  chassis.drive_to_point(0,0);
  chassis.turn_to_angle(0);
}

/**
 * Drives in a square while making a full turn in the process. Should
 * end where it started.
 */

void holonomic_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.holonomic_drive_to_pose(0, 18, 90);
  chassis.holonomic_drive_to_pose(18, 0, 180);
  chassis.holonomic_drive_to_pose(0, 18, 270);
  chassis.holonomic_drive_to_pose(0, 0, 0);
}

void practice_test(){
  if (autonStep == 0){
    chassis.left_swing_to_angle(90);
    autonStep++;
  } else if(autonStep == 1){
    driveInches(10,)
    autonStep++;
  } else if(autonStep == 2){
    turnDegrees(225,50);
    autonStep++;
  }
}

void blue_ringside(){
  if (autonStep == 0){
    driveInches(-30,75);
    PistonMogo.set(false);
    PistonWall.set(true);
    wait(100, msec);
    autonStep++;
  } else if(autonStep == 1){
    turnDegrees(-45, 50);
    wait(100, msec);
    autonStep++;
  } else if(autonStep == 2){
    driveInches(-7,20);
    wait(100, msec);
    driveInches(-3,45);
    PistonMogo.set(true);
    autonStep++;
  } else if(autonStep == 2){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    autonStep++;
  } else if(autonStep == 3){
    turnDegrees(-90,30);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 4){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(30,25);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 5){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    turnDegrees(20,20);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 6){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(5,30);
    autonStep++;
  } else if(autonStep == 7){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(-5,30);
    autonStep++;
  } else if(autonStep == 8){
    turnDegrees(80,20);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 9){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(40,50);
    wait(200,msec);
    autonStep++;
  } else if(autonStep == 10){
    PistonDoinker.set(true);
    turnDegrees(10,25);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 11){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    driveInches(70, 50);
    autonStep++;
    wait(200, msec);
  } else if (autonStep == 12){
    turnDegrees(180,50);
    wait(100,msec);
    autonStep++;
  } else if (autonStep == 13){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    PistonDoinker.set(false);
    wait(100,msec);
    autonStep++;
  } else if (autonStep == 14){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    degreesTurn(-30,40);
    wait(50,msec);
    autonStep++;
  } else if (autonStep == 15){
    driveInches(60,80);
    wait(100, msec);
  }
}

void red_ringside(){
  if (autonStep == 0){
    driveInches(-30,75);
    PistonMogo.set(false);
    PistonWall.set(true);
    wait(100, msec);
    autonStep++;
  } else if(autonStep == 1){
    turnDegrees(45, 50);
    wait(100, msec);
    autonStep++;
  } else if(autonStep == 2){
    driveInches(-7,20);
    wait(100, msec);
    driveInches(-3,45);
    PistonMogo.set(true);
    autonStep++;
  } else if(autonStep == 2){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    autonStep++;
  } else if(autonStep == 3){
    turnDegrees(90,20);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 4){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(30,25);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 5){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    turnDegrees(-20,20);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 6){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(5,30);
    autonStep++;
  } else if(autonStep == 7){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(-5,30);
    autonStep++;
  } else if(autonStep == 8){
    turnDegrees(-80,20);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 9){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    driveInches(40,60);
    wait(200,msec);
    autonStep++;
  } else if(autonStep == 10){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct); 
    PistonDoinker.set(true);
    turnDegrees(-15,25);
    wait(100,msec);
    autonStep++;
  } else if(autonStep == 11){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    driveInches(60, 50);
    autonStep++;
    wait(200, msec);
  } else if (autonStep == 12){
    turnDegrees(180,50);
    wait(100,msec);
    autonStep++;
  } else if (autonStep == 13){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    PistonDoinker.set(false);
    wait(100,msec);
    autonStep++;
  } else if (autonStep == 14){
    IntakeLeft.spin(forward, 10000, velocityUnits::pct);
    IntakeRight.spin(forward, 10000, velocityUnits::pct);
    degreesTurn(30,40);
    wait(50,msec);
    autonStep++;
  } else if (autonStep == 15){
    driveInches(60,80);
    wait(100, msec);
  }
}

void blue_goalside(){

}

void red_ringside(){

}
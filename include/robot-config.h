using namespace vex;

extern brain Brain;
extern controller Controller;
extern motor LeftFront;
extern motor LeftMiddle;
extern motor LeftBack;
extern motor RightFront;
extern motor RightMiddle;
extern motor RightBack;
extern motor IntakeRight;
extern motor IntakeLeft;
extern digital_out PistonMogo;
extern digital_out PistonCorner;
extern digital_out PistonWall;
extern digital_out PistonClaw;
extern optical OpticalSensor;


//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:



void  vexcodeInit( void );
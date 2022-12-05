#include "mbed.h"
#include <chrono>

// the wheel speed
const float wheelSpeed = .9;

chrono::milliseconds turn = 200ms;
chrono::milliseconds _180 = 400ms;
chrono::milliseconds on = 420ms;

//Trigger for scanner
DigitalOut trig(D8);

//input for scanner
DigitalIn echo(D3);

// digital outputs for the wheels
DigitalOut rightFwdDrive(D5), rightBackDrive(D4), leftFwdDrive(D7),
    leftBackDrive(D6);

//Display for Y
BusOut displayY(PC_3,PC_2,PH_1,PH_0,
PC_15,PC_14,PC_13);

//Display for X 
BusOut displayX(PC_4,PB_13,PB_14,PB_15,
PB_1,PB_2,PB_12);

// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);

// ticker that movement instructions are sent to
Ticker movement;

// ticker that controls display
Ticker display;

//scanning for walls
Ticker scan;

// is there a wall in front of the robot
bool wall = false;

// x,y are the robot starting position
// goalX,goalY are the goal position
int x = 0, y = 0, goalX = 0, goalY = 5;

// the robot's current ready state
int i = 0;


// function that displays position that the robot is in
void displayNode();


// function that scans for walls that robot must avoid
void scanWall();

// function that rotates the robot counter clockwise
void turnLeft();

// function that rotates the robot clockwise
void turnRight();

// function that moves the robot forward
void MoveFwd();

void Dance();

// possible orientations of the robot
enum cardinalDirection { north, south, east, west };

// the robot's current orientation
cardinalDirection face = north;

int main() {
  rightWheelSpeed.period(0.2);
  leftWheelSpeed.period(0.1);
  rightFwdDrive = leftBackDrive = leftFwdDrive = rightBackDrive = 0;
  rightWheelSpeed = leftWheelSpeed = wheelSpeed;
  display.attach(&displayNode,100ms);
  scan.attach(&scanWall,100ms);
  movement.attach(&MoveFwd, on);
  while (1) {
  }
}

void MoveFwd() {
  rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
  rightWheelSpeed.period(0.2);
  leftWheelSpeed.period(0.1);
  // if the robot is not at the goal position
  if (goalX != x || goalY != y) {
    if (face == north) {
      if (y < goalY && wall == false) {
        rightFwdDrive = leftFwdDrive = 1;
        rightBackDrive = leftBackDrive = 0;
        y++;
      } else if (y > goalY) {
        movement.attach(&turnRight,_180);
      } else if (x < goalX)
        movement.attach(&turnRight, turn);
      else if (x > goalX)
        movement.attach(&turnLeft, turn);
    }

    else if (face == south) {
      if (y < goalY && wall == false) {
        rightBackDrive = leftBackDrive = 1;
        rightFwdDrive = leftFwdDrive = 0;
        y--;
      } else if (y > goalY) {
        movement.attach(&turnRight,_180);
      } else if (x < goalX)
        movement.attach(&turnLeft, turn);
      else if (x > goalX)
        movement.attach(&turnRight, turn);
    }

    else if (face == east) {
      if (x < goalX && wall == false) {
        rightFwdDrive = leftFwdDrive = 0;
        rightBackDrive = leftBackDrive = 1;
        x++;
      } else if (x > goalX) {
        movement.attach(&turnRight,_180);
      } else if (y < goalY)
        movement.attach(&turnLeft, turn);
      else if (y > goalY)
        movement.attach(&turnRight, turn);
    }

    else if (face == west) {
      if (x < goalX && wall == false) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
        x++;
      } else if (x > goalX) {
        movement.attach(&turnRight,_180);
      } else if (y < goalY)
        movement.attach(&turnRight, turn);
      else if (y > goalY)
        movement.attach(&turnLeft, turn);
    } 
  }
  else if (goalX == x && goalY == y) {
    leftFwdDrive = rightFwdDrive =
    leftBackDrive = rightBackDrive = 0;
    Dance();
    movement.detach();
  }
}

void Dance(){
  rightBackDrive = leftFwdDrive = 1;
  rightFwdDrive = leftBackDrive = 0;
  wait_us(6000000);
  rightFwdDrive = leftBackDrive = 1;
  rightBackDrive = leftFwdDrive = 0;
  wait_us(6000000);
  rightFwdDrive = leftBackDrive = 0;
  rightBackDrive = leftFwdDrive = 0;
  movement.detach();
}
void turnLeft() {
  rightWheelSpeed.period(0.1);
  leftWheelSpeed.period(0.1);
  rightBackDrive = leftFwdDrive = 1;
  rightFwdDrive = leftBackDrive = 0;
  if (i == 0) {
    switch (face) {
    case north:face = west;break;
    case south:face = east;break;
    case west:face = south;break;
    case east:face = north;break;
    }
    i++;
  } 
  else {
    i = 0;
    wall = false;
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
    movement.attach(&MoveFwd, on);
  }
}
void turnRight() {
  rightWheelSpeed.period(0.1);
  leftWheelSpeed.period(0.1);
  rightFwdDrive = leftBackDrive = 1;
  rightBackDrive = leftFwdDrive = 0;
  if (i == 0) {
    switch (face) {
    case north:face = west;break;
    case south:face = east;break;
    case west:face = south;break;
    case east:face = north;break;
    }
    i++;
  } 
  else {
    i = 0;
    wall = false;
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
    movement.attach(&MoveFwd, on);
  }
}

void displayNode(){
    switch (x) {
        case 0:displayX = 0;break;
        case 1:displayX = 1;break;
        case 2:displayX = 2;break;
        case 3:displayX = 3;break;
        case 4:displayX = 4;break;
        case 5:displayX = 5;break;
        case 6:displayX = 6;break;
        default:displayX=12;break;
    }
    switch (y) {
        case 0:displayY = 0;break;
        case 1:displayY = 1;break;
        case 2:displayY = 2;break;
        case 3:displayY = 3;break;
        case 4:displayY = 4;break;
        case 5:displayY = 5;break;
        case 6:displayY = 6;break;
        default:displayY=12;break;
    }
}

void scanWall(){
    trig = 1;
    wait_us(10);
    trig = 0;
    wait_us(1000);
    if(echo.read() == 1) wall = true;
}
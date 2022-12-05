#include "mbed.h"
#include <chrono>

// the wheel speed
const float wheelSpeed = .9;

chrono::milliseconds turn = 200ms;
chrono::milliseconds on = 420ms;

// digital outputs for the wheels
DigitalOut rightFwdDrive(D5), rightBackDrive(D4), leftFwdDrive(D7),
    leftBackDrive(D6);

//Display for Y
BusOut displayY(PC_3,PC_2,PH_1,PH_0,
PC_15, PC_14,PC_13);

//Display for X 
BusOut displayX();

// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);

// ticker that movement instructions are sent to
Ticker movement;

// is there a wall in front of the robot
bool wall = false;

// x,y are the robot starting position
// goalX,goalY are the goal position
int x = 0, y = 0, goalX = 0, goalY = 5;

// the robot's current ready state
int i = 0;


// function that displays position that the robot is in
void displayNode(int num, BusOut dis);


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
      if (y < goalY) {
        rightFwdDrive = leftFwdDrive = 1;
        rightBackDrive = leftBackDrive = 0;
        y++;
      } else if (y > goalY) {
        rightBackDrive = leftBackDrive = 1;
        rightFwdDrive = leftFwdDrive = 0;
        y--;
      } else if (x < goalX)
        movement.attach(&turnRight, turn);
      else if (x > goalX)
        movement.attach(&turnLeft, turn);
    }

    else if (face == south) {
      if (y < goalY) {
        rightBackDrive = leftBackDrive = 1;
        rightFwdDrive = leftFwdDrive = 0;
        y++;
      } else if (y > goalY) {
        rightFwdDrive = leftFwdDrive = 1;
        rightBackDrive = leftBackDrive = 0;
        y--;
      } else if (x < goalX)
        movement.attach(&turnLeft, turn);
      else if (x > goalX)
        movement.attach(&turnRight, turn);
    }

    else if (face == east) {
      if (x < goalX) {
        rightFwdDrive = leftFwdDrive = 0;
        rightBackDrive = leftBackDrive = 1;
        x++;
      } else if (x > goalX) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
        x--;
      } else if (y < goalY)
        movement.attach(&turnLeft, turn);
      else if (y > goalY)
        movement.attach(&turnRight, turn);
    }

    else if (face == west) {
      if (x < goalX) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
        x++;
      } else if (x > goalX) {
        rightFwdDrive = leftFwdDrive = 0;
        rightBackDrive = leftBackDrive = 1;
        x--;
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
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
    movement.attach(&MoveFwd, on);
  }
}

void displayNode(int num, BusOut dis){
    switch (num) {
        case 0:dis = 0;break;
        case 1:dis = 1;break;
        case 2:dis = 2;break;
        case 3:dis = 3;break;
        case 4:dis = 4;break;
        case 5:dis = 5;break;
        case 6:dis = 6;break;
        default:dis =12;break;
    }
}

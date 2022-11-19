#include "mbed.h"
#include <chrono>

// the wheel speed
const float wheelSpeed = .9;

chrono::milliseconds turn = 160ms;
chrono::milliseconds on = 420ms;

// digital outputs for the wheels
DigitalOut rightFwdDrive(D7), rightBackDrive(D6), leftFwdDrive(D5),
    leftBackDrive(D4);

// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);

// ticker that movement instructions are sent to
Ticker movement;

// x,y are the robot starting position
// goalX,goalY are the goal position
int x = 0, y = 0, goalX = 2, goalY = 2;

// the robot's current ready state
int i = 0;

// function that rotates the robot counter clockwise
void turnLeft();

// function that rotates the robot clockwise
void turnRight();

// function that moves the robot forward
void MoveFwd();

// possible orientations of the robot
enum cardinalDirection { north, south, east, west };

// the robot's current orientation
cardinalDirection face = north;

int main() {
  rightFwdDrive = leftBackDrive = leftFwdDrive = rightBackDrive = 0;
  rightWheelSpeed = leftWheelSpeed = wheelSpeed;
  rightWheelSpeed.period(0.1);
  leftWheelSpeed.period(0.1);
  movement.attach(&MoveFwd, on);
  while (1) {
  }
}

void MoveFwd() {
  rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
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
    rightFwdDrive = rightBackDrive = leftFwdDrive = leftBackDrive = 0;
    goalX=0;goalY=0;
    if (x != goalX&& y!=goalY){
        movement.attach(&turnRight, turn);
    }
    else{ leftFwdDrive = rightFwdDrive =
    leftBackDrive = rightBackDrive = 0;
    movement.detach();}
  }
}
void turnLeft() {
  rightFwdDrive = leftBackDrive = 1;
  rightBackDrive = leftFwdDrive = 0;
  if (i == 0) {
    switch (face) {
    case north:
      face = west;
      break;
    case south:
      face = east;
      break;
    case west:
      face = south;
      break;
    case east:
      face = north;
      break;
    }
    i++;
  } else {
    i = 0;
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
    movement.attach(&MoveFwd, on);
  }
}
void turnRight() {
  rightBackDrive = leftFwdDrive = 1;
  rightFwdDrive = leftBackDrive = 0;
  if (i == 0) {
    switch (face) {
    case north:
      face = west;
      break;
    case south:
      face = east;
      break;
    case west:
      face = south;
      break;
    case east:
      face = north;
      break;
    }
    i++;
  } else {
    i = 0;
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive =0;
    movement.attach(&MoveFwd, on);
  }
}

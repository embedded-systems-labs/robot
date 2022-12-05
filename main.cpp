#include "mbed.h"
#include <chrono>

// the wheel speed
const float wheelSpeed = .9;

chrono::milliseconds turn = 200ms;
chrono::milliseconds _180 = 400ms;
chrono::milliseconds on = 460ms;

// digital outputs for the wheels
DigitalOut rightFwdDrive(D5), rightBackDrive(D4), leftFwdDrive(D7),
    leftBackDrive(D6);

DigitalOut led(D2);
// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);

DigitalOut trig(D10);DigitalIn echo(D8);
// ticker that movement instructions are sent to
Ticker movement;

// is there a wall in front of the robot
bool wall = false;

// x,y are the robot starting position
// goalX,goalY are the goal position
int x = 0, y = 0, goalX = 3, goalY = 5;

// the robot's current ready state
int i = 0;

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
  trig = 0;
  
  movement.attach(&MoveFwd, on);
  while (1) {
    trig =1;
    wait_us(10);
    trig=0;
    wait_us(2000);
    if (echo.read() >= 1){
      wall = false; 
      led = 1;
    }
    else {
      wall = true;
      led = 0;
    }
  wait_us(10000);
  }
}

void MoveFwd() {
  rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
  rightWheelSpeed = leftWheelSpeed = wheelSpeed;
  rightWheelSpeed.period(0.2);
  leftWheelSpeed.period(0.2);
  // if the robot is not at the goal position

  if (goalX != x || goalY != y) {
    if (face == north) {
      if (y < goalY && wall == false) {
        rightFwdDrive = leftFwdDrive = 1;
        rightBackDrive = leftBackDrive = 0;
        y++;
      } else if (y > goalY) {
        movement.attach(&turnRight, _180);
      } else if (x < goalX)
        movement.attach(&turnRight, turn);
      else if (x > goalX)
        movement.attach(&turnLeft, turn);
    }

    else if (face == south) {
      if (y < goalY && wall == false) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
        y--;
      } else if (y > goalY) {
        movement.attach(&turnRight, _180);
      } else if (x < goalX)
        movement.attach(&turnLeft, turn);
      else if (x > goalX)
        movement.attach(&turnRight, turn);
    }

    else if (face == east) {
      if (x < goalX && wall == false) {
        rightFwdDrive = leftFwdDrive = 1;
        rightBackDrive = leftBackDrive = 0;
        x++;
      } else if (x > goalX) {
        movement.attach(&turnRight, _180);
      } else if (y < goalY)
        movement.attach(&turnLeft, turn);
      else if (y > goalY)
        movement.attach(&turnRight, turn);
    }

    else if (face == west) {
      if (x < goalX && wall == false) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
        x--;
      } else if (x > goalX) {
        movement.attach(&turnRight, _180);
      } else if (y < goalY)
        movement.attach(&turnRight, turn);
      else if (y > goalY)
        movement.attach(&turnLeft, turn);
    }
  } else{
    leftFwdDrive = rightFwdDrive = leftBackDrive = rightBackDrive = 0;
    movement.attach(&Dance, 2000ms);
  }
}

void Dance() {
  rightBackDrive = leftFwdDrive = 1;
  rightFwdDrive = leftBackDrive = 0;
  wait_us(2000000);
  rightFwdDrive = leftBackDrive = 1;
  rightBackDrive = leftFwdDrive = 0;
  wait_us(2000000);
  rightFwdDrive = leftBackDrive = 0;
  rightBackDrive = leftFwdDrive = 0;
  movement.detach();
}
void turnLeft() {
  rightWheelSpeed.period(0.1);
  leftWheelSpeed.period(0.1);
  rightBackDrive = leftFwdDrive = 0;
  rightFwdDrive = leftBackDrive = 1;
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
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
    movement.attach(&MoveFwd, on);
  }
}
void turnRight() {
  rightWheelSpeed.period(0.1);
  leftWheelSpeed.period(0.1);
  rightFwdDrive = leftBackDrive = 0;
  rightBackDrive = leftFwdDrive = 1;
  if (i == 0) {
    switch (face) {
    case north:face = east;break;
    case south:face = west;break;
    case west:face = north;break;
    case east:face = south;break;
    }
    i++;
  }
  else {
    i = 0;
    wall = false;
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
    movement.attach(&MoveFwd, on);
  }
}
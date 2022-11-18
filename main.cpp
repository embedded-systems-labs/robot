#include "mbed.h"

//the wheel speed
const float wheelSpeed = 0.1;

// digital outputs for the wheels
DigitalOut rightFwdDrive(D7), leftBackDrive(D6),
    leftFwdDrive(D5), leftBackDrive(D4);
rightFwdDrive = leftBackDrive = leftFwdDrive = leftBackDrive = 0;
// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);
rightWheelSpeed = leftWheelSpeed = wheelSpeed;
rightWheelSpeed.period(0.5);
leftWheelSpeed.period(0.5);
// ticker that movement instructions are sent to
Ticker movement;

// x,y are the robot starting position
// goalX,goalY are the goal position
char x = 0, y = 0, goalX = 2, goalY = 2;

// the robot's current ready state
int i = 0;

// function that rotates the robot counter clockwise
void turnLeft();

// function that rotates the robot clockwise
void turnRight();

// function that moves the robot forward
void MoveFwd();

// possible orientations of the robot
enum cardinalDirection
{
    north,
    south,
    east,
    west
};

// the robot's current orientation
cardinalDirection face = north;


int main()
{
    movement.attach(&MoveFwd, 1000ms);
}

void MoveFwd()
{
    // if the robot is not at the goal position
    if (goalX != x || goalY != y)
    {
        if (face == north)
        {
            if (y < goalY)
            {
                rightFwdDrive = leftFwdDrive = 1;
                leftBackDrive = leftBackDrive = 0;
                y++;
            }
            else if (y > goalY)
            {
                leftBackDrive = leftBackDrive = 1;
                rightFwdDrive = leftFwdDrive = 0;
                y--;
            }
            else if (x < goalX)
                movement.attach(&turnRight, 666ms);
            else if (x > goalX)
                movement.attach(&turnLeft, 666ms);
        }

        else if (face == south)
        {
            if (y < goalY)
            {
                leftBackDrive = leftBackDrive = 1;
                rightFwdDrive = leftFwdDrive = 0;
                y++;
            }
            else if (y > goalY)
            {
                rightFwdDrive = leftFwdDrive = 1;
                leftBackDrive = leftBackDrive = 0;
                y--;
            }
            else if (x < goalX)
                movement.attach(&turnLeft, 666ms);
            else if (x > goalX)
                movement.attach(&turnRight, 666ms);
        }

        else if (face == east)
        {
            if (x < goalX)
            {
                rightFwdDrive = leftFwdDrive = 1;
                leftBackDrive = leftBackDrive = 0;
                x++;
            }
            else if (x > goalX)
            {
                leftBackDrive = leftBackDrive = 1;
                rightFwdDrive = leftFwdDrive = 0;
                x--;
            }
            else if (y < goalY)
                movement.attach(&turnLeft, 666ms);
            else if (y > goalY)
                movement.attach(&turnRight, 666ms);
        }

        else if (face == west)
        {
            if (x < goalX)
            {
                leftBackDrive = leftBackDrive = 1;
                rightFwdDrive = leftFwdDrive = 0;
                x++;
            }
            else if (x > goalX)
            {
                rightFwdDrive = leftFwdDrive = 1;
                leftBackDrive = leftBackDrive = 0;
                x--;
            }
            else if (y < goalY)
                movement.attach(&turnRight, 666ms);
            else if (y > goalY)
                movement.attach(&turnLeft, 666ms);
        }
    }
    else if (goalX == x && goalY == y)
    {
        rightFwdDrive = leftBackDrive = leftFwdDrive = leftBackDrive = 0;
        movement.detach();
    }
}
void turnLeft()
{
    rightFwdDrive = leftBackDrive = 1;
    leftBackDrive = leftFwdDrive = 0;
    if (i == 0)
    {
        switch (face)
        {
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
    }
    else
    {
        i = 0;
        movement.attach(&MoveFwd, 1000ms);
    }
}
void turnRight()
{
    leftBackDrive = leftFwdDrive = 1;
    rightFwdDrive = leftBackDrive = 0;
    if (i == 0)
    {
        switch (face)
        {
        case north:
            face = east;
            break;
        case south:
            face = west;
            break;
        case west:
            face = north;
            break;
        case east:
            face = south;
            break;
        }
        i++;
    }
    else
    {
        i = 0;
        movement.attach(&MoveFwd, 1000ms);
    }
}
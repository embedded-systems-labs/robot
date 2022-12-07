#include "mbed.h"
#include "nRF24L01P.h"
#include <chrono>
#include <cstdio>


// the wheel speed
const float wheelSpeed = .6;

AnalogOut wallCh(A2);

chrono::milliseconds turn = 200ms;
chrono::milliseconds _180 = 400ms;
chrono::milliseconds on = 500ms;

nRF24L01P my_nrf24l01p(D11, D12, D13, D15, D14,
                       D2); // mosi, miso, sck, csn, ce, irq

int TrCounter = 0;

// digital outputs for the wheels
DigitalOut rightFwdDrive(D5), rightBackDrive(D4), leftFwdDrive(D7),
    leftBackDrive(D6);

// digital outputs for the Pulse Width Modulation controlling wheel speed
PwmOut rightWheelSpeed(D9), leftWheelSpeed(D3);

DigitalOut trig(D10);
AnalogIn echo(A0);
// ticker that movement instructions are sent to
Ticker movement;

// is there a wall in front of the robot
bool wall = false;

// x,y are the robot starting position
// goalX,goalY are the goal position
int x = 0, y = 0, goalX = 0, goalY = 0;

// the robot's current ready state
int i = 0;
// function that rotates the robot counter clockwise
void turnLeft();

// function that rotates the robot clockwise
void turnRight();

// function that moves the robot forward
void MoveFwd();

void Dance();

void scanning();

// possible orientations of the robot
enum cardinalDirection { north, south, east, west };

// the robot's current orientation
cardinalDirection face = north;

int main() {

#define Transfer_Size 3
  char rxData[Transfer_Size];
  int rxDataCnt = 0;
  my_nrf24l01p.powerUp();
  my_nrf24l01p.setRfOutputPower(-6);
  my_nrf24l01p.setRxAddress((0x1F22676D90), DEFAULT_NRF24L01P_ADDRESS_WIDTH);
  my_nrf24l01p.setAirDataRate(2000);
  printf("nRF24L01+ Frequency    : %d MHz\r\n", my_nrf24l01p.getRfFrequency());
  printf("nRF24L01+ Output power : %d dBm\r\n",
         my_nrf24l01p.getRfOutputPower());
  printf("nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate());
  printf("nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress());
  my_nrf24l01p.setTransferSize(Transfer_Size);
  my_nrf24l01p.setReceiveMode();
  my_nrf24l01p.enable();
  trig = 0;
  rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
  while (1) {
    rightWheelSpeed.period(1.6);
    leftWheelSpeed.period(1.6);

    rightWheelSpeed = leftWheelSpeed = wheelSpeed;
    if (my_nrf24l01p.readable()) {
      my_nrf24l01p.read(NRF24L01P_PIPE_P0, rxData, sizeof(rxData));
      printf("x: %d\n", rxData[2]);
      printf("y: %d\n", rxData[1]);
      printf("start, %d\n", rxData[0]);
      if (rxData[0] == 1) {
        goalX = rxData[2];
        goalY = rxData[1];
        rxData[0] = 0;
        x = y = 0;
        face = north;
        wall = false;
        movement.attach(&MoveFwd, on);
      }
    }
    trig = 1;
    wait_us(10);
    trig = 0;
    wait_us(1000);
    if (echo <= 0.6f) {
      wall = false;
      wallCh = 0;
    } else {
      wall = false;
      wallCh = 1;
    }
    wait_us(10000);
  }
}

void MoveFwd() {

  // if the robot is not at the goal position
  if (goalX != x || goalY != y) {
    if (face == north) {
      if (y < goalY && wall == false) {
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
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
        rightBackDrive = leftBackDrive = 0;
        rightFwdDrive = leftFwdDrive = 1;
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
  } else {
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
  x = y = goalX = goalY = 0;
  movement.detach();
}

void turnLeft() {
  rightBackDrive = leftFwdDrive = 0;
  rightFwdDrive = leftBackDrive = 1;
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
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
    i = 0;
    movement.attach(&MoveFwd, on);
  }
}

void turnRight() {
  rightFwdDrive = leftBackDrive = 0;
  rightBackDrive = leftFwdDrive = 1;
  if (i == 0) {
    switch (face) {
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
  } else {
    rightBackDrive = rightFwdDrive = leftBackDrive = leftFwdDrive = 0;
    i = 0;
    movement.attach(&MoveFwd, on);
  }
}
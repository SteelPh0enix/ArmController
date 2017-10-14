#include "MotorDriver.hpp"

MC33926MotorDriver::MC33926MotorDriver()
    : _initialized(false), _pinsSet(false), _speed(0) {}

MC33926MotorDriver::MC33926MotorDriver(const Pins& pins, bool init)
    : _initialized(false), _pinsSet(true), _pins(pins), _speed(0) {
  if (init) initialize();
}

bool MC33926MotorDriver::initialize() {
  if (!pinsSet()) return false;
  if (isInitialized()) return true;
  pinMode(_pins.directory, OUTPUT);
  pinMode(_pins.pwm, OUTPUT);
  pinMode(_pins.feedback, INPUT);
  pinMode(_pins.nD2, OUTPUT);
  pinMode(_pins.nSF, INPUT);

  digitalWrite(_pins.nD2, HIGH);
  setSpeed(_speed);

  _initialized = true;
  return true;
}

bool MC33926MotorDriver::initialize(const Pins& pins) {
  if (isInitialized()) return true;
  _pins = pins;
  _pinsSet = true;
  return initialize();
}

void MC33926MotorDriver::setSpeed(int speed) {
  if (!isInitialized()) return;
  bool reverse{false};

  if (speed < 0) {
    speed *= -1;
    reverse = true;
  }

  if (speed > 400) speed = 400;

  analogWrite(_pins.pwm, speed * 51 / 80);
  digitalWrite(_pins.directory, reverse);
  _speed = (reverse ? -speed : speed);
}

uint32_t MC33926MotorDriver::getCurrent() const {
  if (!isInitialized()) return 0u;
  return analogRead(_pins.feedback) * 9;
}

bool MC33926MotorDriver::getError() const {
  if (!isInitialized()) return true;
  return !static_cast<bool>(digitalRead(_pins.nSF));
}
#ifndef __ORION_MOTORDRIVER__
#define __ORION_MOTORDRIVER__

#include <Arduino.h>
#include <cstdint>

class MC33926MotorDriver {
 public:
  struct Pins {
    uint32_t pwm, directory, feedback, nD2, nSF;
  };

  MC33926MotorDriver();
  MC33926MotorDriver(const Pins& pins, bool init = true);

  bool initialize();
  bool initialize(const Pins& pins);

  void setSpeed(int speed);
  int getSpeed() const { return _speed; }

  bool isInitialized() const { return _initialized; }
  bool pinsSet() const { return _pinsSet; }

  uint32_t getCurrent() const;
  bool getError() const;

 private:
  bool _initialized, _pinsSet;
  Pins _pins;
  int _speed;
};

#endif
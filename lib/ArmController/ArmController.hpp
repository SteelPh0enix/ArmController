#ifndef __ORION_ARMCONTROLLER__
#define __ORION_ARMCONTROLLER__

#include <Servo.h>
#include <MotorDriver.hpp>
#include <array>
#include <cstdint>
#include <cstring>

namespace Orion {
enum class MotorType { Null, Servo, DC, Actuator };

template <std::size_t ID_LENGHT, std::size_t COUNT>
class ArmController {
  using MotorName = char[ID_LENGHT + 1];

  struct MotorInfo {
    MotorName id;
    bool error;
    int speed;
    MotorType type;
  };

 public:
  const std::size_t IDMaxLenght = ID_LENGHT;
  const std::size_t MotorCount = COUNT;

  ArmController() {
    for (Motor& m : _motors) m.id[0] = '\0';
  }

  bool addMotor(const MotorName id, MotorType type,
                const MC33926MotorDriver::Pins& pins) {
    if (type == MotorType::Servo) return false;
    for (Motor& m : _motors)
      if (m.id[0] == '\0') {
        m.driver.initialize(pins);
        strcpy(m.id, id);
        m.type = type;
        return true;
      }
    return false;
  }

  bool addServo(const MotorName id, int pin) {
    for (Motor& m : _motors)
      if (m.id[0] == '\0') {
        m.servo.attach(pin);
        m.servo.write(90);
        strcpy(m.id, id);
        m.type = MotorType::Servo;
        return true;
      }
    return false;
  }

  bool deleteMotor(const MotorName id) {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr) {
      motorPtr->id[0] = '\0';
      if (motorPtr->servo.attached()) motorPtr->servo.detach();
      return true;
    }
    return false;
  }

  bool setMotorSpeed(const MotorName id, int speed) {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr) {
      switch (motorPtr->type) {
        case MotorType::Null:
          return false;
        case MotorType::Servo:
          motorPtr->servo.write(speed);
          break;
        case MotorType::DC:
          motorPtr->driver.setSpeed(speed);
          break;
        case MotorType::Actuator:
          if (speed > 0)
            motorPtr->driver.setSpeed(400);
          else if (speed < 0)
            motorPtr->driver.setSpeed(-400);
          else
            motorPtr->driver.setSpeed(0);
          break;
      }
      return true;
    }
    return false;
  }

  int getMotorSpeed(const MotorName id) {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr) {
      if (motorPtr->type == MotorType::Servo)
        return motorPtr->servo.read();
      else
        return motorPtr->driver.getSpeed();
    }
    return 0;
  }

  MotorInfo getMotorInfo(const MotorName id) const {
    MotorInfo info{"", true, 0, MotorType::Null};

    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr) {
      info.id = id;
      if (motorPtr->type != MotorType::Servo) {
        info.error = motorPtr->driver.getError();
        info.speed = motorPtr->driver.getSpeed();
      }
      info.type = motorPtr->type;
    }
    return info;
  }

  bool getMotorError(const MotorName id) const {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr && motorPtr->type != MotorType::Servo)
      return motorPtr->driver.getError();
    return true;
  }

  MotorType getMotorType(const MotorName id) const {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr) return motorPtr->type;
    return MotorType::Null;
  }

  bool motorExists(const MotorName id) const {
    return _getMotorPtr(id) != nullptr;
  }

  uint32_t getMotorCurrent(const MotorName id) const {
    auto motorPtr = _getMotorPtr(id);
    if (motorPtr != nullptr && motorPtr->type != MotorType::Servo &&
        motorPtr->type != MotorType::Null)
      return motorPtr->driver.getCurrent();
    else
      return 0;
  }

 private:
  struct Motor {
    Servo servo;
    MC33926MotorDriver driver;
    MotorName id;
    MotorType type;
  };

  Motor* _getMotorPtr(const MotorName id) {
    for (auto& m : _motors) {
      if (std::strcmp(m.id, id) == 0) return &m;
    }
    return nullptr;
  }

  const Motor* _getMotorPtr(const MotorName id) const {
    for (const auto& m : _motors) {
      if (std::strcmp(m.id, id) == 0) return &m;
    }
    return nullptr;
  }

  std::array<Motor, COUNT> _motors;
};

}  // namespace Orion
#endif
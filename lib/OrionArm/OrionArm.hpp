/*
  Arm structure and JSON keys for motors:
  - Turntable motor - TRM
  - Lower arm actuator - LAA
  - Upper arm actuator - UAA
  - Arm extending motor - AEM
  - Grasper up/down (vertical) movement motor - GVM
  - Grasper rotation motor - GRM
  - Grasper horizontal servo - GHS

  Incoming data has format:
  -
*/
#ifndef __ORION_ARMLIB__
#define __ORION_ARMLIB__

#include <ArduinoJson.h>
#include <ArmController.hpp>

namespace Orion {

class Arm {
 public:
  Arm() : _arm(new ArmController<3, 7>) {
    _arm->addMotor("TRM", MotorType::DC, {2, 22, A0, 23, 24});
    _arm->addMotor("LAA", MotorType::Actuator, {3, 26, A1, 27, 28});
    _arm->addMotor("UAA", MotorType::Actuator, {4, 30, A2, 31, 32});
    _arm->addMotor("AEM", MotorType::DC, {5, 34, A3, 35, 36});
    _arm->addMotor("GVM", MotorType::DC, {6, 38, A4, 39, 40});
    _arm->addMotor("GRM", MotorType::DC, {7, 42, A5, 43, 44});
    _arm->addServo("GHS", 8);
  }

  ~Arm() { delete _arm; }

  template <class JSON_BUFFER>
  JsonObject executeJSON(const JsonObject& json, JSON_BUFFER& buffer) {
    // Create return object
    auto ret = buffer.createObject();
    ret["ID"] = 32;
    ret["DATA"] = buffer.createObject();

    // Check if the command is for you
    auto ID = json["ID"].as<int>();
    if (ID != 32) return ret;

    // Execute the command and store return in object
    auto mode = json["MODE"].as<int>();
    ret["MODE"] = mode;

    switch (mode) {
      case 1: {
        auto data = json["DATA"];
        if (data.success()) {
          move(json["DATA"], ret, buffer);
        }
      }
      case 2:
        executeCommand(json["DATA"].as<int>(), buffer);
    }

    // Return the object
    return ret;
  }

  template <class JSON_BUFFER>
  void executeCommand(int command, JsonObject& ret, JSON_BUFFER& buffer) {
    // TODO: Write actual command behaviour.
    // Reason why it's not done yet: i have to write encoders code first
    // For now, it does nothing. All the command ID's are invalid.
    ret["DATA"]["CMDF"] = -1;
  }

  template <class JSON_BUFFER>
  void move(const JsonObject& data, JsonObject& ret, JSON_BUFFER& buffer) {}

 private:
  // The arm class will be put on heap, to prevent stack overflow.
  // At least, for now.
  ArmController<3, 7>* _arm;
};

}  // namespace Orion
#endif
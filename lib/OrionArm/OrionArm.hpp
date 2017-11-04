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

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArmController.hpp>

namespace Orion {

class Arm {
  using arm_controller = ArmController<3, 1>;

 public:
  Arm() : _arm(new arm_controller) {
    _arm->addMotor(/*"TRM"*/ "GVM", MotorType::DC, {2, 26, A1, 29, 28});
    //_arm->addMotor("LAA", MotorType::Actuator, {3, 26, A1, 27, 28});
    //_arm->addMotor("UAA", MotorType::Actuator, {4, 30, A2, 31, 32});
    //_arm->addMotor("AEM", MotorType::DC, {5, 34, A3, 35, 36});
    //_arm->addMotor("GVM", MotorType::DC, {6, 38, A4, 39, 40});
    //_arm->addMotor("GRM", MotorType::DC, {7, 42, A5, 43, 44});
    //_arm->addServo("GHS", 8);
  }

  ~Arm() { delete _arm; }

  template <class JSON_BUFFER>
  JsonObject& executeJSON(const JsonObject& json, JSON_BUFFER& buffer) {
    // Check if the command is for you
    auto ID = json["ID"].as<int>();
    if (ID != 32) return buffer.createObject();

    JsonObject& ret = buffer.createObject();
    ret["ID"] = 32;
    ret["DATA"] = buffer.createObject();

    // Execute the command and store return in object
    auto mode = json["MODE"].as<int>();
    ret["MODE"] = mode;

    switch (mode) {
      case 1: {
        auto data = json["DATA"];
        if (data.success()) {
          move(data, ret["DATA"], buffer);
        }
        break;
      }
      case 2:
        executeCommand(json["DATA"].as<int>(), ret["DATA"], buffer);
        break;
    }

    // Return the object
    return ret;
  }

  // Recives command ID, and executes it. Modifies referenced JSON.
  // Arguments:
  //  int command - command ID
  //  JsonObject& ret - reference to json object in which the output data will
  //    be stored
  //  JSON_BUFFER& buffer - a buffer used to create objects/arrays
  template <class JSON_BUFFER>
  void executeCommand(int command, JsonObject& ret, JSON_BUFFER& buffer) {
    // TODO: Write actual command behaviour.
    // Reason why it's not done yet: i have to write encoders code first
    // For now, it does nothing. All the command ID's are invalid.
    ret["CMDF"] = -1;
  }

  // Received move data and sets the motor speeds/positions according to them.
  // Arguments:
  //  const JsonObject& data - received data
  //  JsonObject& ret - reference to json object in which the output data will
  //    be stored
  //  JSON_BUFFER& buffer - a buffer used to create objects/arrays
  template <class JSON_BUFFER>
  void move(const JsonObject& data, JsonObject& ret, JSON_BUFFER& buffer) {
    // Iterate throught received data
    // Feedback is returned only when referenced motor exists
    for (const auto& o : data) {
      if (_arm->motorExists(o.key)) {
        _arm->setMotorSpeed(o.key, o.value.as<int>());
        auto& feedback = ret.createNestedArray(o.key);
        feedback.add(_arm->getMotorSpeed(o.key));
        feedback.add(_arm->getMotorCurrent(o.key));
        feedback.add(_arm->getMotorError(o.key));
      }
    }
  }

  void waitForResponse() {
    while (!Serial.available()) {
      Serial.println("{\"ID\":32}");
      Sleep(500);
    }
  }

 private:
  // The arm class will be put on heap, to save the stack for JSON buffer
  // At least, for now.
  arm_controller* _arm;
};

}  // namespace Orion
#endif
/*
  Arm structure and JSON keys for motors:
  [CONTROLLER_ID - NAME - KEY]
  #1.1 - Upper arm actuator - UAA
  #1.2 - Lower arm actuator - LAA
  #2.1 - Turret rotation motor (NOT COUPLED) - TRM
  #2.2 - Grasper vertical motor (NOT COUPLED) - GVM
  #3.1 - n/a
  #3.2 - n/a
  PWM #8 - n/a

  Motor driver physical connections:
  [CONTROLLER_ID - PIN/{PWM, DIR, FB, ND2, SF}]
  #1.1 - {2, 22, A0, 24, 25}
  #1.2 - {3, 23, A1, 24, 25}
  #2.1 - {4, 26, A2, 28, 29}
  #2.2 - {5, 27, A3, 28, 29}
  #3.1 - {6, 30, A4, 32, 33}
  #3.2 - {7, 31, A5, 32, 33}
  PWM #8 - 8

  For more, go here:
  http://oriondoc.readthedocs.io/en/latest/docs/SDD/arm_io_documentation.html
*/
#ifndef __ORION_ARMLIB__
#define __ORION_ARMLIB__

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArmController.hpp>

namespace Orion {

class Arm {
  using arm_controller = ArmController<4, 3>;

 public:
  Arm() : _arm(new arm_controller) {
    _arm->addMotor("UAA", MotorType::Actuator, {2, 22, A0, 24, 25});
    _arm->addMotor("LAA", MotorType::Actuator, {3, 23, A1, 24, 25});
    _arm->addMotor("TRM", MotorType::DC, {4, 26, A2, 28, 29});

    _motor_names[0] = "UAA";
    _motor_names[1] = "LAA";
    _motor_names[2] = "TRM";
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
          move(data, ret["DATA"]);
        }
        break;
      }
      case 2:
        executeCommand(json["DATA"].as<int>(), ret["DATA"]);
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
  void executeCommand(int command, JsonObject& ret) {
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
  void move(const JsonObject& data, JsonObject& ret) {
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

  void printCurrents() const {
    for (const auto& m : _motor_names) {
      Serial.print(m);
      Serial.print(" -> ");
      Serial.println(_arm->getMotorCurrent(m));
    }
  }

 private:
  // The arm class will be put on heap, to save the stack for JSON buffer
  // At least, for now.
  arm_controller* _arm;
  const char* _motor_names[3];
};

}  // namespace Orion
#endif

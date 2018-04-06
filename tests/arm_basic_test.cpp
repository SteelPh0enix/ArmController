#include <Arduino.h>
#include <ArmController.hpp>

using Arm = Orion::ArmController<6, 1>;
Arm arm;

void setSpeed(int speed) {
  Serial.print("Setting motor speed to ");
  Serial.print(speed);
  Serial.print("... ");
  Serial.println(arm.setMotorSpeed("TEST", speed));
  Serial.print("Motor speed: ");
  Serial.println(arm.getMotorSpeed("TEST"));
  Serial.print("Motor error: ");
  Serial.println(arm.getMotorError("TEST"));
  Serial.print("Motor current: ");
  Serial.println(arm.getMotorCurrent("TEST"));
}

void checkDumb() {
  Serial.print("Checking if not-existing motor exist: ");
  Serial.println(arm.setMotorSpeed("NOP", 100));
}

void setup() {
  Serial.begin(9600);
  Serial.print("Adding motor... ");
  Serial.println(
      arm.addMotor("TEST", Orion::MotorType::Actuator, {5, 27, A3, 28, 29}));

  setSpeed(100);
  delay(1000);
  setSpeed(-100);
  delay(1000);
  setSpeed(1234);
  delay(1000);
  setSpeed(-1234);
  delay(1000);
  setSpeed(0);

  checkDumb();
}

void loop() {}
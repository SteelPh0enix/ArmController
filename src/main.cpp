#include <Arduino.h>
#include <ArmController.hpp>

using Arm = ArmController<6, 1>;
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
  Serial.println(arm.setMotorSpeed("NOPE", 100));
}

void setup() {
  Serial.begin(9600);
  Serial.print("Adding motor... ");
  Serial.println(arm.addMotor("TEST", Arm::MotorType::DC, {2, 40, A0, 41, 42}));

  setSpeed(100);
  delay(1500);
  setSpeed(-100);
  delay(1500);
  setSpeed(2137);
  delay(1500);
  setSpeed(-2137);
  delay(1500);

  checkDumb();
  setSpeed(0);
}

void loop() {}
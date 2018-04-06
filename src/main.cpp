#include <Arduino.h>
#include <OrionArm.hpp>

Orion::Arm* arm;

void setup() {
  arm = new Orion::Arm;
  Serial.begin(115200);
  arm->waitForResponse();
}

void loop() {
  StaticJsonBuffer<1024> jsonBuffer;
  if (Serial.available()) {
    JsonObject& json = jsonBuffer.parseObject(Serial, 2);

    JsonObject& feedback = arm->executeJSON(json, jsonBuffer);
    if (feedback.size() > 0) {
      feedback.printTo(Serial);
      Serial.println();
    }
  } else {
    arm->printCurrents();
    Sleep(500);
  }
}
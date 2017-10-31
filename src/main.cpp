#include <Arduino.h>
#include <OrionArm.hpp>

Orion::Arm arm;
StaticJsonBuffer<512> jsonBuffer;

void setup() { Serial.begin(38400); }

void loop() {
  if (Serial.available()) {
    JsonObject& json = jsonBuffer.parse(Serial);
    JsonObject& feedback = arm.executeJSON(json, jsonBuffer);
    feedback.printTo(Serial);
  }
}
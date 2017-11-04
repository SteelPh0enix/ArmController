#include <Arduino.h>
#include <OrionArm.hpp>

Orion::Arm arm;
StaticJsonBuffer<1024> jsonBuffer;

void setup() {
  Serial.begin(115200);
  arm.waitForResponse();
}

void loop() {
  if (Serial.available()) {
    auto received_json = Serial.readStringUntil('\n');
    JsonObject& json = jsonBuffer.parse(received_json);

    JsonObject& feedback = arm.executeJSON(json, jsonBuffer);
    feedback.printTo(Serial);
    jsonBuffer.clear();
  }
}
#include <MotorDriver.hpp>

MC33926MotorDriver driver;

void test_driver_config() {
  Serial.println("== Starting test_driver_config... ==");
  Serial.print("driver.pinsSet() = ");
  Serial.println(driver.pinsSet());
  Serial.print("driver.isInitialied() = ");
  Serial.println(driver.isInitialized());

  Serial.print("driver.initialize({2, 26, A1, 29, 28}) = ");
  Serial.println(driver.initialize({2, 26, A1, 29, 28}));

  Serial.print("driver.pinsSet() = ");
  Serial.println(driver.pinsSet());
  Serial.print("driver.isInitialied() = ");
  Serial.println(driver.isInitialized());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  Serial.println("== End of test ==");
}

void test_driver_set_speed() {
  Serial.println("== Starting test_driver_set_speed... ==");

  driver.setSpeed(100);
  Serial.print("Speed set to 100. driver.getSpeed() = ");
  Serial.println(driver.getSpeed());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  delay(1000);

  driver.setSpeed(-100);
  Serial.print("Speed set to -100. driver.getSpeed() = ");
  Serial.println(driver.getSpeed());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  Serial.println("== End of test ==");
}

void test_driver_overset_speed() {
  Serial.println("== Starting test_driver_overset_speed... ==");
  driver.setSpeed(1000);
  Serial.print("Speed set to 1000. driver.getSpeed() = ");
  Serial.println(driver.getSpeed());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  delay(1000);

  driver.setSpeed(-1000);
  Serial.print("Speed set to -1000. driver.getSpeed() = ");
  Serial.println(driver.getSpeed());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  Serial.println("== End of test ==");
}

void test_driver_reset_speed() {
  Serial.println("== Starting test_driver_reset_speed... ==");
  driver.setSpeed(0);
  Serial.print("Speed set to 0. driver.getSpeed() = ");
  Serial.println(driver.getSpeed());
  Serial.print("driver.getError() = ");
  Serial.println(driver.getError());
  Serial.println("== End of test ==");
}

void setup() {
  Serial.begin(9600);

  test_driver_config();
  delay(2000);
  test_driver_set_speed();
  delay(2000);
  test_driver_overset_speed();
  delay(2000);
  test_driver_reset_speed();
  delay(2000);
}

void loop() {}
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

#define Depan 26
#define Belakang 27
#define Kiri 32
#define Kanan 33

void setup() {
  Wire.begin(21, 22);
  Serial.begin(9600);

  mpu.initialize();

// Verify connection with sensor
  if (mpu.testConnection()) {
    Serial.println("MPU6050 terdeteksi!");
  } else {
    Serial.println("Koneksi dengan MPU6050 gagal.");
  }

// Set the motor pin as OUTPUT
  pinMode(Depan, OUTPUT);
  pinMode(Belakang, OUTPUT);
  pinMode(Kiri, OUTPUT);
  pinMode(Kanan, OUTPUT);
}

void loop() {
  int16_t accelX, accelY, accelZ;
  mpu.getAcceleration(&accelX, &accelY, &accelZ);

  // Calculates tilt angle based on accelerometer data
  float roll = atan2(accelY, accelZ) * 180.0 / PI;
  float pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / PI;

  // Determine the tilt direction
  String orientation = determineOrientation(roll, pitch);

  // Display data on Serial Monitor

  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.println("0");

  // Motor control based on orientation
  controlMotors(orientation);

  delay(100); // Wait 100ms before reading the accelerometer data again
}

String determineOrientation(float roll, float pitch) {
  if (roll > 30) {
    return "Kanan";
  } else if (roll < -30) {
    return "Kiri";
  } else if (pitch > 30) {
    return "Depan"; 
  } else if (pitch < -30) {
    return "Belakang";
  } else {
    return "Diam";
  }
}

void controlMotors(String orientation) {
  if (orientation == "Kanan") {
    digitalWrite(Kanan, HIGH);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    // Control LED when mpu6050 tilts to the right
  } else if (orientation == "Kiri") {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, HIGH);
    // Control LED when mpu6050 tilts to the left
  } else if (orientation == "Depan") {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, HIGH);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    // Control LED when mpu6050 tilts to the front
  } else if (orientation == "Belakang") {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, HIGH);
    digitalWrite(Kiri, LOW);
    // Control LED when mpu6050 tilts to the back
  } else {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    // LED control when mpu6050 has no tilt movement
  }
}

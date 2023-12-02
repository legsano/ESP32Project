#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

MPU6050 mpu;
#define INTERRUPT_PIN 23

volatile bool mpuInterrupt = false;
volatile uint8_t fifoBuffer[64];
uint8_t packetSize;
uint8_t mpuIntStatus;
volatile uint16_t fifoCount;

Quaternion q;
VectorFloat gravity;
float ypr[3];
float yaw, pitch, roll;

void dmpDataReady() {
    mpuInterrupt = true;
}
// NOTE MAKE OFFSITE CHANGES AND IF YOU CAN ADD CALIBRATION
void setup() {
  Wire.begin(21, 22);
  mpu.initialize();

  // Enable DMP
  Serial.println("Initializing DMP...");
  uint8_t devStatus = mpu.dmpInitialize();

  mpu.setXGyroOffset(220);    //220
  mpu.setYGyroOffset(76);     //76
  mpu.setZGyroOffset(90);     //-85
  mpu.setZAccelOffset(1788);  //1788

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);

    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuInterrupt = false;

    // Initialize Serial
    Serial.begin(9600);
    
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.println("DMP Initialization failed.");
  }
}

void loop() {
  if (!mpuInterrupt) {
    return;
  }
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  if (mpuIntStatus & 0x10) {
    mpu.resetFIFO();
    Serial.println("FIFO overflow!");
    return;
  } else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
    
    uint8_t fifoData[packetSize];
    mpu.getFIFOBytes(fifoData, packetSize);
    mpu.resetFIFO();
    mpu.dmpGetQuaternion(&q, fifoData);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    

    yaw = ypr[0];
    pitch = ypr[1];
    roll = ypr[2];

    Serial.print("Yaw: ");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print(" Pitch: ");
    Serial.print(ypr[1] * 180/M_PI);  
    Serial.print(" Roll: ");
    Serial.println(ypr[2] * 180/M_PI);

    
    //Serial.print(yaw * 180/M_PI); //yaw
    //Serial.print(",");
    //Serial.print(pitch* 180/M_PI); // pitch
    //Serial.print(",");                
    //Serial.println(roll * 180/M_PI); // roll
  }
  delay(90);
}

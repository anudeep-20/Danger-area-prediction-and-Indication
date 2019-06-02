#include <TinyWireM.h>
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
SoftwareSerial mySerial(-1, 4);

const int trigPin = 1;
const int echoPin = 3;

float gyroX, gyroY, gyroZ;
int distance;
Ultrasonic ultrasonic(trigPin, echoPin);

void setup() {
  mySerial.begin(9600);
  TinyWireM.begin();
  TinyWireM.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  TinyWireM.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  TinyWireM.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(0b1101000); //I2C address of the MPU
  TinyWireM.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  TinyWireM.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s
  TinyWireM.endTransmission();
}

void loop() {

  TinyWireM.beginTransmission(0b1101000); //I2C address of the MPU
  TinyWireM.write(0x43); //Starting register for Gyro Readings
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(0b1101000, 6); //Request Gyro Registers (43 - 48)
  while (TinyWireM.available() < 6);
  gyroX = TinyWireM.read() << 8 | TinyWireM.read(); //Store first two bytes into accelX
  gyroY = TinyWireM.read() << 8 | TinyWireM.read(); //Store middle two bytes into accelY
  gyroZ = TinyWireM.read() << 8 | TinyWireM.read(); //Store last two bytes into accelZ

  gyroX = gyroX / 131.0;
  gyroY = gyroY / 131.0;
  gyroZ = gyroZ / 131.0;

  distance = ultrasonic.read();
  mySerial.print(gyroX);
  mySerial.print(",");
  mySerial.print(gyroY);
  mySerial.print(",");
  mySerial.print(gyroZ);
  mySerial.print(",");
  if (distance < 100) {
    mySerial.println(distance);
  }
  else {
    distance = 0;
    mySerial.println(distance);
  }
}

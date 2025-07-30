#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;
SoftwareSerial simModule(7, 8);                           // SIMA7670C connected to pins 7 RX and 8 TX
int vibrationSensorPin = 2;
int vibrationState = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
                                                          // start comms
  Serial.begin(9600);
  simModule.begin(9600);
  
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected.");
  } else {
    Serial.println("MPU6050 connection failed.");
  }

                                                        
  pinMode(vibrationSensorPin, INPUT);

                                                        // create message
  simModule.println("AT"); 
  delay(1000);
  simModule.println("AT+CMGF=1");                       //SMS text mode from ATcommands
  delay(1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  vibrationState = digitalRead(vibrationSensorPin);

                                                        // gyro readings 
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

                                                        // data packet
  String message = "Gyro Data: Gx=" + String(gx) + ", Gy=" + String(gy) + ", Gz=" + String(gz);
  message += " | Vibration: " + String(vibrationState);

                                                        // Send SMS
  simModule.println("AT+CMGS=\"+919067xxxxx""); // Replace phone number
  delay(1000);
  simModule.println(message);
  simModule.write(26);
  delay(60000);
}

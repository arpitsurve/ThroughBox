#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;
SoftwareSerial simModule(7, 8); // SIMA7670C connected to pins 7 (RX) and 8 (TX)
int vibrationSensorPin = 2; // Digital pin for the vibration sensor
int vibrationState = 0;

void setup() {
  // Start Serial communication with the PC and the SIMA7670C
  Serial.begin(9600);
  simModule.begin(9600);
  
  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected.");
  } else {
    Serial.println("MPU6050 connection failed.");
  }

  // Initialize vibration sensor pin as input
  pinMode(vibrationSensorPin, INPUT);

  // Setup the SIMA7670C to send messages
  simModule.println("AT"); // Check communication
  delay(1000);
  simModule.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);
}

void loop() {
  // Get vibration sensor reading
  vibrationState = digitalRead(vibrationSensorPin);

  // Get gyroscope readings from the MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Prepare the data to send
  String message = "Gyro Data: Gx=" + String(gx) + ", Gy=" + String(gy) + ", Gz=" + String(gz);
  message += " | Vibration: " + String(vibrationState);

  // Send SMS with the gyro and vibration data
  simModule.println("AT+CMGS=\"+1234567890\""); // Replace with the recipient's phone number
  delay(1000); // Wait for prompt
  simModule.println(message); // Send the message
  simModule.write(26); // End the message (CTRL+Z)
  delay(5000); // Wait before sending the next message
}

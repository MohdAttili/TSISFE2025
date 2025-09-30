## This folder contains the control software code for all components, including the main algorithms, sensor integration, and motor control functions. It is the core that makes the vehicle autonomous.
<p>
  <img src="/other/autonomous_vehicle_software_core.png" alt="Image 8" />
      </p>


**Cam Test Code**
```cpp
#include <Pixy2I2C.h>
#include <Wire.h>

Pixy2I2C cam;

void setup() {
Serial.begin(115200);
Wire.begin(21, 22); 
cam.init();
delay(500);
}
```
<details>
<summary>Show remaining code</summary>

```cpp
    char getObstcaleColor(){
    cam.ccc.getBlocks();
    int numBlocks = cam.ccc.numBlocks;
    if (numBlocks > 0) {
        int largestIndex = 0;
        int maxArea = 0;

        
        for (int i = 0; i < numBlocks; i++) {
        int area = cam.ccc.blocks[i].m_width * cam.ccc.blocks[i].m_height;
        if (area > maxArea) {
            maxArea = area;
            largestIndex = i;
        }
        }
        
        int obstcaleColor =cam.ccc.blocks[largestIndex].m_signature;
        
        if(obstcaleColor == 1||obstcaleColor == 2||obstcaleColor == 3||obstcaleColor == 4){
        return 'R';
        }
        else if (obstcaleColor == 5||obstcaleColor == 6||obstcaleColor == 7||obstcaleColor == 8){
        return 'G';
        }
        else{
        return 'U';
        }
    }
    else{
        return 'N';
        }
    }
    void loop() {

    Serial.println(getObstcaleColor());
    delay(200);

    }
```
</details>

## --------------------------------------------------------------------------------------

**Sensor Testing Code**
```cpp
// Motor Control Pins
#define ENA 16    // PWM pin for speed control
#define IN1 17    // Direction control 1
#define IN2 5     // Direction control 2

// Encoder Pins
#define ENC_A 32  // Encoder channel A
#define ENC_B 33  // Encoder channel B

// Variables for encoder counting
volatile long encoderCount = 0;
int lastEncA = 0;
int lastEncB = 0;

// Motor control parameters
int motorSpeed = 200;  // PWM value (0-255)
bool motorDirection = true; // true = forward, false = reverse

 ```   
<details>
<summary>Show remaining code</summary>

```cpp

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Set encoder pins as inputs with pullup resistors
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  
  // Read initial encoder state
  lastEncA = digitalRead(ENC_A);
  lastEncB = digitalRead(ENC_B);
  
  // Attach interrupt for encoder pin A
  attachInterrupt(digitalPinToInterrupt(ENC_A), updateEncoder, CHANGE);
  
  Serial.println("ESP32 Motor Control with Encoder");
  Serial.println("Commands: ");
  Serial.println("  F - Forward");
  Serial.println("  R - Reverse");
  Serial.println("  S - Stop");
  Serial.println("  + - Increase Speed");
  Serial.println("  - - Decrease Speed");
  Serial.println("  P - Print encoder count");
}

void loop() {
  // Check for serial commands
  if (Serial.available()) {
    char command = Serial.read();
    
    switch(command) {
      case 'F':
      case 'f':
        motorForward();
        Serial.println("Motor: FORWARD");
        break;
        
      case 'R':
      case 'r':
        motorReverse();
        Serial.println("Motor: REVERSE");
        break;
        
      case 'S':
      case 's':
        motorStop();
        Serial.println("Motor: STOP");
        break;
        
      case '+':
        increaseSpeed();
        Serial.print("Speed increased to: ");
        Serial.println(motorSpeed);
        break;
        
      case '-':
        decreaseSpeed();
        Serial.print("Speed decreased to: ");
        Serial.println(motorSpeed);
        break;
        
      case 'P':
      case 'p':
        Serial.print("Encoder pulses: ");
        Serial.println(encoderCount);
        break;
    }
  }
  
  // Small delay to prevent overwhelming the serial
  delay(100);
}

// Encoder interrupt service routine
void updateEncoder() {
  int currentEncA = digitalRead(ENC_A);
  int currentEncB = digitalRead(ENC_B);
  
  // Determine direction based on state changes
  if (currentEncA != lastEncA) {
    if (currentEncA == currentEncB) {
      encoderCount++;
    } else {
      encoderCount--;
    }
  } else if (currentEncB != lastEncB) {
    if (currentEncA == currentEncB) {
      encoderCount--;
    } else {
      encoderCount++;
    }
  }
  
  lastEncA = currentEncA;
  lastEncB = currentEncB;
}

// Motor control functions
void motorForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, motorSpeed);
  motorDirection = true;
}

void motorReverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, motorSpeed);
  motorDirection = false;
}

void motorStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void increaseSpeed() {
  motorSpeed = min(255, motorSpeed + 25);
  analogWrite(ENA, motorSpeed);
}

void decreaseSpeed() {
  motorSpeed = max(0, motorSpeed - 25);
  analogWrite(ENA, motorSpeed);
}

```
</details>

## --------------------------------------------------------------------------------------

**Motor Testing Code**
```cpp
// Motor Control Pins
#define ENA 16    // PWM pin for speed control
#define IN1 17    // Direction control 1
#define IN2 5     // Direction control 2

// Encoder Pins
#define ENC_A 32  // Encoder channel A
#define ENC_B 33  // Encoder channel B

// Variables for encoder counting
volatile long encoderCount = 0;
int lastEncA = 0;
int lastEncB = 0;

// Motor control parameters
int motorSpeed = 200;  // PWM value (0-255)
bool motorDirection = true; // true = forward, false = reverse

 ```   
<details>
<summary>Show remaining code</summary>

```cpp

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Set encoder pins as inputs with pullup resistors
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  
  // Read initial encoder state
  lastEncA = digitalRead(ENC_A);
  lastEncB = digitalRead(ENC_B);
  
  // Attach interrupt for encoder pin A
  attachInterrupt(digitalPinToInterrupt(ENC_A), updateEncoder, CHANGE);
  
  Serial.println("ESP32 Motor Control with Encoder");
  Serial.println("Commands: ");
  Serial.println("  F - Forward");
  Serial.println("  R - Reverse");
  Serial.println("  S - Stop");
  Serial.println("  + - Increase Speed");
  Serial.println("  - - Decrease Speed");
  Serial.println("  P - Print encoder count");
}

void loop() {
  // Check for serial commands
  if (Serial.available()) {
    char command = Serial.read();
    
    switch(command) {
      case 'F':
      case 'f':
        motorForward();
        Serial.println("Motor: FORWARD");
        break;
        
      case 'R':
      case 'r':
        motorReverse();
        Serial.println("Motor: REVERSE");
        break;
        
      case 'S':
      case 's':
        motorStop();
        Serial.println("Motor: STOP");
        break;
        
      case '+':
        increaseSpeed();
        Serial.print("Speed increased to: ");
        Serial.println(motorSpeed);
        break;
        
      case '-':
        decreaseSpeed();
        Serial.print("Speed decreased to: ");
        Serial.println(motorSpeed);
        break;
        
      case 'P':
      case 'p':
        Serial.print("Encoder pulses: ");
        Serial.println(encoderCount);
        break;
    }
  }
  
  // Small delay to prevent overwhelming the serial
  delay(100);
}

// Encoder interrupt service routine
void updateEncoder() {
  int currentEncA = digitalRead(ENC_A);
  int currentEncB = digitalRead(ENC_B);
  
  // Determine direction based on state changes
  if (currentEncA != lastEncA) {
    if (currentEncA == currentEncB) {
      encoderCount++;
    } else {
      encoderCount--;
    }
  } else if (currentEncB != lastEncB) {
    if (currentEncA == currentEncB) {
      encoderCount--;
    } else {
      encoderCount++;
    }
  }
  
  lastEncA = currentEncA;
  lastEncB = currentEncB;
}

// Motor control functions
void motorForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, motorSpeed);
  motorDirection = true;
}

void motorReverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, motorSpeed);
  motorDirection = false;
}

void motorStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void increaseSpeed() {
  motorSpeed = min(255, motorSpeed + 25);
  analogWrite(ENA, motorSpeed);
}

void decreaseSpeed() {
  motorSpeed = max(0, motorSpeed - 25);
  analogWrite(ENA, motorSpeed);
}

```
</details>

## --------------------------------------------------------------------------------------

**open challenge v1.0**
```cpp
#include <ESP32Servo.h>
#include <Arduino.h>
#include <Wire.h>
#include <NewPing.h>

// ==================== Hardware ====================
Servo myservo;
const int servoPin = 32;

#define US_LEFT_TRIG 27
#define US_LEFT_ECHO 14
#define US_FRONT_TRIG 12
#define US_FRONT_ECHO 13
#define US_RIGHT_TRIG 2
#define US_RIGHT_ECHO 15
#define MAX_DISTANCE 200

NewPing sonar_left(US_LEFT_TRIG, US_LEFT_ECHO, MAX_DISTANCE);
NewPing sonar_front(US_FRONT_TRIG, US_FRONT_ECHO, MAX_DISTANCE);
NewPing sonar_right(US_RIGHT_TRIG, US_RIGHT_ECHO, MAX_DISTANCE);

#define MOTOR1_IN1 17
#define MOTOR1_IN2 5
#define MOTOR1_ENA 16

const int MOTOR1_CHANNEL = 1; // Motor channel
const int SERVO_CHANNEL = 0;  // Servo channel

#define MAX_STEERING_ANGLE 20   // ±20°

 ```   
<details>
<summary>Show remaining code</summary>

```cpp

// MPU6050 Address
const int MPU_addr = 0x68;

// Gyroscope calibration and yaw tracking
float gyroX_bias = 0;
float gyroY_bias = 0;
float gyroZ_bias = 0;
float yaw = 0;
unsigned long prevTime = 0;

// ---------------- MPU Functions ----------------
int16_t read16(int addr) {
  int16_t value;
  Wire.beginTransmission((uint8_t)MPU_addr);
  Wire.write((uint8_t)addr);
  Wire.endTransmission(false);
  Wire.requestFrom((uint16_t)MPU_addr, (uint8_t)2, (uint8_t)true);
  value = (Wire.read() << 8) | Wire.read();
  return value;
}

void calibrateGyro(int samples = 200) {
  long sumX = 0, sumY = 0, sumZ = 0;
  for (int i = 0; i < samples; i++) {
    int16_t gx = read16(0x43);
    int16_t gy = read16(0x45);
    int16_t gz = read16(0x47);
    sumX += gx; sumY += gy; sumZ += gz;
    delay(5);
  }
  gyroX_bias = sumX / (float)samples;
  gyroY_bias = sumY / (float)samples;
  gyroZ_bias = sumZ / (float)samples;


void resetGyro() {
  yaw = 0;
  prevTime = millis();
}

void setupMPU() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

float getYaw() {
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0;
  prevTime = currentTime;

  int16_t gz_raw = read16(0x47);
  float gz = gz_raw - gyroZ_bias;
  float gz_dps = gz / 131.0;

  yaw += gz_dps * dt;
  if (yaw > 180) yaw -= 360;
  if (yaw < -180) yaw += 360;

  return yaw;
}


float get_distance(NewPing &sensor) {
    float sum = 0;
    byte valid = 0;
    for (byte i = 0; i < 5; i++) {
        float dist = sensor.ping_cm();
        // Check if the distance is valid (greater than 0 and less than MAX_DISTANCE)
        if (dist > 0 && dist < MAX_DISTANCE) {
            sum += dist;
            valid++;
        }
        // delay(2);  // Small delay between readings
    }
    // If we have valid readings, return the average; otherwise, return MAX_DISTANCE
    return (valid > 0) ? (sum / valid) : MAX_DISTANCE;
}



void setAngle(int angle) {
    myservo.write(angle);  // Move servo to the specified angle
    delay(100);  // Small delay to allow the servo to move
}

void set_drive_motor(int speed) {
    speed = constrain(speed, -1023, 1023);  // Ensure speed is within PWM range
    if (speed >= 0) {
        digitalWrite(MOTOR1_IN1, HIGH);
        digitalWrite(MOTOR1_IN2, LOW);
    } else {
        digitalWrite(MOTOR1_IN1, LOW);
        digitalWrite(MOTOR1_IN2, HIGH);
    }
    ledcWrite(MOTOR1_CHANNEL, abs(speed));  // Motor control using PWM
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000);
    delay(500);
    setupMPU();
    delay(500);
    calibrateGyro();
    delay(500);
    resetGyro();
    delay(500);
    pinMode(MOTOR1_IN1, OUTPUT);  // Motor control pins
    pinMode(MOTOR1_IN2, OUTPUT);
    pinMode(MOTOR1_ENA, OUTPUT);

    pinMode(servoPin, OUTPUT);  // Set the servo pin as OUTPUT for PWM

    // Motor PWM setup
    ledcSetup(MOTOR1_CHANNEL, 1000, 10);  // 1000 Hz frequency, 10-bit resolution
    ledcAttachPin(MOTOR1_ENA, MOTOR1_CHANNEL);

    
    ledcSetup(SERVO_CHANNEL, 50, 16);  // 50Hz for servo control, 16-bit resolution
    ledcAttachPin(servoPin, SERVO_CHANNEL);

    myservo.attach(servoPin, 1000, 2000);  // Attach servo to the pin
    setAngle(113);  // Set initial servo position
}



// ---------------- PID ----------------
float Kp_dist = 0.3, Kd_dist = 1;
float Kp_gyro = 2.0, Kd_gyro = 0.5;

float prev_error_dist = 0;
float prev_error_gyro = 0;

bool use_distance_pid = true;
const float target_yaw = 0.0;

void pid_distance_balance() {
  float left = get_distance(sonar_left);
  float right = get_distance(sonar_right);

  float error = left - right;
  float derivative = error - prev_error_dist;
  prev_error_dist = error;

  float output = (Kp_dist * error) + (Kd_dist * derivative);
  output = constrain(output, -MAX_STEERING_ANGLE, MAX_STEERING_ANGLE);

  if(output>30){
    output=20;
  }
  else if(output<-30){
    output=-20;
  }

  setAngle(113+((int)output*2));

}

unsigned long lastPIDRun = 0;
const unsigned long SENSOR_INTERVAL = 50; // PID interval in ms

int dir=-1;
    int counter = 0;
bool use_pid=false;
void loop() {
  if (counter>11){
    set_drive_motor(0);
    setAngle(113);
    return;
  }
  if (counter>0){
    use_pid=true;
  }
    set_drive_motor(500);
    // Get distance readings from ultrasonic sensors
    float left = get_distance(sonar_left);
    float right = get_distance(sonar_right);
    float front = get_distance(sonar_front);
    if(dir==-1){
            if(right>100){
                dir=1;
            }
            else if(left>100)dir=0;
        }
    
    if (((right>100 && right !=200)||(left>100 && left !=200)) && counter <= 11) {

        
        if(dir==1){
          counter++;
            resetGyro();
            while(getYaw()>-75){
                    set_drive_motor(500); 

                setAngle(35);  
              }
            
        
            resetGyro();
        }
        else if(dir==0){
          counter++;
            resetGyro();
            while(getYaw()<75){
              
                setAngle(180); 
            set_drive_motor(500);
            }
            resetGyro();
        }
      while(right>100||left>100){
        float left = get_distance(sonar_left);
    float right = get_distance(sonar_right);
        setAngle(113);
        set_drive_motor(500);
        if(right+left<120){
          break;
        }
      }
    }
    if(use_pid){
        unsigned long currentMillis = millis();
  if (currentMillis - lastPIDRun >= SENSOR_INTERVAL &&counter<=11) {
    lastPIDRun = currentMillis;
      pid_distance_balance();
    
  }}
}
```
</details>
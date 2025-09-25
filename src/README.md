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

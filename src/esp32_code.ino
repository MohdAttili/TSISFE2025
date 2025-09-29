#include <ESP32Servo.h>
#include <Arduino.h>
#include <Wire.h>
#include <NewPing.h>
Servo myservo;
const int MPU_addr = 0x68;
float gyroX_bias = 0;
float gyroY_bias = 0;
float gyroZ_bias = 0;
float yaw = 0;
const int servoPin = 13; 

#define US_LEFT_TRIG 27
#define US_LEFT_ECHO 14
#define US_FRONT_TRIG 12
#define US_FRONT_ECHO 13
#define US_RIGHT_TRIG 2
#define US_RIGHT_ECHO 15

#define MOTOR1_IN1 17
#define MOTOR1_IN2 5
#define MOTOR1_ENA 16

NewPing sonar_left(US_LEFT_TRIG, US_LEFT_ECHO, MAX_DISTANCE);
NewPing sonar_front(US_FRONT_TRIG, US_FRONT_ECHO, MAX_DISTANCE);
NewPing sonar_right(US_RIGHT_TRIG, US_RIGHT_ECHO, MAX_DISTANCE);

float measured_distance_left = 0, measured_distance_front = 0,measured_distance_right=0;
int current_steering_angle = 0;
int target_steering_angle = 0;

const int MOTOR1_CHANNEL = 0;

#define MAX_STEERING_ANGLE 20

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
  Serial.println("Calibration done!");
  Serial.print("Bias Z (Yaw): "); Serial.println(gyroZ_bias);
}

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



void set_drive_motor(int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
  } else {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
  }
  ledcWrite(MOTOR1_CHANNEL, abs(speed));
}
// Define the min and max pulse widths for the DS558HV servo
// These values might need calibration for optimal performance
// Standard values are 500us to 2500us for 180 degrees.
// For a 40 degree servo, we need to find the pulse width for -20, 0, and +20 degrees.
// Let\'s assume a linear mapping for now. If 1500us is 0 degrees, and the total travel is 40 degrees,
// then 20 degrees corresponds to a certain pulse width change.
// A common mapping for 180 degrees is 500us to 2500us (2000us range).
// For 40 degrees, the range would be (40.0/180.0) * 2000.0 = 444.44us. (Using floats for precision)
// So, if 0 degrees is 1500us, then -20 degrees would be 1500 - (444.44/2) = 1500 - 222.22 = 1277.78us.
// And +20 degrees would be 1500 + (444.44/2) = 1500 + 222.22 = 1722.22us.
// Let\'s use these calculated values as a starting point, rounded to integers.


const int center = 1500; // Pulse width for 0 degrees (approx)

void setup() {
  Serial.begin(115200);
  myservo.setPeriodHertz(50); // Standard 50Hz servo
  myservo.attach(servoPin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  Serial.println("Servo initialized. Moving to 0 degrees.");
  setAngle(0); // Move to initial 0 degree position
}

float get_filtered_distance(NewPing &sensor) {
  float sum = 0;
  byte valid = 0;
  for (byte i = 0; i <5; i++) {
    delay(5);
    float dist = sensor.ping_cm();
    if (dist > 0 && dist < MAX_DISTANCE) {
      sum += dist;
      valid++;
    }
  }
  return valid > 0 ? sum / valid : 0;
}

void setAngle(int angle) {
  long pulseWidth = map(angle, -MAX_STEERING_ANGLE, MAX_STEERING_ANGLE, center+(MAX_STEERING_ANGLE/180)*2000), center-(MAX_STEERING_ANGLE/180)*2000));
  myservo.writeMicroseconds(pulseWidth);
  Serial.print("Setting servo to angle: ");
  Serial.print(angle);
  Serial.print(" degrees (pulse width: ");
  Serial.print(pulseWidth);
  Serial.println(" us)");
}
//--------------------------------------------------------------
// --- PID parameters (tune them!) ---
float Kp_dist = 2.0, Kd_dist = 0.5;
float Kp_gyro = 3.0, Kd_gyro = 0.8;

float integral_dist = 0, prev_error_dist = 0;
float integral_gyro = 0, prev_error_gyro = 0;

int distance_output = 0;
bool use_distance_pid = true;

// --- Targets ---
const float target_yaw = 0.0; // straight direction

// --- PID for left/right balance ---
void pid_distance_balance() {
  float left = get_filtered_distance(sonar_left);
  float right = get_filtered_distance(sonar_right);

  // Error: want left == right
  float error = left - right;

  float derivative = error - prev_error_dist;
  prev_error_dist = error;

  float output = (Kp_dist * error) + (Kd_dist * derivative);

  // Clamp steering to max
  output = constrain(output, -MAX_STEERING_ANGLE, MAX_STEERING_ANGLE);
  distance_output = output;

  // Apply steering correction
  setAngle((int)output);

  Serial.print("Left: "); Serial.print(left);
  Serial.print("  Right: "); Serial.print(right);
  Serial.print("  Error: "); Serial.print(error);
  Serial.print("  Output: "); Serial.println(output);

  // If angle exceeds 30°, switch to gyro PID
  if (output >= 30) {
    output=25;
  }
  else if(output<=-30){
    output=-25;
  }
  if(abs(getYaw())>30){
    use_distance_pid = false;
    integral_gyro = 0;
    prev_error_gyro = 0;
    Serial.println(">>> Switching to Gyro PID");
  }
}

// --- PID for gyro stabilization ---
void pid_gyro() {
  float current_yaw = getYaw();
  float error = target_yaw - current_yaw;

  float derivative = error - prev_error_gyro;
  prev_error_gyro = error;

  float output = (Kp_gyro * error) + (Kd_gyro * derivative);
  output = constrain(output, -MAX_STEERING_ANGLE, MAX_STEERING_ANGLE);

  setAngle((int)output);

  Serial.print("Yaw: "); Serial.print(current_yaw);
  Serial.print("  Gyro PID Output: "); Serial.println(output);

  // When robot is straight again, switch back to distance PID
  if(output>=30){
    output=25;
  }
  else if(output<=-30){
    output=-25
  }
  if (abs(current_yaw) < 2.0) {
    use_distance_pid = true;
    integral_dist = 0;
    prev_error_dist = 0;
    Serial.println(">>> Switching back to Distance PID");
  }
}

String dirc(){
  float left = get_filtered_distance(sonar_left);
  float right = get_filtered_distance(sonar_right);
  float front = get_filtered_distance(sonar_front);
  if(front<50){
    if(right>100){
      return "right";
    }
    else return "left";
  }
  else{
    return "forward";
  }
}

void turn_90Deg(int deg){
  while (getYaw()<85){
    setAngle(deg);
  }
  stop_robot();
  setAngle(0);
  resetGyro(0);
  curr+=90;
}

int prev=0;
int curr=0;
int angle=0;

void gyro_angle(int prev_angle,curr_angle){
  angle=curr_angle-prev_angle;
}
void stop_robot() {
  set_drive_motor(0);
  set_steering_angle(0);
  ledcWrite(MOTOR2_CHANNEL, 0);
}
void loop() {
  if(dirc()=="forward"){
    set_drive_motor(230);
    if (use_distance_pid) {
      pid_distance_balance();
    } else {
      pid_gyro();
    }
  delay(200);
  }
  else if(dirc()=="left"){

    set_drive_motor(180);
    gyro_angle(prev,curr);
    turn_90Deg(-angle);
  }
  else {
    set_drive_motor(180);
    gyro_angle(prev,curr);
    turn_90Deg(angle);
  }
}

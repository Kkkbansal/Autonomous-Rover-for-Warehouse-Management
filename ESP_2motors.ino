#include <Arduino.h>

// ===================== PIN DEFINITIONS =====================

// -------- Motor 1 pins --------
#define M1_PWM   25        // PWM pin
#define M1_DIR   26        // DIR pin
#define M1_ENC_A 35        // Encoder A
#define M1_ENC_B 13        // Encoder B

// -------- Motor 2 pins --------
#define M2_PWM   33       // PWM pin
#define M2_DIR   32        // DIR pin
#define M2_ENC_A 27        // Encoder A
#define M2_ENC_B 14        // Encoder B

// ==================== ENCODER VARIABLES ====================

volatile long enc1Count = 0;
volatile uint8_t prev1State = 0;

volatile long enc2Count = 0;
volatile uint8_t prev2State = 0;

// =================== MOTOR / PID PARAMETERS =================

const int ppr_mech = 325;
const int ppr_4x   = ppr_mech * 4;

// PID values
float Kp = 2.35f;
float Ki = 3.875f;
float Kd = 0.415f;

float integMin = -50.0f;
float integMax =  50.0f;

// PID states M1
float err1=0, prevErr1=0, integ1=0, dFiltered1=0;
float rpmFiltered1 = 0;

// PID states M2
float err2=0, prevErr2=0, integ2=0, dFiltered2=0;
float rpmFiltered2 = 0;

// Filters
const float rpmAlpha = 0.3f;
const float dAlpha   = 0.15f;

// Setpoints
float setRpm1 = 50.0f;
float setRpm2 = 50.0f;

// Timing
unsigned long lastTs = 0;
const unsigned long Ts_ms = 10;


// ====================== QUADRATURE ISR M1 =====================
void IRAM_ATTR enc1ISR() {
  uint8_t state = (digitalRead(M1_ENC_A) << 1) | digitalRead(M1_ENC_B);

  if ((prev1State == 0b00 && state == 0b01) ||
      (prev1State == 0b01 && state == 0b11) ||
      (prev1State == 0b11 && state == 0b10) ||
      (prev1State == 0b10 && state == 0b00))
    enc1Count++;
  else if ((prev1State == 0b00 && state == 0b10) ||
           (prev1State == 0b10 && state == 0b11) ||
           (prev1State == 0b11 && state == 0b01) ||
           (prev1State == 0b01 && state == 0b00))
    enc1Count--;

  prev1State = state;
}

// ====================== QUADRATURE ISR M2 =====================
void IRAM_ATTR enc2ISR() {
  uint8_t state = (digitalRead(M2_ENC_A) << 1) | digitalRead(M2_ENC_B);

  if ((prev2State == 0b00 && state == 0b01) ||
      (prev2State == 0b01 && state == 0b11) ||
      (prev2State == 0b11 && state == 0b10) ||
      (prev2State == 0b10 && state == 0b00))
    enc2Count++;
  else if ((prev2State == 0b00 && state == 0b10) ||
           (prev2State == 0b10 && state == 0b11) ||
           (prev2State == 0b11 && state == 0b01) ||
           (prev2State == 0b01 && state == 0b00))
    enc2Count--;

  prev2State = state;
}

// ====================== MOTOR HELPER =========================
void setMotor(int pwmPin, int dirPin, int val) {
  val = constrain(val, -255, 255);

  if (val >= 0) {
    digitalWrite(dirPin, LOW);
    analogWrite(pwmPin, val);
  } else {
    digitalWrite(dirPin, HIGH);
    analogWrite(pwmPin, -val);
  }
}

float pulsesToRpm(long pulses, int ppr4x, unsigned long dt_ms) {
  float dt = dt_ms / 1000.0f;
  return (pulses / (float)ppr4x) * (60.0f / dt);
}

// ============================= SETUP =========================
void setup() {
  Serial.begin(115200);

  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);

  pinMode(M1_ENC_A, INPUT);
  pinMode(M1_ENC_B, INPUT_PULLUP);

  pinMode(M2_ENC_A, INPUT);
  pinMode(M2_ENC_B, INPUT_PULLUP);

  prev1State = (digitalRead(M1_ENC_A) << 1) | digitalRead(M1_ENC_B);
  prev2State = (digitalRead(M2_ENC_A) << 1) | digitalRead(M2_ENC_B);

  attachInterrupt(M1_ENC_A, enc1ISR, CHANGE);
  attachInterrupt(M1_ENC_B, enc1ISR, CHANGE);
  attachInterrupt(M2_ENC_A, enc2ISR, CHANGE);
  attachInterrupt(M2_ENC_B, enc2ISR, CHANGE);

  lastTs = millis();
}

// ============================== LOOP ==========================
void loop() {

  unsigned long now = millis();
  if (now - lastTs >= Ts_ms) {

    unsigned long dt = now - lastTs;
    lastTs = now;

    long p1 = enc1Count; enc1Count = 0;
    long p2 = enc2Count; enc2Count = 0;

    float rpm1 = pulsesToRpm(p1, ppr_4x, dt);
    float rpm2 = pulsesToRpm(p2, ppr_4x, dt);

    rpmFiltered1 = rpmAlpha * rpm1 + (1 - rpmAlpha) * rpmFiltered1;
    rpmFiltered2 = rpmAlpha * rpm2 + (1 - rpmAlpha) * rpmFiltered2;

    float dt_s = dt / 1000.0f;

    // ===== Motor 1 PID =====
    err1 = setRpm1 - rpmFiltered1;
    integ1 += err1 * dt_s;
    integ1 = constrain(integ1, integMin, integMax);
    float dRaw1 = (err1 - prevErr1) / dt_s;
    dFiltered1 = dAlpha * dRaw1 + (1 - dAlpha) * dFiltered1;
    int out1 = (int)(Kp*err1 + Ki*integ1 + Kd*dFiltered1);
    prevErr1 = err1;

    // ===== Motor 2 PID =====
    err2 = setRpm2 - rpmFiltered2;
    integ2 += err2 * dt_s;
    integ2 = constrain(integ2, integMin, integMax);
    float dRaw2 = (err2 - prevErr2) / dt_s;
    dFiltered2 = dAlpha * dRaw2 + (1 - dAlpha) * dFiltered2;
    int out2 = (int)(Kp*err2 + Ki*integ2 + Kd*dFiltered2);
    prevErr2 = err2;

    // Apply outputs
    setMotor(M1_PWM, M1_DIR, out1);
    setMotor(M2_PWM, M2_DIR, out2);

    // Plotter-friendly output
    Serial.print("M1_set:"); Serial.print(setRpm1);
    Serial.print("\tM1_RPM:"); Serial.print(rpmFiltered1);
    Serial.print("\tM1_out:"); Serial.print(out1);

    Serial.print("\tM2_set:"); Serial.print(setRpm2);
    Serial.print("\tM2_RPM:"); Serial.print(rpmFiltered2);
    Serial.print("\tM2_out:"); Serial.println(out2);
  }
}

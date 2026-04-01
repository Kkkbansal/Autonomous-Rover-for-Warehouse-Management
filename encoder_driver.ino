/* *************************************************************
   Encoder definitions
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ARDUINO_ARCH_ESP32
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  
  // Interrupt Service Routines (ISRs)
  // IRAM_ATTR puts this code in RAM for speed (Crucial for ESP32)
  void IRAM_ATTR leftEncoderISR() {
    // Read Phase B to determine direction
    if (digitalRead(LEFT_ENC_PIN_B) == HIGH) {
      left_enc_pos++;
    } else {
      left_enc_pos--;
    }
  }

  void IRAM_ATTR rightEncoderISR() {
    // Read Phase B to determine direction
    if (digitalRead(RIGHT_ENC_PIN_B) == HIGH) {
      right_enc_pos++;
    } else {
      right_enc_pos--;
    }
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
  
  // Initialize the pins and interrupts
  // We will call this from setup() in the main file
  void initEncoders(){
    pinMode(LEFT_ENC_PIN_A, INPUT_PULLUP);
    pinMode(LEFT_ENC_PIN_B, INPUT_PULLUP);
    pinMode(RIGHT_ENC_PIN_A, INPUT_PULLUP);
    pinMode(RIGHT_ENC_PIN_B, INPUT_PULLUP);
    
    // Trigger interrupt when Phase A rises (goes Low to High)
    attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN_A), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_A), rightEncoderISR, RISING);
  }
  
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif

#define PIN_DATA_R1 A0
#define PIN_CLOCK_R1 2
#define PIN_LED_3_B 3

uint8_t R1[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void send_data_to_register(uint8_t* reg_vals, int data_pin, int clock_pin) {
  digitalWrite(data_pin, 0);
  digitalWrite(clock_pin, 0);

  for (int i = 7; i >= 0; i--) {
    digitalWrite(data_pin, reg_vals[i]);
    digitalWrite(clock_pin, 0);
    delay(1);
    digitalWrite(clock_pin, 1);
    delay(1);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_DATA_R1, OUTPUT);
  pinMode(PIN_CLOCK_R1, OUTPUT);
  pinMode(PIN_LED_3_B, OUTPUT);
}

void loop() {
  R1[0] = 0;
  R1[1] = 1;
  R1[2] = 1;

// pt led ul 2
  R1[3] = 0;
  R1[4] = 1;
  R1[5] = 1;

  // pt led ul 3
  R1[6] = 0;
  R1[7] = 1;
  digitalWrite(PIN_LED_3_B, 1);
  send_data_to_register(R1, PIN_DATA_R1, PIN_CLOCK_R1);
  delay(2000);

  R1[0] = 1;
  R1[1] = 1;
  R1[2] = 0;

// pt led ul 2
  R1[3] = 1;
  R1[4] = 1;
  R1[5] = 0;  
  send_data_to_register(R1, PIN_DATA_R1, PIN_CLOCK_R1);
  delay(2000);

  R1[0] = 1;
  R1[1] = 0;
  R1[2] = 1;

  // pt led ul 2
  R1[3] = 1;
  R1[4] = 0;
  R1[5] = 1;
  send_data_to_register(R1, PIN_DATA_R1, PIN_CLOCK_R1);
  delay(2000);

// pt led ul 2


}

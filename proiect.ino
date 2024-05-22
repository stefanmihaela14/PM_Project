#define PIN_DATA_R1 A0
#define PIN_CLOCK_R1 2
#define PIN_LED_3_BLUE 3
uint8_t R1[8] = {1, 1, 1, 1, 1, 1, 1, 1};

#define LED_1_RED R1[0]
#define LED_1_GREEN R1[1]
#define LED_1_BLUE R1[2]

#define LED_2_RED R1[3]
#define LED_2_GREEN R1[4]
#define LED_2_BLUE R1[5]

#define LED_3_RED R1[6]
#define LED_3_GREEN R1[7]

// for first lights mode
unsigned long startMillisJoc1;
unsigned long currentMillisJoc1;
unsigned long joc1PasLoop = 0;
unsigned long joc1ColorCount = 0;

// for second lights mode
unsigned long currentMillisJoc2;
unsigned long startMillisJoc2 = 0;
int joc2DelayTime = 500;
int blinkState = 0;
int colorIndex = 0;
int blinkCount = 0;

// for third lights mode
float maxValueForCold = 20;
float minValueForMedium = 21;
float maxValueForMedium = 28;
unsigned long startMillisJoc3;
unsigned long currentMillisJoc3;


// for temp sensor and LCD
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
 
#define DHTPIN 7
#define DHTTYPE DHT22
#define DELAY_TIME 50
 
float humidity_prev_value = 0;
float temp_prev_value = 0;
 
LiquidCrystal_I2C lcd(0x3F,16,2); 
DHT dht(DHTPIN, DHTTYPE);

// start code

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


void setLEDColor(int ledIndex, int red, int green, int blue);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_DATA_R1, OUTPUT);
  pinMode(PIN_CLOCK_R1, OUTPUT);
  pinMode(PIN_LED_3_BLUE, OUTPUT);
  startMillisJoc1 = millis();

  // for temp sensor and LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
 
  pinMode(DHTPIN, INPUT);
  pinMode(A3, INPUT);
  dht.begin();
  Serial.begin(9600);

  setLEDColor(1,1,1,1);
  setLEDColor(2,1,1,1);
  setLEDColor(3,1,1,1);


}

// Array of colors (R, G, B)
int colors[][3] = {
  {0, 1, 1},   // Red
  {1, 0, 1},   // Green
  {1, 1, 0},   // Blue
  {0, 0, 1},  // Yellow
  {0, 1, 0},  // Magenta
  {1, 0, 0},  // Cyan
  {0, 0, 0}   // White (all off in this case)
};

int colorCount = 7; // Number of colors in the array
int delayTime = 500; // Time to wait between color changes (milliseconds)

// Function to set the color of the RGB LEDs
void setLEDColor(int ledIndex, int red, int green, int blue) {
  switch (ledIndex) {
    case 1:
      LED_1_RED = red;
      LED_1_GREEN = green;
      LED_1_BLUE = blue;
      break;
    case 2:
      LED_2_RED = red;
      LED_2_GREEN = green;
      LED_2_BLUE = blue;
      break;
    case 3:
      LED_3_RED = red;
      LED_3_GREEN = green;
      digitalWrite(PIN_LED_3_BLUE, blue);
      break;
  }
  send_data_to_register(R1, PIN_DATA_R1, PIN_CLOCK_R1);
}

void lights_mode_1() {
    currentMillisJoc1 = millis();
    if (currentMillisJoc1 - startMillisJoc1 >= delayTime) {
      Serial.println("Schimbare!");
      startMillisJoc1 = currentMillisJoc1;

      if (joc1PasLoop == 0) {
        setLEDColor(1, colors[joc1ColorCount][0], colors[joc1ColorCount][1], colors[joc1ColorCount][2]);
      }
      else if(joc1PasLoop == 1) {
        setLEDColor(2, colors[joc1ColorCount][0], colors[joc1ColorCount][1], colors[joc1ColorCount][2]);
      }
      else {
        setLEDColor(3, colors[joc1ColorCount][0], colors[joc1ColorCount][1], colors[joc1ColorCount][2]);
        joc1ColorCount = (joc1ColorCount + 1) % 7;
      }

      joc1PasLoop = (joc1PasLoop + 1) % 3;
    }
}

void lights_mode_2() {
  currentMillisJoc2 = millis();

  if (currentMillisJoc2 - startMillisJoc2 >= joc2DelayTime) {
    startMillisJoc2 = currentMillisJoc2;

    if (blinkState == 0) {
      // turn on all LEDs with the current color
      setLEDColor(1, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
      setLEDColor(2, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
      setLEDColor(3, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
      // to turn off LEDs
      blinkState = 1;
    } else {
      // turn off all LEDs
      setLEDColor(1, 1, 1, 1);  // Off
      setLEDColor(2, 1, 1, 1);  // Off
      setLEDColor(3, 1, 1, 1);  // Off
      // to turn on LEDs with the next color
      blinkState = 0;

      colorIndex = (colorIndex + 1) % colorCount;

      // for to blink each 2 seconds
      blinkCount++;
      if (blinkCount >= 2 * colorCount) {
        blinkCount = 0;
        colorIndex = 0;
      }
    }
  }
}

void lights_mode_3(float tempValue) {
    currentMillisJoc3 = millis();
    if (currentMillisJoc3 - startMillisJoc3 >= delayTime) {
      startMillisJoc3 = currentMillisJoc3;

      if (tempValue <= maxValueForCold) {
        setLEDColor(1, 1, 1, 0);
        setLEDColor(2, 1, 1, 0);
        setLEDColor(3, 1, 1, 0);
      }
      else if(tempValue >= minValueForMedium && tempValue <= maxValueForMedium) {
        setLEDColor(1, 0, 0, 1);
        setLEDColor(2, 0, 0, 1);
        setLEDColor(3, 0, 0, 1);
      }
      else {
        setLEDColor(1, 0, 1, 1);
        setLEDColor(2, 0, 1, 1);
        setLEDColor(3, 0, 1, 1);
      }
    }
}

int lightsMode = 0;
bool shouldLight = false;

int hours = 0;
int minutes = 0;
int seconds = 0;

bool alarmSet = false;
int Ahours = 0;
int Aminutes = 0;
int Aseconds = 0;

long lastUpdatedTime = 0;

long alarmTime = 60000;
long alarmStartTime;
bool shouldStopAlarm = false;

void takeUserInput() {

  if (Serial.available()) {  // Avem input! yay
    int command = Serial.read();

    switch (command) {
      case 'C':
      Serial.print("Changing time.");
      hours = Serial.parseInt(SKIP_NONE);
      Serial.read();
      minutes = Serial.parseInt(SKIP_NONE);
      Serial.read();
      seconds = Serial.parseInt(SKIP_NONE);
      break;
      case 'A':
      alarmSet = true;
      Serial.print("Changing alarm.");
      Ahours = Serial.parseInt(SKIP_NONE);
      Serial.read();
      Aminutes = Serial.parseInt(SKIP_NONE);
      Serial.read();
      Aseconds = Serial.parseInt(SKIP_NONE);
      break;
      case 'R':
      alarmSet = false;
      break;
      case 'M':
      lightsMode = Serial.parseInt(SKIP_NONE) - 1;
      case 'S':
      shouldLight = false;
      shouldStopAlarm = false;
      break;
    }
  }
}

void loop() {
  takeUserInput();
  long currTime = millis();
  float temperature = dht.readTemperature();

  if (shouldStopAlarm && currTime - alarmStartTime >= alarmTime) {
    shouldStopAlarm = false;
    shouldLight = false;
  }

  if (shouldLight) {
    switch(lightsMode) {
      case 0:
        lights_mode_1();
      break;
      case 1:
        lights_mode_2();
      break;
      case 2:
        lights_mode_3(temperature);
      break;
    }
  } else {
    setLEDColor(1,1,1,1);
    setLEDColor(2,1,1,1);
    setLEDColor(3,1,1,1);
  }

  bool shouldUpdateLCD = false;
  
  if (temperature != temp_prev_value) {
    shouldUpdateLCD = true;
    temp_prev_value = temperature;
  }

  if (currTime - lastUpdatedTime > 1000) {
    lastUpdatedTime = currTime;

    seconds++;

    if (seconds == 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes == 60) {
      minutes = 0;
      hours++;
    }
    if (hours == 24) {
      hours = 0;
    }
    shouldUpdateLCD = true;

    if (alarmSet && hours == Ahours && minutes == Aminutes && seconds == Aseconds) {
      shouldLight = true;
      shouldStopAlarm = true;
      alarmStartTime = currTime;
    }
  }

  if (shouldUpdateLCD) {
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print(String((int)temperature) + "C");
    lcd.setCursor(0, 0);
    lcd.print("C " + String(hours) + ":" + String(minutes) + ":" + String(seconds));
    if (alarmSet) {
      lcd.setCursor(0, 1);
      lcd.print("A " + String(Ahours) + ":" + String(Aminutes) + ":" + String(Aseconds));
    }
    shouldUpdateLCD = false;
  }

}
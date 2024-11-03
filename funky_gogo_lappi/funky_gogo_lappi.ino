#include <LiquidCrystal_I2C.h>

// HC_SR04 - ultrasonic distance sensor module
#define HC_SR04_TRIGGER_PIN 12
#define HC_SR04_ECHO_PIN 13

// light pins
#define GREEN_1_LED_PIN 2
#define GREEN_2_LED_PIN 3
#define YELLOW_1_LED_PIN 4
#define YELLOW_2_LED_PIN 5
#define RED_LED_PIN 6

struct LED {
  // min distance for LED to light up
  const long MIN_DISTANCE;
  const int PIN;
};

// LEDs signalizing how close is target
const int NUMBER_OF_LEDS = 5;
LED red_led = {5, RED_LED_PIN};
LED yellow_2_led = {10, YELLOW_2_LED_PIN};
LED yellow_1_led = {15, YELLOW_1_LED_PIN};
LED green_2_led = {20, GREEN_2_LED_PIN};
LED green_1_led = {10000, GREEN_1_LED_PIN};

LED leds[NUMBER_OF_LEDS] = {green_1_led, green_2_led, yellow_1_led, yellow_2_led, red_led};

LiquidCrystal_I2C lcd(0x27, 16, 2);

long distance = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  pinMode(HC_SR04_TRIGGER_PIN, OUTPUT);
  pinMode(HC_SR04_ECHO_PIN, INPUT);
}

void loop() {
  // read distance
  digitalWrite(HC_SR04_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(HC_SR04_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC_SR04_TRIGGER_PIN, LOW);

  long miliseconds = pulseIn(HC_SR04_ECHO_PIN, HIGH);

  distance = microseconds_to_centimeters(miliseconds);

  // print distance on LCD display
  lcd.clear();
  String text = String(distance) + " cm";
  lcd.print(text);

  // manage LEDs

  for (int i=0; i<NUMBER_OF_LEDS; i++) {
    if (leds[i].MIN_DISTANCE > distance) {
      digitalWrite(leds[i].PIN, HIGH);
    }
    else {
      digitalWrite(leds[i].PIN, LOW);
    }
  }

  delay(300);
}

// calculate the distance based on how long it took for signal to come back, more information in hc_sr04 datasheet
long microseconds_to_centimeters(long miliseconds) {
  return miliseconds / 58;
}

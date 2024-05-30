
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int nabiz;

const int PulseWire = 0; 
const int LED13 = 13;
int Threshold = 510;
PulseSensorPlayground pulseSensor;

void setup() {
  
  Serial.begin(9600);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.drawPixel(10, 10, WHITE);

  display.display();
  delay(2000);

  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(17, 0);
  display.println(F("Nabziniz"));
  display.display();
  delay(100);
 
  
  if (pulseSensor.begin()) {
    Serial.println("Pulse sensörü objesini yarattık."); 
  }

}

void loop() {
  if (pulseSensor.sawStartOfBeat()) {
    nabiz = pulseSensor.getBeatsPerMinute();

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(17, 0);
    display.println(F("Nabziniz"));
    display.setCursor(50, 30);
    display.println(nabiz);
    display.display();
    delay(100);

    Serial.println("Nabız attı. ");
    Serial.print("BPM: ");                        
    Serial.println(nabiz);
  }
  delay(20);
}
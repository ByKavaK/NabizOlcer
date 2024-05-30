#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String agAd = "";
String sifre = "";
String ip = "";
int rxPin = 10;
int txPin = 11;
int nabiz;

const int PulseWire = 0; 
const int LED13 = 13;
int Threshold = 510;
PulseSensorPlayground pulseSensor;

SoftwareSerial esp(rxPin, txPin);

void setup() {
  
  Serial.begin(9600);

  Serial.println("Haberleşme Başladı.");
  esp.begin(115200);
  esp.println("AT");
  Serial.println("AT Geldi.");
  while(!esp.find("OK")){
    esp.println("AT");
    Serial.println("ESP Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                          
  while(!esp.find("OK")){                                   
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAd+"\",\""+sifre+"\"");    
  while(!esp.find("OK"));                                     
  Serial.println("Aga Baglandi.");
  delay(1000);

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
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           
  if(esp.find("Error")){                                      
    Serial.println("AT+CIPSTART Error");
  }

  if (pulseSensor.sawStartOfBeat()) {
    nabiz = pulseSensor.getBeatsPerMinute();

    String veri = "GET https://api.thingspeak.com/update?api_key=";                                     
    veri += "&field1=";
    veri += String(nabiz);                                      
    veri += "\r\n\r\n"; 
    esp.print("AT+CIPSEND=");                                   
    esp.println(veri.length()+2);
    delay(2000);
    if(esp.find(">")){                                          
      esp.print(veri);                                         
      Serial.println(veri);
      Serial.println("Veri gonderildi.");
      delay(1000);
    }

    Serial.println("Baglantı Kapatildi.");
    delay(1000);

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
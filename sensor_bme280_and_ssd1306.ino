#include <BME280I2C.h>
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans12pt7b.h>

#define SERIAL_BAUD 9600
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

/* ==== Global Variables ==== */
BME280I2C bme;                  // Default : forced mode, standby time = 1000 ms
bool metric = true;
float tempc, humd, pre;

/* ==== Setup ==== */
void setup() {
  Serial.begin(SERIAL_BAUD);
  
  while(!Serial) {} // Wait

  Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize ssd1306 with the I2C addr 0x3D (128x64)
  display.clearDisplay(); // Clear the buffer.
  display.setRotation(0);
}

void loop() {
   printBME280Data(&Serial);
   refresh();
   delay(500);
}

void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Fahrenheit);
   BME280::PresUnit presUnit(BME280::PresUnit_inHg);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println(" Pa");
   tempc = temp;
   humd = hum;
   pre = pres;

   delay(500);
}

void refresh(){
  display.setFont(&FreeSans12pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0,18);
  display.print(tempc,1);
  display.println(" F");

  display.setCursor(0,42);
  display.print(humd,1);
  display.println("% RH");
  
  display.setFont();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,50);
  display.print(pre,1);
  display.print(" inHg");

  display.setCursor(90,0);
  display.println("FF lab");
   display.setCursor(90,10);
  display.println("rocks!");
  
  display.display();
}
/* ==== END Functions ==== */

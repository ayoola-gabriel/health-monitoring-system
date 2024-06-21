#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "home_img.h"

#define WHITE SH110X_WHITE
#define BLACK SH110X_BLACK

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void beginDisplay()
{
  delay(250);                       // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
                                    // display.setContrast (0); // dim display
  display.clearDisplay();
}

void displayON(bool s)
{
  if (s)
  {
    display.oled_command(SH110X_DISPLAYON);
  }
  else
  {
    display.oled_command(SH110X_DISPLAYOFF);
  }
}

void drawRSSI(uint8_t rssi, int16_t x, int16_t y)
{

  // display.drawTriangle(0, 0, 3, 3, 6, 0, WHITE);
  display.drawTriangle(x, y, x + 3, y + 3, x + 6, y, WHITE);
  display.drawLine(x + 3, y, x + 3, y + 8, WHITE);
  if ((rssi >= -110) && (rssi < -105))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
  }
  else if ((rssi >= -105) && (rssi < -97))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
  }
  else if ((rssi >= -97) && (rssi < -90))
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
    display.drawRect(x + 14, y + 3, 2, 6, WHITE);
  }
  else if (rssi >= -90)
  {
    display.drawRect(x + 6, y + 7, 2, 2, WHITE);
    display.drawRect(x + 10, y + 5, 2, 4, WHITE);
    display.drawRect(x + 14, y + 3, 2, 6, WHITE);
    display.drawRect(x + 18, y + 1, 2, 8, WHITE);
  }
}

#define wifi_height 11
#define wifi_width 15

static const unsigned char PROGMEM noWiFiBmp[] = {
    B00000111, B11000000,
    B00011000, B00110000,
    B01100000, B00001100,
    B10000000, B00000010,
    B01000000, B00000100,
    B00100000, B00001000,
    B00010000, B00010000,
    B00001000, B00100000,
    B00000100, B01000000,
    B00000010, B10000000,
    B00000001, B00000000,
};

static const unsigned char PROGMEM wiFiBmp[] = {
    B00000111, B11000000,
    B00011111, B11110000,
    B01111111, B11111100,
    B11111111, B11111110,
    B01111111, B11111100,
    B00111111, B11111000,
    B00011111, B11110000,
    B00001111, B11100000,
    B00000111, B11000000,
    B00000011, B10000000,
    B00000001, B00000000,
};

void drawWifi(bool s, int16_t xpos, int16_t ypos)
{
  display.fillRect(xpos, ypos, wifi_width, wifi_height, BLACK);
  if (s)
  {
    display.drawBitmap(xpos, ypos, wiFiBmp, wifi_width, wifi_height, WHITE);
  }
  else
  {
    display.drawBitmap(xpos, ypos, noWiFiBmp, wifi_width, wifi_height, WHITE);
  }
  display.display();
}

static const unsigned char PROGMEM heart_bmp[] = {
    B00111000, B00111000,
    B01111110, B11111100,
    B11111111, B11111110,
    B11111111, B11111110,
    B01111111, B11111100,
    B00111111, B11111000,
    B00011111, B11110000,
    B00001111, B11100000,
    B00000111, B11000000,
    B00000011, B10000000,
    B00000001, B00000000,
};

static const unsigned char PROGMEM no_heart_bmp[] = {
    B00111000, B00111000,
    B01000010, B10000100,
    B10000001, B10000010,
    B10000000, B00000010,
    B01000000, B00000100,
    B00100000, B00001000,
    B00010000, B00010000,
    B00001000, B00100000,
    B00000100, B01000000,
    B00000010, B10000000,
    B00000001, B00000000,
};

void drawHeart(bool s, int16_t xpos, int16_t ypos){
  display.fillRect(xpos,ypos,wifi_width,wifi_height,BLACK);
  if (s){
    display.drawBitmap(xpos, ypos, heart_bmp, wifi_width, wifi_height, WHITE);
  } else {
    display.drawBitmap(xpos, ypos, no_heart_bmp, wifi_width, wifi_height, WHITE);
  }
  display.display();
}

bool drawInfo(char *info)
{
  display.fillRect(0,0,100,9,BLACK);
  display.setCursor(5, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.print(info);

  // draw a Horizontal line
  display.drawLine(0, 10, 100, 10, WHITE);
  display.display();
  return true;
}

void drawBaseLine(int16_t length)
{
  // display.drawLine(0, 0, length, 63, WHITE);
  display.drawLine(0, 63, length, 63, WHITE);
  display.display();
}

void drawBatteryLevel(uint16_t vbat)
{
  display.fillRect(109, 0, 18, 9, BLACK);
  display.drawRect(109, 0, 18, 9, WHITE);
  display.drawLine(107, 2, 107, 6, WHITE);
  if ((vbat >= 5) && (vbat < 25))
  {
    display.drawRect(123, 2, 2, 5, WHITE);
  }
  else if ((vbat >= 25) && (vbat < 50))
  {
    display.drawRect(123, 2, 2, 5, WHITE);
    display.drawRect(119, 2, 2, 5, WHITE);
  }
  else if ((vbat >= 50) && (vbat < 75))
  {
    display.drawRect(123, 2, 2, 5, WHITE);
    display.drawRect(119, 2, 2, 5, WHITE);
    display.drawRect(115, 2, 2, 5, WHITE);
  }
  else if (vbat >= 75)
  {
    display.drawRect(123, 2, 2, 5, WHITE);
    display.drawRect(119, 2, 2, 5, WHITE);
    display.drawRect(115, 2, 2, 5, WHITE);
    display.drawRect(111, 2, 2, 5, WHITE);
  }
  display.display();
}

void drawBP(uint8_t sysBP, uint8_t diaBP)
{
  display.fillRect(0,30,110,56,BLACK);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 19);
  display.println("BP");
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print(sysBP);
  display.print("/");
  display.print(diaBP);
  display.setTextSize(1);
  display.print("mmHg");
  display.display();
}

void drawHR(int hr, int temp)
{
  char deg = 0xF8;
  // display heart rate and temperature
  display.fillRect(0,56,127,7,BLACK);
  display.setCursor(0, 56);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print(hr);
  display.print("bpm");
  display.setCursor(100, 56);
  display.print(temp);
  display.setCursor(115, 53);
  display.print(deg);
  display.setCursor(120, 56);
  display.print("C");
  display.display();
}

void showScreen()
{
  display.display();
}

void clearScreen()
{
  display.clearDisplay();
}

void displaySetUp()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Setup mode!\n");
  display.println("Address: hms.local\n");
  display.println("IP: 192.168.4.1");
}

void displayWelcome()
{
  display.drawBitmap(0, 0, homeImg, 128, 64, WHITE);
}

void formatScreen()
{
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("RESETING...\n");
  display.println('Please Wait...');
  display.display();
}
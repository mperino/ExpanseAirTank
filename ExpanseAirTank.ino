/***************************************************
  This is our GFX example for the Adafruit ILI9341 TFT FeatherWing
  ----> http://www.adafruit.com/products/3315

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef ESP32
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif
#if defined(ARDUINO_NRF52832_FEATHER) || defined(ARDUINO_NRF52_FEATHER) /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#endif
#if defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define TFT_DC   P5_4
   #define TFT_CS   P5_3
   #define STMPE_CS P3_3
   #define SD_CS    P3_2
#endif

// Anything else!
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__) || \
    defined(ARDUINO_SAMD_ZERO) || defined(__SAMD51__) || defined(__SAM3X8E__) || defined(ARDUINO_NRF52840_FEATHER)
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

unsigned long countdown;
  
void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  delay(1000);
  tft.fillScreen(ILI9341_BLACK);
  yield();
  delay(1000);
  tft.setRotation(0);
  bootText();
  delay(5000);
  countdown = 0;
  

}


void loop(void) {
    countdown= countdown +1;
    tft.setRotation(0);
    airLines(ILI9341_WHITE);
    airText();
    airStatusBar(ILI9341_GREEN);
    depletionBars(countdown);
    delay(60000);
    tft.fillScreen(ILI9341_BLACK);
    delay(10);
  
}



unsigned long bootText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("Booting Pn'K PackOS");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println("Ver 19.2.1");
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println("IH Canturbury");
  tft.println();
  return micros() - start;
}

unsigned long airText() {
  unsigned long start = micros();
  tft.setRotation(3);
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("AL BR ST HR BP 02");
  tft.println();
  return micros() - start;
}


unsigned long airLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = x2 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x1=0; x1<w; x1+=w/6) tft.drawLine(x1, y1, x1, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing
  yield();
  
}

unsigned long airStatusBar(uint16_t color) {
  unsigned long start, t;
  int           x1,
                w = tft.width(),
                h = tft.height();
  
  x1 = 0;

  tft.setRotation(0);
//  tft.println(h);
  start = micros();
// bottom bars
  for(x1=7*w/48; x1<w; x1+=6*w/48) tft.fillRect(x1, 23*w/24, w/12, h/12, color);
//  tft.fillRect(7*w/48, y1, w/12, h/12, color);
//  tft.fillRect(13*w/48, y1, w/12, h/12, color);
//  tft.fillRect(19*w/48, y1, w/12, h/12, color);
//  tft.fillRect(25*w/48, y1, w/12, h/12, color);
//  tft.fillRect(31*w/48, y1, w/12, h/12, color);
// top bars
  for(x1=7*w/48; x1<w; x1+=6*w/48) tft.fillRect(x1, 0, w/12, h/12, color);
  t     = micros() - start; // fillScreen doesn't count against timing
  yield();
  
}

unsigned long depletionBars(uint16_t count) {
  unsigned long start, t;
  int           x1,
                w = tft.width(),
                h = tft.height(),
                y1 = 22*h/24;
  
  x1 = 0;

  tft.setRotation(0);
//  tft.println(h);
  start = micros();
// depleting bars
  tft.fillRect(18*w/48, y1, w/12, -.8*h*pow(.99,count),ILI9341_CYAN);
  tft.fillRect(26*w/48, y1, w/12, -.5*h*pow(.99,count),ILI9341_YELLOW);
  tft.fillRect(34*w/48, y1, w/12, -.85*h*pow(.99,count),ILI9341_CYAN);
  tft.fillRect(42*w/48, y1, w/12, -.35*h*pow(.99,count),ILI9341_RED);
//  tft.fillRect(19*w/48, y1, w/12, h/12, color);
//  tft.fillRect(25*w/48, y1, w/12, h/12, color);
//  tft.fillRect(31*w/48, y1, w/12, h/12, color);
// top bars

  t     = micros() - start; // fillScreen doesn't count against timing
  yield();
  
}


unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }

  return t;
}

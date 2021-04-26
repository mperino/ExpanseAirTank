/***********************************************************************
 * Based on the fine examples and headers from Adafruit
 * Written (poorly) by Mark Perino 
 * 
 * Modified by Dan Shope April 2021
 * Removed unused returns, libraries, and cleaned up timing.
 * Changed graphics to only re-draw bar areas.
 * Added a fifth bar for activity level, with random width
 * 
 * Extending the MIT license, and since it's based on 
 * existing Adafruit code, you must include their text bellow

 *  This is our GFX example for the Adafruit ILI9341 TFT FeatherWing
 *  ----> http://www.adafruit.com/products/3315
 *
 *  Check out the links above for our tutorials and wiring diagrams
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *  MIT license, all text above must be included in any redistribution
 ************************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <gfxfont.h>

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
  delay(10); //let screen fully boot

  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  yield();
  delay(1000);
  
  tft.fillScreen(ILI9341_BLACK);
  yield();
  delay(1000);
  
  tft.setRotation(0);
  drawBootText();
  delay(5000);
  countdown = 0;

  tft.fillScreen(ILI9341_BLACK);
  yield();

  //draw static parts
  drawLines(ILI9341_WHITE);
  drawLabels(ILI9341_WHITE);
  drawStatusBar(ILI9341_GREEN);
}


void loop(void) {
    countdown++; //increment by one
    
    tft.setRotation(0);
    drawDepletionBars(countdown, true);

    //****** ANIMATION TIMER *******/
    //wait before continuing to count
    delay(1000); //was 60000 for one minute
    //****** END ANIMATION TIMER *******/

    //animation reset after 6 hours (if delay is 60000)
    if(countdown > 360) {
      countdown = 0;
    }
}

void drawBootText() {
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("Booting Pn'K PackOS");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println("Ver 19.2.1");
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println("IH Canterbury"); //fixed spelling
}

void drawLabels(uint16_t color) {
  tft.setRotation(3);
  tft.setCursor(10, 10);
  tft.setTextColor(color);  tft.setTextSize(3);
  tft.println("AL BR ST HR BP 02");
}


void drawLines(uint16_t color) {
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.setRotation(0);
  x1 = x2 = y1 = 0;
  y2 = h - 1;
  for(x1=0; x1<w; x1+=w/6) {
    tft.drawLine(x1, y1, x1, y2, color);
  }
  yield();
}

void drawStatusBar(uint16_t color) {
  int           x1,
                w = tft.width(),
                h = tft.height();
  x1 = 0;

  tft.setRotation(0);
  // bottom bars
  for(x1=(int)(7*w/48); x1<w; x1+=(int)(6*w/48)) {
    tft.fillRect(x1, 23*w/24, w/12, h/12, color);
    yield();
  }
  
  // top bars
  for(x1=(int)(7*w/48); x1<w; x1+=(int)(6*w/48)) {
    tft.fillRect(x1, 0, w/12, h/12, color);
    yield();
  }
}

void drawDepletionBars(uint16_t count, bool clearScreen) {
  int           maxw,
                w = tft.width(),  //240
                h = tft.height(), //320
                y1 = 22*h/24,
                bh = w/48,  //scalar for x0 coord
                bw = w/12;  //bar thickness (fixed)
  
  maxw = -0.85 * h; //pow(0.99,0) = 1 //max bar width [-0.85 * h * 0.99^count] = 272px?

  tft.setRotation(0);

  if(clearScreen) tft.fillRect(10*bh, y1, bw, maxw, ILI9341_BLACK);  //erase last block
  tft.fillRect(10*bh, y1, bw, -1*random(50,-1*maxw),ILI9341_YELLOW); // make the bar yellow
  
  if(clearScreen) tft.fillRect(18*bh, y1, bw, maxw, ILI9341_BLACK);  //erase last block
  if ( count <= 68) {
    tft.fillRect(18*bh, y1, bw, -.8*h*pow(.99,count),ILI9341_CYAN);   // make the bar cyan
  } else if ( count <=137) {
    tft.fillRect(18*bh, y1, bw, -.8*h*pow(.99,count),ILI9341_YELLOW); // make the bar yellow
  } else {
    tft.fillRect(18*bh, y1, bw, -.8*h*pow(.99,count),ILI9341_RED);    // make the bar red
  }
  yield();

  if(clearScreen) tft.fillRect(26*bh, y1, bw, maxw, ILI9341_BLACK);   //erase last block
  tft.fillRect(26*bh, y1, bw, -.5*h*pow(.99,count),ILI9341_YELLOW);
  yield();

  if(clearScreen) tft.fillRect(34*bh, y1, bw, maxw, ILI9341_BLACK);   //erase last block
  tft.fillRect(34*bh, y1, bw, -.85*h*pow(.99,count),ILI9341_CYAN);
  yield();

  if(clearScreen) tft.fillRect(42*bh, y1, bw, maxw, ILI9341_BLACK);   //erase last block
  tft.fillRect(42*w/48, y1, bw, -.35*h*pow(.99,count),ILI9341_RED);
  yield();
}

#include <stdbool.h>
#include <bsp.h>
#include <buttons.h>
#include <timers.h>
#include <leds.h>
#include <lcd.h>
#include <delay.h>

enum controlData {N_ITERATIONS = 2500};

void displayStats(uint32_t, uint32_t, uint32_t, uint32_t);

void main(void) {
  uint32_t timeElapsed = 0;
  uint32_t minTime = 0xFFFFFFFF;
  uint32_t meanTime = 0;
  uint32_t maxTime = 0;
  uint32_t totalTime = 0;
  
  bspInit();
  initWatch();
  while (true) {
    startWatch();
    timeElapsed = stopWatch();
    lcdSetTextPos(2,1);
    lcdWrite("NOP  : %011u", timeElapsed);
    minTime = 0xFFFFFFFF;
    meanTime = 0;
    maxTime = 0;
    totalTime = 0;
    displayStats(minTime, meanTime, maxTime, totalTime);
    for (int i = N_ITERATIONS; i > 0; i-=1) {
      startWatch();
      /********* Software under test *********************/
      lcdSetTextPos(9,9);
      // lcdWrite("Hello World");
      lcdWrite("%011u", totalTime);
      /********* End Software under test ****************/
      timeElapsed = stopWatch();
      if (timeElapsed < minTime) {
        minTime = timeElapsed;
      }
      if (timeElapsed > maxTime) {
        maxTime = timeElapsed;
      }
      totalTime += timeElapsed;
    }
    meanTime = totalTime / N_ITERATIONS;
    displayStats(minTime, meanTime, maxTime, totalTime);
    ledToggle(USB_LINK_LED);
    while (updateButtonState(buttonsRead(), BUT_1) != B_PRESSED_RELEASED) {
      // wait for button press and release
    }
  }
}

void displayStats(uint32_t minTime,
                  uint32_t meanTime,
                  uint32_t maxTime,
                  uint32_t totalTime) {
  lcdSetTextPos(2,2);
  lcdWrite("Min  : %011u", minTime);
  lcdSetTextPos(2,3);
  lcdWrite("Mean : %011u", meanTime);
  lcdSetTextPos(2,4);
  lcdWrite("Max  : %011u", maxTime);
  lcdSetTextPos(2,5);
  lcdWrite("Tot  : %011u", totalTime);
}

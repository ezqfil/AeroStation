#include <Arduino.h>
#include <EEPROM.h>
#include "UserConfiguration.h"
#ifdef DEBUG
#include <MemoryFree.h>
#endif
#include "AeroStation.h"

#include <Servo.h>
#include "Traker.h"

#ifdef TELEMETRY_TEST
#include <AQ_RSCode.h>
#include "TelemetryTest.h"
#endif

#include "SlowTelemetry.h"
#include <LiquidCrystal.h>
#include "UIUserInterface.h"
#include "UIPageConfig.h"
#include "UIPageData.h"

#include "UIPageTest.h"




void initializeAeroStation(){
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  SERIAL_BEGIN(BAUD);
}


void setup()
{
  initializeAeroStation();
  readConfiguration();
  initializeTraker();
  initializeTelemetry();
  initializeUI();
  digitalWrite(LED_Green, HIGH);
  previousTime = micros();

}


/*******************************************************************
 * // tasks (microseconds of interval)
 * (   5000); // 200hz
 * (  10000); // 100hz
 * (  20000); //  50hz
 * (  40000); //  25hz
 * ProcessTelem    processTelem  ( 100000); //  10Hz
 * 
 * Task *tasks[] = {&readGyro, &readAccel, &runDCM, &flightControls,   \
 * &readReceiver, &readBaro, &readCompass,            \
 * &processTelem, &readBattery};
 * 
 * TaskScheduler sched(tasks, NUM_TASKS(tasks));
 * 
 * sched.run();
 *******************************************************************/
void loop()
{
  currentTime = micros();
  deltaTime = currentTime - previousTime;

  // Main scheduler loop set for 100hz
  if (deltaTime >= 10000) {

    frameCounter++;

    // ================================================================
    // 100hz task loop
    // ================================================================
    if (frameCounter % TASK_100HZ == 0) {  //  100 Hz tasks
      updateSlowTelemetry();
    }

    // ================================================================
    // 50hz task loop
    // ================================================================
    if (frameCounter % TASK_50HZ == 0) {  //  50 Hz tasks
      updatePosition();
    }

    // ================================================================
    // 50hz task loop
    // ================================================================
    if (frameCounter % TASK_10HZ == 0) {  //  10 Hz tasks
      updateUI();
      //debug output to usb Serial
#ifdef DEBUG
      debug();
#endif
    }

    previousTime = currentTime;
  }
  if (frameCounter >= 100) {
    frameCounter = 0;
  }
}






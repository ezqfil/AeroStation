#include <Arduino.h>
#include <EEPROM.h>
#include "UserConfiguration.h"

#if defined DEBUG_LOG
#include <MemoryFree.h>
#endif

#include "AeroStation.h"
#include <Servo.h>
#include "Traker.h"

#if defined BATTERY_TEST
#include "Battery.h"
#endif


#if defined  TELEMETRY_TEST
#include <AQ_RSCode.h>
#include "TelemetryTest.h"
#endif

#if defined(TELEMETRY_PROXY)
#include "TelemetryProxy.h"
#endif

#include "SlowTelemetry.h"

//#if defined(RSSI_TRACKER)
//#include "RSSI.h"
//#endif

#include <LiquidCrystal.h>
#include "UIUserInterface.h"

void initializeAeroStation() {
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  DEBUG_BEGIN(BAUD);
}


void setup()
{
  initializeAeroStation();
  readConfiguration();
  initializeTraker();
  initializeTelemetry();
//#if defined(RSSI_TRACKER)
//  initializeRSSI();
//#endif
#if defined(TELEMETRY_PROXY)
  initializeTelemetryProxy();
#endif
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
//#if defined(RSSI_TRACKER)
//      updateRSSI();
//#endif
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
#if defined BATTERY_TEST
      measureBatteryVoltage();
#endif
#if defined(TELEMETRY_PROXY)
      updateProxyTelemetry();
#endif
      //debug output to usb Serial
#if defined  DEBUG_LOG
      debug_log();
#endif
    }

    previousTime = currentTime;
  }
  if (frameCounter >= 100) {
    frameCounter = 0;
  }
}






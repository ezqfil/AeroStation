#ifndef _AS_BATTERY_
#define _AS_BATTERY_

boolean isBattLow;
//float battVoltage;

#define BATTERY_PIN A2

//Battery Monitor tuning
#define BATT_AREF_VALUE          5.0
#define BATT_RESISTOR_HIGH       10.1
#define BATT_RESISTOR_LOW        1.48

void measureBatteryVoltage() {
  home.bat_voltage = analogRead(BATTERY_PIN)  * BATT_AREF_VALUE  / 1024.0;
  home.bat_voltage /= (BATT_RESISTOR_LOW / (BATT_RESISTOR_HIGH + BATT_RESISTOR_LOW));

  if (home.bat_voltage < 6.6) isBattLow = true;
  else isBattLow = false;
}

#endif

#ifndef _AS_AEROSTATION_
#define _AS_AEROSTATION_


// Station Software Version
#define SOFTWARE_VERSION 0x0001

#ifndef CONFIG_VERSION
#define CONFIG_VERSION 0x0001 // Changing this will reset eeprom to default values
#endif

#define TASK_100HZ 1
#define TASK_50HZ 2
#define TASK_10HZ 10

// main loop executive frame counter
unsigned long frameCounter = 0; 

// main loop time variable
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long deltaTime = 0;

/**
 * Serial communication global declaration
 */

#define toRad(x) (x*PI)/180.0
#define toDeg(x) (x*180.0)/PI
#define ledON(x) digitalWrite(x, HIGH);
#define ledOFF(x) digitalWrite(x, LOW);


#define BAUD 57600
#define BAUD_TELEMETRY 1200
//#define BAUD_TELEMETRY 57600

#define BUZZER_Pin 14 //Pin mode as output to control buzzer (analog0)

#define LED_Green 13
#define LED_Red 12
#define LED_Yellow 11

#define PAM_Pin 9
#define TILT_Pin 10

#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
#define SERIAL_PORT Serial
#define TELEMETRY_PORT Serial2
#else    // force 328p to use the normal port
#define SERIAL_PORT Serial
#endif


#define SERIAL_PRINT      SERIAL_PORT.print
#define SERIAL_PRINTLN    SERIAL_PORT.println
#define SERIAL_AVAILABLE  SERIAL_PORT.available
#define SERIAL_READ       SERIAL_PORT.read
#define SERIAL_FLUSH      SERIAL_PORT.flush
#define SERIAL_BEGIN      SERIAL_PORT.begin

#define TELEMETRY_BEGIN      TELEMETRY_PORT.begin
#define TELEMETRY_AVAILABLE  TELEMETRY_PORT.available
#define TELEMETRY_READ       TELEMETRY_PORT.read
#define TELEMETRY_WRITE      TELEMETRY_PORT.write

typedef struct {
  long  latitude;
  long  longitud;
  int   altitude;
  int   alt_relative;
  float distance;
  float distance3d;
} 
t_HOME_DATA;

typedef struct {
  long latitude;
  long longitud;
  int  altitude;
  byte speed;
  byte bat_voltage;
  byte bat_current;
  int  bat_capacity;
  byte rssi;
  int pitch;
  int roll;
  int heading;
} 
t_UAV_DATA;


//Configuration stored in EEprom
typedef struct // 28 bytes
{
  int config_crc;
  int pan_minpwm;
  int pan_minangle; 
  int pan_maxpwm;
  int pan_maxangle;
  int tilt_minpwm;
  int tilt_minangle;
  int tilt_maxpwm;
  int tilt_maxangle;
  byte baudrate;
  //    int telemetry;
  //    int bearing;
  //    uint8_t osd_enabled;
  //    uint8_t bearing_method;
  //    uint16_t voltage_ratio;
} 
t_CONFIG;

t_CONFIG cnf;

t_HOME_DATA home = {
  0,0,0,0,0,0};
t_UAV_DATA uav =   {
  0,0,0,0,0,0,0,0,0,0,0};

boolean telemetry_ok=false;

#ifndef BAUDRATE56K
#define BAUDRATE56K 57600 
#endif

const long baudrates[8]= {
  1200, 2400, 4800, 9600, 19200, 38400, BAUDRATE56K, 115200};


#define GET_NVR_OFFSET(param) ((int)&(((t_CONFIG*) 0)->param))

void writeEEPROM(int address, int value) {
#ifdef DEBUG_EEPROM
  SERIAL_PRINT("W ADDR: ");
  SERIAL_PRINT(address, HEX);
  SERIAL_PRINT(" VALUE: ");
  SERIAL_PRINT(value);
  SERIAL_PRINT(" 0x");
  SERIAL_PRINTLN(value, HEX);
#endif
  cli();
  EEPROM.write(address,  ((value >> 8*0) & 0xFF));
  EEPROM.write(address+1, ((value >> 8*1) & 0xFF));
  sei();
}

int readEEPROMInt(int address){
#ifdef DEBUG_EEPROM
  SERIAL_PRINT("R ADDR: ");
  SERIAL_PRINT(address, HEX);
#endif
  cli();
  int value =  EEPROM.read(address);
  value |= EEPROM.read(address+1)<<8;
  sei();
#ifdef DEBUG_EEPROM
  SERIAL_PRINT(" VALUE: ");
  SERIAL_PRINT(value);
  SERIAL_PRINT(" 0x");
  SERIAL_PRINTLN(value, HEX);
#endif
  return value;
}

void defaultConfiguration() {
  cnf.config_crc = DEF_CONFIGCRC;
  //PAN
  cnf.pan_minpwm = DEF_PANMINPWM;
  cnf.pan_minangle = DEF_PANMINANGLE;
  cnf.pan_maxpwm = DEF_PANMAXPWM;
  cnf.pan_maxangle = DEF_PANMAXANGLE;
  //TILT
  cnf.tilt_minpwm = DEF_TILTMINPWM;
  cnf.tilt_minangle = DEF_TILTMINANGLE;
  cnf.tilt_maxpwm = DEF_TILTMAXPWM;
  cnf.tilt_maxangle = DEF_TILTMAXANGLE;
  //TELEMETRY
  cnf.baudrate = DEF_BAUDRATE;
}

void saveConfiguration(){
  //SOFTWARE_VERSION
#ifdef DEBUG_EEPROM
  SERIAL_PRINTLN("=====EEPROM W=====");
#endif

  writeEEPROM(GET_NVR_OFFSET(config_crc),cnf.config_crc);
  writeEEPROM(GET_NVR_OFFSET(pan_minpwm),cnf.pan_minpwm);
  writeEEPROM(GET_NVR_OFFSET(pan_minangle),cnf.pan_minangle);
  writeEEPROM(GET_NVR_OFFSET(pan_maxpwm),cnf.pan_maxpwm);
  writeEEPROM(GET_NVR_OFFSET(pan_maxangle),cnf.pan_maxangle);

  writeEEPROM(GET_NVR_OFFSET(tilt_minpwm),cnf.tilt_minpwm);
  writeEEPROM(GET_NVR_OFFSET(tilt_minangle),cnf.tilt_minangle);
  writeEEPROM(GET_NVR_OFFSET(tilt_maxpwm),cnf.tilt_maxpwm);
  writeEEPROM(GET_NVR_OFFSET(tilt_maxangle),cnf.tilt_maxangle);
}

void resetConfiguration() {
  defaultConfiguration();
  saveConfiguration();
}

void readConfiguration(){

  cnf.config_crc = readEEPROMInt(GET_NVR_OFFSET(config_crc));

  if(cnf.config_crc!=DEF_CONFIGCRC){
    defaultConfiguration();
    saveConfiguration();
  }
  else{
    cnf.pan_minpwm = readEEPROMInt(GET_NVR_OFFSET(pan_minpwm));
    cnf.pan_minangle = readEEPROMInt(GET_NVR_OFFSET(pan_minangle));
    cnf.pan_maxpwm = readEEPROMInt(GET_NVR_OFFSET(pan_maxpwm));
    cnf.pan_maxangle = readEEPROMInt(GET_NVR_OFFSET(pan_maxangle));

    cnf.tilt_minpwm = readEEPROMInt(GET_NVR_OFFSET(tilt_minpwm));
    cnf.tilt_minangle = readEEPROMInt(GET_NVR_OFFSET(tilt_minangle));
    cnf.tilt_maxpwm = readEEPROMInt(GET_NVR_OFFSET(tilt_maxpwm));
    cnf.tilt_maxangle = readEEPROMInt(GET_NVR_OFFSET(tilt_maxangle));
  }


#ifdef DEBUG_CONFIG
  SERIAL_PRINTLN("=====READ CONFIG=====");
  SERIAL_PRINT("config_crc: ");
  SERIAL_PRINTLN(cnf.config_crc);
  SERIAL_PRINT("pan_minpwm: ");
  SERIAL_PRINTLN(cnf.pan_minpwm);
  SERIAL_PRINT("pan_maxpwm: ");
  SERIAL_PRINTLN(cnf.pan_maxpwm);
  SERIAL_PRINT("pan_minangle: ");
  SERIAL_PRINTLN(cnf.pan_minangle);
  SERIAL_PRINT("pan_maxangle: ");
  SERIAL_PRINTLN(cnf.pan_maxangle);
  SERIAL_PRINT("tilt_minpwm: ");
  SERIAL_PRINTLN(cnf.tilt_minpwm);
  SERIAL_PRINT("tilt_maxpwm: ");
  SERIAL_PRINTLN(cnf.tilt_maxpwm);
  SERIAL_PRINT("tilt_minangle: ");
  SERIAL_PRINTLN(cnf.tilt_minangle);
  SERIAL_PRINT("tilt_maxangle: ");
  SERIAL_PRINTLN(cnf.tilt_maxangle);
#endif
}

#ifdef DEBUG
void debug() {
  Serial.print("mem ");
  int freememory = freeMem();
  Serial.println(freememory);
  Serial.print("activ:");
  //Serial.println(current_activity);
  Serial.print("conftelem:");
  //Serial.println(configuration.telemetry);
  Serial.print("baud");
  //Serial.println(configuration.baudrate);
  Serial.print("lat=");
  Serial.println(uavPosition.latitude,7);///10000000.0
  Serial.print("lon=");
  Serial.println(uavPosition.longitud,7);
  Serial.print("alt=");
  Serial.println(uavPosition.altitude);
  Serial.print("rel_alt=");
  //Serial.println(rel_alt);
  //Serial.print(uav_groundspeed);
  //Serial.println(uav_groundspeed);
  Serial.print("dst=");
  //Serial.println(home_dist);
  Serial.print("El:");
  //Serial.println(Elevation);
  Serial.print("Be:");
  //Serial.println(Bearing);
  Serial.print("H Be:");
  //Serial.println(home_bearing);
  Serial.print("uav_fix_type=");
  //Serial.println(uav_fix_type);
  Serial.print("uav_satellites_visible=");
  //Serial.println(uav_satellites_visible);
  Serial.print("pitch:");
  //Serial.println(uav_pitch);
  Serial.print("roll:");
  //Serial.println(uav_roll);
  Serial.print("yaw:");
  //Serial.println(uav_heading);
  Serial.print("rbat:");
  //Serial.println(uav_bat);
  Serial.print("amp:");
  //Serial.println(uav_amp);
  Serial.print("rssi:");
  //Serial.println(uav_rssi);
  Serial.print("aspeed:");
  //Serial.println(uav_airspeed);
  Serial.print("armed:");
  //Serial.println(uav_arm);
  Serial.print("fs:");
  //Serial.println(uav_failsafe);
  Serial.print("fmode:");
  //Serial.println(uav_flightmode);
  Serial.print("armfsmode");
  //Serial.println(ltm_armfsmode);
}
#endif

#endif











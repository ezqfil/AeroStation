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

//#define toRad(x) (x*PI)/180.0
#define toDeg(x) (x*180.0)/PI
//#define ledON(x) digitalWrite(x, HIGH);
//#define ledOFF(x) digitalWrite(x, LOW);


#define BAUD 57600
#define BAUD_TELEMETRY 1200
//#define BAUD_TELEMETRY 57600

//#define BUZZER_Pin 14 //Pin mode as output to control buzzer (analog0)

#define LED_Green 13
#define LED_Red 12
#define LED_Yellow 11

#define PAM_Pin 9
#define TILT_Pin 10

//mega
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
#define DEBUG_PORT Serial
#define TELEMETRY_PROXY_PORT Serial1
#define TELEMETRY_PORT Serial2
//leonardo
#else
#if defined (__AVR_ATmega32U4__)
#if defined (TELEMETRY_PROXY) && defined (DEBUG)
#error Telemetry proxy and Debug are inconpatibility
#endif
#define DEBUG_PORT Serial
#define TELEMETRY_PROXY_PORT Serial
#define TELEMETRY_PORT Serial1
#else
#error Use arduino mega
#endif
#endif
//Yun, leonardo, micro, espora,Lilypad USB, Robot Control,Robot Motor
//#if defined (__AVR_ATmega32U4__)
//#define DEBUG_PORT Serial
//#define TELEMETRY_PROXY_PORT Serial
//#define TELEMETRY_PORT Serial1
//#endif

// uno, diecimilia,nano, mini,ethernet, fio,bt, LilyPad, pro, ng
//#if defined (__AVR_ATmega328P__)  || defined (__AVR_ATmega168__)  || defined (__AVR_ATmega8__) // force 328p to use the normal port
//#define DEBUG_PORT Serial
//#define TELEMETRY_PROXY_PORT Serial
//#define TELEMETRY_PORT Serial1
//#endif

#if defined (DEBUG)
#define DEBUG_BEGIN      DEBUG_PORT.begin
#define DEBUG_AVAILABLE  DEBUG_PORT.available
#define DEBUG_PRINT      DEBUG_PORT.print
#define DEBUG_PRINTH     DEBUG_PORT.print
#define DEBUG_PRINTLN    DEBUG_PORT.println
#define DEBUG_PRINTHLN   DEBUG_PORT.println
#define DEBUG_READ       DEBUG_PORT.read
#else
#define DEBUG_BEGIN(x)    0
#define DEBUG_AVAILABLE() false
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTH(x,y) 
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTHLN(x,y)
#define DEBUG_READ()      0
//#define DEBUG_FLUSH      DEBUG_PORT.flush
#endif

#define TELEMETRY_BEGIN      TELEMETRY_PORT.begin
#define TELEMETRY_AVAILABLE  TELEMETRY_PORT.available
#define TELEMETRY_READ       TELEMETRY_PORT.read
#define TELEMETRY_WRITE      TELEMETRY_PORT.write
#define TELEMETRY_FLUSH      TELEMETRY_PORT.flush

#define TELEMETRY_PROXY_BEGIN      TELEMETRY_PROXY_PORT.begin
#define TELEMETRY_PROXY_READ       TELEMETRY_PROXY_PORT.read
#define TELEMETRY_PROXY_WRITE      TELEMETRY_PROXY_PORT.write
//#define TELEMETRY_PROXY_FLUSH      TELEMETRY_PROXY_PORT.flush

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
  int  course;
  byte speed;
  byte airspeed;
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
  long latitude;
  long longitud;
  int  altitude;
  //    int telemetry;
  //    int bearing;
  //    uint8_t osd_enabled;
  //    uint8_t bearing_method;
  //    uint16_t voltage_ratio;
}
t_CONFIG;

t_CONFIG cnf;

t_HOME_DATA home = {
  0, 0, 0, 0, 0, 0
};
t_UAV_DATA uav =   {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//boolean telemetry_ok = false;
boolean telemetry_proxy = false;

#ifndef BAUDRATE56K
#define BAUDRATE56K 57600
#endif

const long baudrates[8] = {
  1200, 2400, 4800, 9600, 19200, 38400, BAUDRATE56K, 115200
};

#define GET_NVR_OFFSET(param) ((int)&(((t_CONFIG*) 0)->param))
#define NVR_WRITE(param,value) writeEEPROM(GET_NVR_OFFSET(param),value)
#define NVR_READ(param,value) readEEPROM(GET_NVR_OFFSET(param),value)

void writeEEPROM(unsigned int address, byte value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Wb ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif
  cli();
  EEPROM.write(address,  ((value >> 8 * 0) & 0xFF));
  sei();
}

void writeEEPROM(unsigned int address, int value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Wi ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif
  cli();
  //EEPROM.write(address,  ((value >> 8*0) & 0xFF));
  //EEPROM.write(address+1, ((value >> 8*1) & 0xFF));
  const byte* p = (const byte*)(const void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    EEPROM.write(address++, *p++);
  }
  sei();
}

void writeEEPROM( unsigned int address, long value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Wl ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif//20158
  cli();
  const byte* p = (const byte*)(const void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    EEPROM.write(address++, *p++);
  }
  sei();
}

void readEEPROM(unsigned int address, byte &value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Rb ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
#endif
  cli();
  value =  EEPROM.read(address);
  sei();
#ifdef DEBUG_EEPROM
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif
}

void readEEPROM(unsigned int address, int &value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Ri ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
#endif
  cli();
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(address++);
  sei();
#ifdef DEBUG_EEPROM
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif
}

void readEEPROM(unsigned int address, long &value) {
#ifdef DEBUG_EEPROM
  DEBUG_PRINT("Rl ADDR: ");
  DEBUG_PRINTH(address, HEX);
  DEBUG_PRINT(" (");
  DEBUG_PRINT(sizeof(value));
  DEBUG_PRINT(") ");
#endif
  cli();//20172
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(address++);
  }
  sei();
#ifdef DEBUG_EEPROM
  DEBUG_PRINT(" VALUE: ");
  DEBUG_PRINT(value);
  DEBUG_PRINT(" 0x");
  DEBUG_PRINTHLN(value, HEX);
#endif
}

void defaultConfiguration() {
#ifdef DEBUG_CONFIG
  DEBUG_PRINTLN("Set default configuration");
#endif
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

void saveConfiguration() {
  //SOFTWARE_VERSION
#ifdef DEBUG_EEPROM
  DEBUG_PRINTLN("=====EEPROM W=====");
#endif

  NVR_WRITE(config_crc, cnf.config_crc);


  NVR_WRITE(pan_minpwm, cnf.pan_minpwm);
  NVR_WRITE(pan_minangle, cnf.pan_minangle);
  NVR_WRITE(pan_maxpwm, cnf.pan_maxpwm);
  NVR_WRITE(pan_maxangle, cnf.pan_maxangle);

  NVR_WRITE(tilt_minpwm, cnf.tilt_minpwm);
  NVR_WRITE(tilt_minangle, cnf.tilt_minangle);
  NVR_WRITE(tilt_maxpwm, cnf.tilt_maxpwm);
  NVR_WRITE(tilt_maxangle, cnf.tilt_maxangle);

  NVR_WRITE(baudrate, cnf.baudrate);

  NVR_WRITE(latitude, home.latitude);
  NVR_WRITE(longitud, home.longitud);
  NVR_WRITE(altitude, home.altitude);
}

void resetConfiguration() {
  defaultConfiguration();
  saveConfiguration();
}

void readConfiguration() {

  //cnf.config_crc = readEEPROMInt(GET_NVR_OFFSET(config_crc));
  NVR_READ(config_crc, cnf.config_crc);
  if (cnf.config_crc != DEF_CONFIGCRC) {
#ifdef DEBUG_CONFIG
    DEBUG_PRINTLN("Config crc not mached");
#endif
    resetConfiguration();
  }
  else {

    NVR_READ(pan_minpwm, cnf.pan_minpwm);
    NVR_READ(pan_minangle, cnf.pan_minangle);
    NVR_READ(pan_maxpwm, cnf.pan_maxpwm);
    NVR_READ(pan_maxangle, cnf.pan_maxangle);

    NVR_READ(tilt_minpwm, cnf.tilt_minpwm);
    NVR_READ(tilt_minangle, cnf.tilt_minangle);
    NVR_READ(tilt_maxpwm, cnf.tilt_maxpwm);
    NVR_READ(tilt_maxangle, cnf.tilt_maxangle);

    NVR_READ(baudrate, cnf.baudrate);

    NVR_READ(latitude, home.latitude);
    NVR_READ(longitud, home.longitud);
    NVR_READ(altitude, home.altitude);
  }


#ifdef DEBUG_CONFIG
  DEBUG_PRINTLN("=====READ CONFIG=====");
  DEBUG_PRINT("config_crc: ");
  DEBUG_PRINTLN(cnf.config_crc);
  DEBUG_PRINT("pan_minpwm: ");
  DEBUG_PRINTLN(cnf.pan_minpwm);
  DEBUG_PRINT("pan_maxpwm: ");
  DEBUG_PRINTLN(cnf.pan_maxpwm);
  DEBUG_PRINT("pan_minangle: ");
  DEBUG_PRINTLN(cnf.pan_minangle);
  DEBUG_PRINT("pan_maxangle: ");
  DEBUG_PRINTLN(cnf.pan_maxangle);
  DEBUG_PRINT("tilt_minpwm: ");
  DEBUG_PRINTLN(cnf.tilt_minpwm);
  DEBUG_PRINT("tilt_maxpwm: ");
  DEBUG_PRINTLN(cnf.tilt_maxpwm);
  DEBUG_PRINT("tilt_minangle: ");
  DEBUG_PRINTLN(cnf.tilt_minangle);
  DEBUG_PRINT("tilt_maxangle: ");
  DEBUG_PRINTLN(cnf.tilt_maxangle);
#endif
}

#ifdef DEBUG_LOG
void debug_log() {
  DEBUG_PRINT("mem ");
  int freememory = freeMem();
  DEBUG_PRINTLN(freememory);
  //Serial.print("activ:");
  //Serial.println(current_activity);
  //Serial.print("conftelem:");
  //Serial.println(configuration.telemetry);
  //Serial.print("baud");
  //Serial.println(configuration.baudrate);
  //Serial.print("lat=");
  //Serial.println(uav.latitude,7);///10000000.0
  //Serial.print("lon=");
  //Serial.println(uav.longitud,7);
  //Serial.print("alt=");
  //Serial.println(uav.altitude);
  //Serial.print("rel_alt=");
  //Serial.println(rel_alt);
  //Serial.print(uav_groundspeed);
  //Serial.println(uav_groundspeed);
  //Serial.print("dst=");
  //Serial.println(home_dist);
  //Serial.print("El:");
  //Serial.println(Elevation);
  //Serial.print("Be:");
  //Serial.println(Bearing);
  //Serial.print("H Be:");
  //Serial.println(home_bearing);
  //Serial.print("uav_fix_type=");
  //Serial.println(uav_fix_type);
  //Serial.print("uav_satellites_visible=");
  //Serial.println(uav_satellites_visible);
  //Serial.print("pitch:");
  //Serial.println(uav_pitch);
  //Serial.print("roll:");
  //Serial.println(uav_roll);
  //Serial.print("yaw:");
  //Serial.println(uav_heading);
  //Serial.print("rbat:");
  //Serial.println(uav_bat);
  //Serial.print("amp:");
  //Serial.println(uav_amp);
  //Serial.print("rssi:");
  //Serial.println(uav_rssi);
  //Serial.print("aspeed:");
  //Serial.println(uav_airspeed);
  //Serial.print("armed:");
  //Serial.println(uav_arm);
  //Serial.print("fs:");
  //Serial.println(uav_failsafe);
  //Serial.print("fmode:");
  //Serial.println(uav_flightmode);
  //Serial.print("armfsmode");
  //Serial.println(ltm_armfsmode);
}
#endif

#endif






















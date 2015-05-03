/*
 *
 */

#define DEF_CONFIGCRC 5
#define DEF_PANMINPWM 609     //pam 
#define DEF_PANMINANGLE 0     //These numbers seem to work well 
#define DEF_PANMAXPWM 2255    //for the HITEC-485 servos 
#define DEF_PANMAXANGLE 360   //end pam
#define DEF_TILTMINPWM 591    //tilt 
#define DEF_TILTMINANGLE 0
#define DEF_TILTMAXPWM 2235
#define DEF_TILTMAXANGLE 180  //end tilt 
// 0 =  1200, 1 =  2400, 2 =  4800, 3 =   9600
// 4 = 19200, 5 = 38400, 6 = 57600, 7 = 115200
#define DEF_BAUDRATE 0       
/* If you have communication problem at 56K , set this on. ( ie APM 2/2.5/2.6/AIO )
 Most Arduino have a +2.18% error at 57600 bd, Teensyduino has a -0.74% error. Booth cumulated are too much.
 Successfull com between Teensy & arduino requires 58824 bauds for Teensy.*/
//#define BAUDRATE56K 58824


/*
 *
 */

#define TELEMETRY_TEST
//#define TELEMETRY_PROXY
#define DEBUG

/*
 *
 */

//#define DEBUG_LOG
//#define DEBUG_TRAKER
//#define DEBUG_SERVOTEST
//#define DEBUG_EEPROM
//#define DEBUG_CONFIG
//#define DEBUG_TELEMETRY 
//#define DEBUG_SLOWTELEMETRY 
//#define DEBUG_PROXYTELEMETRY 


#ifndef _AS_SERIAL_COMM_
#define _AS_SERIAL_COMM_
/*

 AeroQuad v3.0 - December 2011
 www.AeroQuad.com
 Copyright (c) 2011 Ted Carancho.  All rights reserved.
 An Open Source Arduino based multicopter.
 
 
 AQ PROTOCOL
 0x51	0x41	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
 A	Q	--------- Latitude ---------    -------- Longitude ---------    - Altitude -    -- Curse  --    - Heading  -    Speed   RSSI    Volt    Curr    - Capacity -    - Gps info -
 
 
 // SerialCom.pde is responsible for the serial communication for commands and telemetry from the AeroQuad
 // This comtains readSerialCommand() which listens for a serial command and it's arguments
 // This also contains readSerialTelemetry() which listens for a telemetry request and responds with the requested data
 // For more information on each command/telemetry look at: http://aeroquad.com/content.php?117
 
 // Includes re-write / fixes from Aadamson and ala42, special thanks to those guys!
 // http://aeroquad.com/showthread.php?1461-We-have-some-hidden-warnings&p=14618&viewfull=1#post14618
 
 
 */

/* #################################################################################################################
 * LightTelemetry protocol (LTM)
 *
 * Ghettostation one way telemetry protocol for really low bitrates (1200/2400 bauds). 
 *			   
 * Protocol details: 3 different frames, little endian.
 *   G Frame (GPS position) (2hz @ 1200 bauds , 5hz >= 2400 bauds): 18BYTES
 *     0x24 0x54 0x47 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF  0xFF   0xC0   
 *      $     T    G  --------LAT-------- -------LON---------  SPD --------ALT-------- SAT/FIX  CRC
 *   A Frame (Attitude) (5hz @ 1200bauds , 10hz >= 2400bauds): 10BYTES
 *     0x24 0x54 0x41 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xC0   
 *      $     T    A  --PITCH-- --ROLL--- -HEADING-  CRC
 *   S Frame (Sensors) (2hz @ 1200bauds, 5hz >= 2400bauds): 11BYTES
 *     0x24 0x54 0x53 0xFF 0xFF  0xFF 0xFF    0xFF    0xFF      0xFF       0xC0     
 *      $     T    S  VBAT(mv)  Current(ma)   RSSI  AIRSPEED  ARM/FS/FMOD   CRC
 *   
 *     0x24 0x54 0x   0XFF 0XFF
 *      $     T   Q   CURSE
 * ################################################################################################################# */

//AeroQuad SlowTelemetry
#define TELEMETRY_AQ_MSGSIZE 24
#define TELEMETRY_AQ_MSGSIZE_ECC (TELEMETRY_AQ_MSGSIZE + 8)

//Ghettostation LightTelemetry protocol (LTM)
#define LIGHTTELEMETRY_GFRAMELENGTH 18
#define LIGHTTELEMETRY_AFRAMELENGTH 10
#define LIGHTTELEMETRY_SFRAMELENGTH 11
//#define LIGHTTELEMETRY_OFRAMELENGTH 18

//byte s;
byte telemetryMsgSize;
byte readTelemetryByte;

byte telemetryBufferReader[35];

void initializeTelemetry() {
  TELEMETRY_BEGIN(BAUD_TELEMETRY);
}

uint8_t readByte()  {
  return telemetryBufferReader[readTelemetryByte++];
}

uint16_t readShort() {
  uint16_t t = readByte();
  t |= (uint16_t)readByte()<<8;
  return t;
}

uint32_t readLong() {
  uint32_t t = readShort();
  t |= (uint32_t)readShort()<<16;
  return t;
}

void decode(){
  readTelemetryByte=0;
  //DEBUG_PRINTLN("decode");
  if(telemetryBufferReader[0]=='A'){
    DEBUG_PRINTLN("AQ");
    DEBUG_PRINT("id1:");
    DEBUG_PRINTLN((char)readByte());
    DEBUG_PRINT("id2:");
    DEBUG_PRINTLN((char)readByte());
    uav.latitude=readLong();
    uav.longitud=readLong();
    uav.altitude=readShort()/10;
    //DEBUG_PRINTLN(readShort());      
    uav.course=readShort();      
    uav.heading=readShort();
    uav.speed=readByte(); 
    uav.airspeed=uav.speed;   
    uav.rssi=readByte();      
    uav.bat_voltage=readByte();      
    uav.bat_current=readByte();      
    uav.bat_capacity=readShort();      
    DEBUG_PRINT("gpsinfo:");
    DEBUG_PRINTLN((unsigned short)readShort()>>12);     

#ifdef DEBUG_TELEMETRY
    DEBUG_PRINTLN("=====Telemetry=====");
    DEBUG_PRINT("latitude:"); // degrees/10000000
    DEBUG_PRINTLN(uav.latitude);
    DEBUG_PRINT("longitude:"); // degrees/10000000
    DEBUG_PRINTLN(uav.longitud);     
    DEBUG_PRINT("altitude: "); // x/10.0
    DEBUG_PRINTLN(uav.altitude); // x/10.0
    DEBUG_PRINT("course:");
    DEBUG_PRINTLN(uav.course); 
    DEBUG_PRINT("heading:");
    DEBUG_PRINTLN(uav.heading); 
    DEBUG_PRINT("speed:"); // x/36*1000
    DEBUG_PRINTLN(uav.speed); 
    DEBUG_PRINT("rssi:"); // x/36*1000
    DEBUG_PRINTLN(uav.rssi); 
    DEBUG_PRINT("bat voltage:"); // to 0.1V x*10
    DEBUG_PRINTLN(uav.bat_voltage); 
    DEBUG_PRINT("bat current:"); // to A x*100
    DEBUG_PRINTLN(uav.bat_current); 
    DEBUG_PRINT("capacity:"); // mAh x*1000
    DEBUG_PRINTLN(uav.bat_capacity); 
#endif

  } 
  else {
    char p;
    DEBUG_PRINTLN("TLM");
    DEBUG_PRINT("id1:");
    DEBUG_PRINTLN((char)readByte());
    DEBUG_PRINT("id2:");
    DEBUG_PRINTLN((char)readByte());
    DEBUG_PRINT("id3:");
    p= (char)readByte();
    DEBUG_PRINTLN(p);
    if(p=='G') {
      uav.latitude=readLong();
      uav.longitud=readLong();
      uav.speed=readByte();  
      uav.altitude=readLong();
      p=readByte();// gps info
#ifdef DEBUG_TELEMETRY
      DEBUG_PRINTLN("=====Telemetry=====");
      DEBUG_PRINT("latitud:");
      DEBUG_PRINTLN(uav.latitude);
      DEBUG_PRINT("longitud:");
      DEBUG_PRINTLN(uav.longitud);
      DEBUG_PRINT("speed:");
      DEBUG_PRINTLN(uav.speed);
      DEBUG_PRINT("altitud: ");
      DEBUG_PRINTLN(uav.altitude); 
      DEBUG_PRINTLN("gpsinfo:");
      DEBUG_PRINTLN(p>>2); 
      DEBUG_PRINTLN((p)&11);
#endif
    } 
    else if(p=='S') {
      uav.bat_voltage=readShort();
      uav.bat_current=readShort();
      uav.rssi=readByte();  
      uav.airspeed=readByte(); 
      p=readByte(); 
#ifdef DEBUG_TELEMETRY
      DEBUG_PRINT("voltage:");
      DEBUG_PRINTLN(uav.bat_voltage); 
      DEBUG_PRINT("amperaje:");
      DEBUG_PRINTLN(uav.bat_current); 
      DEBUG_PRINT("rssi:");
      DEBUG_PRINTLN(uav.rssi);
      DEBUG_PRINT("airspeed:");
      DEBUG_PRINTLN(uav.airspeed); 
      DEBUG_PRINTLN("info:");
      DEBUG_PRINTLN(p>>2);
      DEBUG_PRINTLN((p>>1)&1);
      DEBUG_PRINTLN((p)&1);
#endif
    } 
    else if(p=='A') {
      uav.pitch=readShort();
      uav.roll=readShort();
      uav.heading=readShort();
#ifdef DEBUG_TELEMETRY
      DEBUG_PRINTLN("=====Telemetry=====");
      DEBUG_PRINT("pitch:");
      DEBUG_PRINTLN(uav.pitch);
      DEBUG_PRINT("roll:");
      DEBUG_PRINTLN(uav.roll);
      DEBUG_PRINT("heading:");
      DEBUG_PRINTLN(uav.heading);
#endif
    }
  }
  readTelemetryByte=0;
  
  telemetry_ok=true;
  telemetry_proxy=true;
}

void updateSlowTelemetry () {


  static enum _serial_state {
    IDLE,
    HEADER_START1,
    HEADER_START2,
    HEADER_MSGTYPE,
    HEADER_DATA
  }
  c_state = IDLE;

  byte c;

#ifdef DEBUG_SLOWTELEMETRY
  boolean ln = false;
  if(TELEMETRY_AVAILABLE()){
    DEBUG_PRINTLN("=====SlowTelemetry MSG=====");
    DEBUG_PRINT("RX: ");
    ln=true;
  }
#endif
  //c_state = IDLE;
  while (TELEMETRY_AVAILABLE()) {
    c = TELEMETRY_READ();
#ifdef DEBUG_SLOWTELEMETRY
    DEBUG_PRINT(c,HEX);
    DEBUG_PRINT("|");
#endif

    if (c_state == IDLE) {
      if  (c=='$' || c=='A') {
        //DEBUG_PRINTLN("head 1|A");
        readTelemetryByte=0;
        c_state = HEADER_START1;
        telemetryBufferReader[readTelemetryByte++]=c;
      } 
      //else {//no tiene sentido esto sobra
      //  DEBUG_PRINTLN("Que paso head1");
      //}
    } 
    else if (c_state == HEADER_START1) {
      telemetryBufferReader[readTelemetryByte++]=c;
      if  (c=='T') {
        c_state = HEADER_START2;
        //DEBUG_PRINTLN("head t");
      } 
      else if  (c=='Q') {
        //DEBUG_PRINTLN("head q");
        c_state = HEADER_MSGTYPE;
        telemetryMsgSize = TELEMETRY_AQ_MSGSIZE_ECC;
      } 
      else {
        //DEBUG_PRINTLN("Procesar alerta sonora por contador");
        c_state = IDLE;
      }
    } 
    else if (c_state == HEADER_START2) {
      //DEBUG_PRINTLN("head g");
      telemetryBufferReader[readTelemetryByte++]=c;
      switch (c) {
      case 'G':
        telemetryMsgSize = LIGHTTELEMETRY_GFRAMELENGTH;
        c_state = HEADER_MSGTYPE;
        break;
      case 'A':
        telemetryMsgSize = LIGHTTELEMETRY_AFRAMELENGTH;
        c_state = HEADER_MSGTYPE;
        break;
      case 'S':
        telemetryMsgSize = LIGHTTELEMETRY_SFRAMELENGTH;
        c_state = HEADER_MSGTYPE;
        break;
      default:
        //DEBUG_PRINTLN("Mensaje a desartar");
        c_state = IDLE;
      } 
    } 
    else if (c_state == HEADER_MSGTYPE) {
      if (readTelemetryByte<telemetryMsgSize){
        telemetryBufferReader[readTelemetryByte++]=c;        
        if (readTelemetryByte==telemetryMsgSize){
          //DEBUG_PRINTLN("End Message");
          decode();
          readTelemetryByte=0;
          c_state = IDLE;
        } 
      } 
      //else {// no tiene sentido esta linea, sobra
      //  DEBUG_PRINTLN("Que paso");
      //  c_state = IDLE;
      //}
    } 
    else { 
      //DEBUG_PRINT("byte perdido ");
      //DEBUG_PRINTLN(c);    
      // wrong checksum, drop packet
      c_state = IDLE; 
    }

  }

#ifdef DEBUG_SLOWTELEMETRY
  if (ln) {
    DEBUG_PRINTLN("");
  }
#endif

}

#endif






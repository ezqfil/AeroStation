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
 *    0x24 0x54 0x47 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF  0xFF   0xC0   
 *     $     T    G  --------LAT-------- -------LON---------  SPD --------ALT-------- SAT/FIX  CRC
 *   A Frame (Attitude) (5hz @ 1200bauds , 10hz >= 2400bauds): 10BYTES
 *     0x24 0x54 0x41 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xC0   
 *      $     T   A   --PITCH-- --ROLL--- -HEADING-  CRC
 *   S Frame (Sensors) (2hz @ 1200bauds, 5hz >= 2400bauds): 11BYTES
 *     0x24 0x54 0x53 0xFF 0xFF  0xFF 0xFF    0xFF    0xFF      0xFF       0xC0     
 *      $     T   S   VBAT(mv)  Current(ma)   RSSI  AIRSPEED  ARM/FS/FMOD   CRC
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
  telemetry_ok=true;
  readTelemetryByte=0;
  //SERIAL_PORT.println("decode");
  if(telemetryBufferReader[0]=='A'){
    SERIAL_PORT.println("AQ");
    SERIAL_PORT.print("id1:");
    SERIAL_PORT.println((char)readByte());
    SERIAL_PORT.print("id2:");
    SERIAL_PORT.println((char)readByte());
    uav.latitude=readLong();
    uav.longitud=readLong();
    uav.altitude=readShort()/10;
    //SERIAL_PORT.println(readShort());      
    SERIAL_PORT.print("course:");
    SERIAL_PORT.println(readShort());      
    SERIAL_PORT.print("heading:");
    SERIAL_PORT.println(readShort());      
    uav.speed=readByte();      
    uav.rssi=readByte();      
    uav.bat_voltage=readByte();      
    uav.bat_current=readByte();      
    uav.bat_capacity=readShort();      
    SERIAL_PORT.print("gpsinfo:");
    SERIAL_PORT.println((unsigned short)readShort()>>12);     

#ifdef DEBUG_TELEMETRY
    SERIAL_PORT.println("=====Telemetry=====");
    SERIAL_PORT.print("latitude:"); // degrees/10000000
    SERIAL_PORT.println(uav.latitude);
    SERIAL_PORT.print("longitude:"); // degrees/10000000
    SERIAL_PORT.println(uav.longitud);     
    SERIAL_PORT.print("altitude: "); // x/10.0
    SERIAL_PORT.println(uav.altitude); // x/10.0
    SERIAL_PORT.print("speed:"); // x/36*1000
    SERIAL_PORT.println(uav.speed); 
    SERIAL_PORT.print("rssi:"); // x/36*1000
    SERIAL_PORT.println(uav.rssi); 
    SERIAL_PORT.print("bat voltage:"); // to 0.1V x*10
    SERIAL_PORT.println(uav.bat_voltage); 
    SERIAL_PORT.print("bat current:"); // to A x*100
    SERIAL_PORT.println(uav.bat_current); 
    SERIAL_PORT.print("capacity:"); // mAh x*1000
    SERIAL_PORT.println(uav.bat_capacity); 
#endif

  } 
  else {
    char p;
    SERIAL_PORT.println("TLM");
    SERIAL_PORT.print("id1:");
    SERIAL_PORT.println((char)readByte());
    SERIAL_PORT.print("id2:");
    SERIAL_PORT.println((char)readByte());
    SERIAL_PORT.print("id3:");
    p= (char)readByte();
    SERIAL_PORT.println(p);
    if(p=='G') {
      uav.latitude=readLong();
      uav.longitud=readLong();
      uav.speed=readByte();  
      uav.altitude=readLong();
      p=readByte();// gps info
#ifdef DEBUG_TELEMETRY
      SERIAL_PORT.println("=====Telemetry=====");
      SERIAL_PORT.print("latitud:");
      SERIAL_PORT.println(uav.latitude);
      SERIAL_PORT.print("longitud:");
      SERIAL_PORT.println(uav.longitud);
      SERIAL_PORT.print("speed:");
      SERIAL_PORT.println(uav.speed);
      SERIAL_PORT.print("altitud: ");
      SERIAL_PORT.println(uav.altitude); 
      SERIAL_PORT.println("gpsinfo:");
      SERIAL_PORT.println(p>>2); 
      SERIAL_PORT.println((p)&11);
#endif
    } 
    else if(p=='S') {
      uav.bat_voltage=readShort();
      uav.bat_current=readShort();
      uav.rssi=readByte();  
      SERIAL_PORT.print("airspeed:");
      SERIAL_PORT.println(readByte()); 
      p=readByte(); 
#ifdef DEBUG_TELEMETRY
      SERIAL_PORT.print("voltage:");
      SERIAL_PORT.println(uav.bat_voltage); 
      SERIAL_PORT.print("amperaje:");
      SERIAL_PORT.println(uav.bat_current); 
      SERIAL_PORT.print("rssi:");
      SERIAL_PORT.println(uav.rssi); 
      SERIAL_PORT.println("info:");
      SERIAL_PORT.println(p>>2);
      SERIAL_PORT.println((p>>1)&1);
      SERIAL_PORT.println((p)&1);
#endif
    } 
    else if(p=='A') {
      uav.pitch=readShort();
      uav.roll=readShort();
      uav.heading=readShort();
#ifdef DEBUG_TELEMETRY
      SERIAL_PORT.println("=====Telemetry=====");
      SERIAL_PORT.print("pitch:");
      SERIAL_PORT.println(uav.pitch);
      SERIAL_PORT.print("roll:");
      SERIAL_PORT.println(uav.roll);
      SERIAL_PORT.print("heading:");
      SERIAL_PORT.println(uav.heading);
#endif
    }
  }
  readTelemetryByte=0;
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

#ifdef DEBUG_TELEMETRY
  boolean ln = false;
  if(TELEMETRY_AVAILABLE()){
    SERIAL_PORT.println("=====SlowTelemetry MSG=====");
    SERIAL_PORT.print("RX: ");
    ln=true;
  }
#endif
  //c_state = IDLE;
  while (TELEMETRY_AVAILABLE()) {
    c = TELEMETRY_READ();
#ifdef DEBUG_SLOWTELEMETRY
    SERIAL_PRINT(c,HEX);
    SERIAL_PRINT("|");
#endif

    if (c_state == IDLE) {
      if  (c=='$' || c=='A') {
        //SERIAL_PORT.println("head 1|A");
        readTelemetryByte=0;
        c_state = HEADER_START1;
        telemetryBufferReader[readTelemetryByte++]=c;
      } 
      //else {//no tiene sentido esto sobra
      //  SERIAL_PORT.println("Que paso head1");
      //}
    } 
    else if (c_state == HEADER_START1) {
      telemetryBufferReader[readTelemetryByte++]=c;
      if  (c=='T') {
        c_state = HEADER_START2;
        //SERIAL_PORT.println("head t");
      } 
      else if  (c=='Q') {
        //SERIAL_PORT.println("head q");
        c_state = HEADER_MSGTYPE;
        telemetryMsgSize = TELEMETRY_AQ_MSGSIZE_ECC;
      } 
      else {
        //SERIAL_PORT.println("Procesar alerta sonora por contador");
        c_state = IDLE;
      }
    } 
    else if (c_state == HEADER_START2) {
      //SERIAL_PORT.println("head g");
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
        //SERIAL_PORT.println("Mensaje a desartar");
        c_state = IDLE;
      } 
    } 
    else if (c_state == HEADER_MSGTYPE) {
      if (readTelemetryByte<telemetryMsgSize){
        telemetryBufferReader[readTelemetryByte++]=c;        
        if (readTelemetryByte==telemetryMsgSize){
          //SERIAL_PORT.println("End Message");
          decode();
          readTelemetryByte=0;
          c_state = IDLE;
        } 
      } 
      //else {// no tiene sentido esta linea, sobra
      //  SERIAL_PORT.println("Que paso");
      //  c_state = IDLE;
      //}
    } 
    else { 
      //SERIAL_PORT.print("byte perdido ");
      //SERIAL_PORT.println(c);    
      // wrong checksum, drop packet
      c_state = IDLE; 
    }

  }

#ifdef DEBUG_TELEMETRY
  if (ln) {
    SERIAL_PORT.println("");
  }
#endif

}

#endif






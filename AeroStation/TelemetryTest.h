#ifndef _AS_TELEMETRY_TEST_
#define _AS_TELEMETRY_TEST_
/*
 -346056154,-584163201  home
 -346050546,-584163201 N
 -346070546,-584163201 S
 -346056154,-584150001 E
 -346056154,-584180001 O
 -345810546,-584167201
 -346055978,-346055978
 -345810546,-584667201
 */

long   lat[8]={-346056154,-346050546,-346070546,-346056154,-346056154,-345810546,-346055978,-345810546};
long   lon[8]={-584163201,-584163201,-584163201,-584150001,-584180001,-584167201,-346055978,-584667201};
int    alt[8]={5,50,75,200,1170,90,456,876};

int    lastpos;

void send_messages(uint8_t *packet, uint8_t packet_size){
  byte slowTelemetryByte= 0;
#ifdef DEBUG_SLOWTELEMETRY
  DEBUG_PRINTLN("=====SlowTelemetry MSG=====");
  DEBUG_PRINT("TX: ");
#endif
  while (slowTelemetryByte < packet_size ) {
#ifdef DEBUG_SLOWTELEMETRY
    DEBUG_PRINT(packet[slowTelemetryByte],HEX);
    DEBUG_PRINT("|");
#endif
    TELEMETRY_WRITE(packet[slowTelemetryByte++]);
    
  }
  TELEMETRY_PROXY_FLUSH();
#ifdef DEBUG_SLOWTELEMETRY
  DEBUG_PRINTLN("");
#endif
}

//////////////////////



void initializeAQTelemetry(){
#define TELEMETRY_MSGSIZE 24
#define TELEMETRY_MSGSIZE_ECC (TELEMETRY_MSGSIZE + 8)

  struct __attribute__((packed)) telemetryPacket {
    unsigned short  id;
    long  latitude;
    long  longitude;
    short altitude;
    short course;
    short heading;
    byte  speed;
    byte  rssi;
    byte  voltage;
    byte  current;
    unsigned short capacity;
    unsigned short gpsinfo;
    byte  ecc[8];
  };

  union telemetryBuffer {
    struct telemetryPacket data;
    byte   bytes[32];
  } 
  telemetryBuffer;

  telemetryBuffer.data.id        = 0x5141; // "AQ"
  telemetryBuffer.data.latitude  = lat[lastpos];  //0;  // degrees/10000000
  telemetryBuffer.data.longitude = lon[lastpos];  //; // degrees/10000000
  telemetryBuffer.data.course    = 100/10; // degrees
  telemetryBuffer.data.speed     = 10000*36/1000;              // km/h
  telemetryBuffer.data.heading   = (short)(2*2); // degrees
  telemetryBuffer.data.gpsinfo   = 0;
  telemetryBuffer.data.gpsinfo  |= ((unsigned short)(12) << 12);
  telemetryBuffer.data.altitude  = (short)(alt[lastpos]  *10.0); // 0.1m
  telemetryBuffer.data.rssi      = 1000/10; // scale to 0-100
  telemetryBuffer.data.voltage   = 110/10;  // to 0.1V
  telemetryBuffer.data.current   = 200/100; // to A
  telemetryBuffer.data.capacity  = 3000000/1000; // mAh
  
  if(++lastpos>7)
    lastpos=0;
  
  encode_data(telemetryBuffer.bytes,TELEMETRY_MSGSIZE);
  send_messages(telemetryBuffer.bytes, TELEMETRY_MSGSIZE_ECC);
}


/////////////////////

void send_LTM_Packet(uint8_t *LTPacket, uint8_t LTPacket_size)
{
#ifdef DEBUG_SLOWTELEMETRY
  DEBUG_PRINTLN("LTM Protocol");
#endif
  //calculate Checksum
  uint8_t LTCrc = 0x00;
  int i;
  for (i = 3; i < LTPacket_size-1; i++) {
    LTCrc ^= LTPacket[i];
  }
  LTPacket[LTPacket_size-1]=LTCrc;
  send_messages(LTPacket, LTPacket_size);
}

void initializeLTMGTelemetry(){
#define LIGHTTELEMETRY_GFRAMELENGTH 18
  uint8_t LTBuff[LIGHTTELEMETRY_GFRAMELENGTH];

  int32_t      uav_lat = lat[lastpos];;                    // latitude
  int32_t      uav_lon = lon[lastpos];;                    // longitude
  uint8_t      uav_groundspeedms = 200;          // ground speed in m/s
  int32_t      uav_alt = alt[lastpos];                    // altitude (dm)
  uint8_t      uav_satellites_visible = 15;     // number of satellites
  uint8_t      uav_fix_type = 3;               // GPS lock 0-1=no fix, 2=2D, 3=3D

  //protocol: START(2 bytes)FRAMEID(1byte)LAT(cm,4 bytes)LON(cm,4bytes) 
  //SPEED(m/s,1bytes)ALT(cm,4bytes)SATS(6bits)FIX(2bits)CRC(xor,1byte)
  //START
  LTBuff[0]=0x24; //$
  LTBuff[1]=0x54; //T
  //FRAMEID
  LTBuff[2]=0x47; // G ( gps frame at 5hz )
  //PAYLOAD
  LTBuff[3]=(uav_lat >> 8*0) & 0xFF;
  LTBuff[4]=(uav_lat >> 8*1) & 0xFF;
  LTBuff[5]=(uav_lat >> 8*2) & 0xFF;
  LTBuff[6]=(uav_lat >> 8*3) & 0xFF;
  LTBuff[7]=(uav_lon >> 8*0) & 0xFF;
  LTBuff[8]=(uav_lon >> 8*1) & 0xFF;
  LTBuff[9]=(uav_lon >> 8*2) & 0xFF;
  LTBuff[10]=(uav_lon >> 8*3) & 0xFF;
  LTBuff[11]=(uav_groundspeedms >> 8*0) & 0xFF;
  LTBuff[12]=(uav_alt >> 8*0) & 0xFF;
  LTBuff[13]=(uav_alt >> 8*1) & 0xFF;
  LTBuff[14]=(uav_alt >> 8*2) & 0xFF;
  LTBuff[15]=(uav_alt >> 8*3) & 0xFF;
  LTBuff[16]= ((uav_satellites_visible << 2 )& 0xFF) | (uav_fix_type & 0b00000011) ; // last 6 bits: sats number, first 2:fix type (0,1,2,3)
  
  if(++lastpos>7)
    lastpos=0;
    
  send_LTM_Packet(LTBuff,LIGHTTELEMETRY_GFRAMELENGTH);
}

void initializeLTMSTelemetry(){
#define LIGHTTELEMETRY_SFRAMELENGTH 11
  uint8_t LTBuff[LIGHTTELEMETRY_SFRAMELENGTH];

  uint16_t     uav_bat = 0;                    // battery voltage (mv)
  uint16_t     uav_amp = 0;                    // consumed mah.
  //uint16_t     uav_current = 0;                // actual current   
  uint8_t      uav_rssi = 0;                   // radio RSSI (%)
  uint8_t      uav_airspeed = 0;               // Airspeed sensor (m/s)
  uint8_t      uav_arm = 0;                    // 0: disarmed, 1: armed
  uint8_t      uav_failsafe = 0;               // 0: normal,   1: failsafe 
  uint8_t      uav_flightmode = 19;            // Flight mode(0-19): 0: Manual, 1: Rate, 2: Attitude/Angle, 3: Horizon, 4: Acro, 5: Stabilized1, 6: Stabilized2, 7: Stab

  // 8: Altitude Hold, 9: Loiter/GPS Hold, 10: Auto/Waypoints, 11: Heading Hold / headFree, 12: Circle, 13: RTH, 14: FollowMe, 15: LAND

  // 16:FlybyWireA, 17: FlybywireB, 18: Cruise, 19: Unknown  

  //START
  LTBuff[0]=0x24; //$
  LTBuff[1]=0x54; //T
  //FRAMEID
  LTBuff[2]=0x53; //S 
  //PAYLOAD
  LTBuff[3]=(uav_bat >> 8*0) & 0xFF;                                                    //vbat converted in mv
  LTBuff[4]=(uav_bat >> 8*1) & 0xFF;
  LTBuff[5]=(uav_amp >> 8*0) & 0xFF;                                                    //consumed current in ma.
  LTBuff[6]=(uav_amp >> 8*1) & 0xFF;
  LTBuff[7]=(uav_rssi >> 8*0) & 0xFF;                                                   
  LTBuff[8]=(uav_airspeed >> 8*0) & 0xFF;                                               // no airspeed in multiwii/baseflight
  LTBuff[9]= ((uav_flightmode << 2)& 0xFF ) | ((uav_failsafe << 1)& 0b00000010 ) | (uav_arm & 0b00000001) ; 
  // last 6 bits: flight mode, 2nd bit: failsafe, 1st bit: Arm status.
  // Flight mode(0-19): 0: Manual, 1: Rate, 2: Attitude/Angle, 3: Horizon, 4: Acro, 5: Stabilized1, 6: Stabilized2, 7: Stabilized3,
  // 8: Altitude Hold, 9: Loiter/GPS Hold, 10: Auto/Waypoints, 11: Heading Hold / headFree, 
  // 12: Circle, 13: RTH, 14: FollowMe, 15: LAND, 16:FlybyWireA, 17: FlybywireB, 18: Cruise, 19: Unknown
  send_LTM_Packet(LTBuff,LIGHTTELEMETRY_SFRAMELENGTH);
}

void initializeLTMATelemetry(){
#define LIGHTTELEMETRY_AFRAMELENGTH 10

  uint8_t LTBuff[LIGHTTELEMETRY_AFRAMELENGTH];
  int16_t      uav_pitch = 0;                  // attitude pitch
  int16_t      uav_roll = 0;                   // attitude roll
  int16_t      uav_heading = 0;                // attitude heading



  //A Frame: $T(2 bytes)A(1byte)PITCH(2 bytes)ROLL(2bytes)HEADING(2bytes)CRC(xor,1byte)
  //START
  LTBuff[0]=0x24; //$
  LTBuff[1]=0x54; //T
  //FRAMEID
  LTBuff[2]=0x41; //A 
  //PAYLOAD
  LTBuff[3]=(uav_pitch >> 8*0) & 0xFF;
  LTBuff[4]=(uav_pitch >> 8*1) & 0xFF;
  LTBuff[5]=(uav_roll >> 8*0) & 0xFF;
  LTBuff[6]=(uav_roll >> 8*1) & 0xFF;
  LTBuff[7]=(uav_heading >> 8*0) & 0xFF;
  LTBuff[8]=(uav_heading >> 8*1) & 0xFF;
  send_LTM_Packet(LTBuff,LIGHTTELEMETRY_AFRAMELENGTH);
}


#endif







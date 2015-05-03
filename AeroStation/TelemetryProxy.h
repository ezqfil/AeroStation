#ifndef _AS_TELEMETRY_PROXY_
#define _AS_TELEMETRY_PROXY_

struct  telemetryPacket {
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

void initializeTelemetryProxy() {
  TELEMETRY_BEGIN(BAUD_TELEMETRY);
}

void send_proxy_messages(uint8_t *packet, uint8_t packet_size) {
  byte proxyTelemetryByte = 0;
#ifdef DEBUG_PROXYTELEMETRY
  DEBUG_PRINTLN("=====ProxyTelemetry MSG=====");
  DEBUG_PRINT("TX: ");
#endif
  while (proxyTelemetryByte < packet_size ) {
#ifdef DEBUG_PROXYTELEMETRY
    DEBUG_PRINT(packet[proxyTelemetryByte], HEX);
    DEBUG_PRINT("|");
#endif
    TELEMETRY_PROXY_WRITE(packet[proxyTelemetryByte++]);
  }
  TELEMETRY_PORT.flush();
#ifdef DEBUG_PROXYTELEMETRY
  DEBUG_PRINTLN("");
#endif
}

void updateProxyTelemetry() {
#define TELEMETRY_MSGSIZE 24
#define TELEMETRY_MSGSIZE_ECC (TELEMETRY_MSGSIZE + 8)

  if (telemetry_proxy) {
    telemetryBuffer.data.id        = 0x5141; // "AQ"
    telemetryBuffer.data.latitude  = uav.latitude; //lat[lastpos];  //0;  // degrees/10000000
    telemetryBuffer.data.longitude = uav.longitud;//lon[lastpos];  //; // degrees/10000000
    telemetryBuffer.data.altitude  = uav.altitude; //(short)(alt[lastpos]  * 10.0); // 0.1m
    telemetryBuffer.data.course    = uav.course;//100 / 10; // degrees
    telemetryBuffer.data.speed     = uav.speed; //10000 * 36 / 1000;          // km/h
    telemetryBuffer.data.heading   = (short)(2 * 2); // degrees
    telemetryBuffer.data.gpsinfo   = 0;
    telemetryBuffer.data.gpsinfo  |= ((unsigned short)(12) << 12);
    telemetryBuffer.data.rssi      = uav.rssi;// 1000 / 10; // scale to 0-100
    telemetryBuffer.data.voltage   = uav.bat_voltage;//110 / 10; // to 0.1V
    telemetryBuffer.data.current   = uav.bat_current;//200 / 100; // to A
    telemetryBuffer.data.capacity  = uav.bat_capacity;//3000000 / 1000; // mAh

    send_proxy_messages(telemetryBuffer.bytes, TELEMETRY_MSGSIZE_ECC);
    telemetry_proxy=false;
  }
}
#endif

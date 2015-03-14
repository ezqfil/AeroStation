#ifndef _AS_TRAKER_H_
#define _AS_TRAKER_H_

//float        lonScaleDown=0.0;
//uint32_t home_dist;
//float _uavDist=0.0;
Servo Pam;
Servo Tilt;


typedef struct {
  float bearing;
  float elevation;
  boolean hold;
  byte test_step;
  int test_cnt;
}
t_TRAKER_DATA;
t_TRAKER_DATA traker = {
  0, 0, 0, 0
};



void pathFinder(int bearing, int elevation) {
#ifdef DEGUG_TRAKER
  SERIAL_PRINTLN("=====FINDER=====");
  SERIAL_PRINT("Bearing: ");
  SERIAL_PRINTLN(bearing);
  SERIAL_PRINT("Elevation: ");
  SERIAL_PRINTLN(elevation);
#endif
  Pam.write(bearing);
  Tilt.write(elevation);
}


int servoTest () {
  switch (traker.test_step) {
    case 0:
      traker.test_step = 1;
      traker.test_cnt = 360;
      break;
    case 1:
      if (traker.test_cnt > 180) {
        pathFinder(traker.test_cnt, (360 - 33
        pathFinder(traker.test_cnt, (360 - traker.test_cnt) / 6);
        traker.test_cnt++;
      }
      else {
        traker.test_step = 3;
        traker.test_cnt = 0;
      }
      break;
    case 3:
      if (traker.test_cnt < 360) {
        pathFinder(traker.test_cnt, traker.test_cnt / 4);
        traker.test_cnt++;
      }
      else {
        traker.test_step = 4;
        traker.test_cnt = 0;
      }
      break;
    case 4:
      if (traker.test_cnt < 360) {
        pathFinder(traker.test_cnt, 90 - (traker.test_cnt / 4));
        traker.test_cnt++;
      }
      else {
        // finished
        traker.test_step = 0;
        pathFinder(0, 0);
      }
      break;
  }
#ifdef DEBUG_SERVOTEST
  SERIAL_PRINT("STEP: ");
  SERIAL_PRINTLN(traker.test_step);
  SERIAL_PRINT("CNT: ");
  SERIAL_PRINTLN(traker.test_cnt);
#endif
  return traker.test_step;
}


void initializeTraker()
{
  Pam.attach(PAM_Pin, cnf.pan_minpwm, cnf.pan_maxpwm); //PAN Servos
  Tilt.attach(TILT_Pin, cnf.tilt_minpwm, cnf.tilt_maxpwm); //Tilt Elevator
  //Pam.write(0);
  //Tilt.write(0);
  pathFinder(0, 0);
}

void saveHomePosition() // Used to save lat/lon/altitude for tilt/pan computations
{
  home.latitude = uav.latitude;
  home.longitud = uav.longitud;
  home.altitude = uav.altitude;
  //eeprom_busy_wait();
  //ee.write_dword(0x00,home);
}


void updatePosition()
{


  if (telemetry_ok) {

    float lat1 = home.latitude / 1.0e7;
    float lon1 = home.longitud / 1.0e7;
    float lat2 = uav.latitude / 1.0e7;
    float lon2 = uav.longitud / 1.0e7;

    home.alt_relative = uav.altitude - home.altitude;
    float x = 69.1 * (lat2 - lat1);
    float y = 69.1 * (lon2 - lon1) * cos(lat1 / 57.295775);

    traker.bearing = toDeg(atan2(y, x));
    if (traker.bearing < 0) traker.bearing += 360;


    home.distance = (float)sqrt((float)(x * x) + (float)(y * y)) * 1609.344;
    home.distance3d = sqrt((home.distance * home.distance) + (home.alt_relative * home.alt_relative));



    traker.elevation = toDeg(atan((home.alt_relative) / home.distance));

#ifdef DEGUG_TRAKER
    SERIAL_PRINTLN("=====Traker=====");
    SERIAL_PRINT("bearing: ");
    SERIAL_PRINTLN(traker.bearing);
    SERIAL_PRINT("distance: ");
    SERIAL_PRINTLN(home.distance);
    SERIAL_PRINT("distance 3d: ");
    SERIAL_PRINTLN(home.distance3d);
    SERIAL_PRINT("elevation: ");
    SERIAL_PRINTLN( traker.elevation);
    SERIAL_PRINT("HOLD: ");
    SERIAL_PRINTLN( traker.hold);
#endif

    pathFinder(traker.bearing, traker.elevation);

    telemetry_ok = false;
  }
}

#endif






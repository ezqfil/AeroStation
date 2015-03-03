#ifndef _AS_UIDATA_H_
#define _AS_UIDATA_H_

void mnuData(){

  if(uiStatus.state==0){
    uiStatus.state=1;
  }
  else if(uiStatus.action==50){
    uiStatus.state--;
    uiStatus.state = constrain(uiStatus.state,1,3);
  } 
  else if(uiStatus.action==51){ 
    uiStatus.state++;
    uiStatus.state = constrain(uiStatus.state,1,3);
  } 

  if(uiStatus.state!=uiStatus.statePrevius) {
    if(uiStatus.state==1){
      SERIAL_PRINTLN("\n---------------------");
      SERIAL_PRINT("WP#~      DIS~"); 
      SERIAL_PRINTLN(home.distance);
      SERIAL_PRINT("CRS~      BER~"); 
      SERIAL_PRINTLN(traker.bearing);
      SERIAL_PRINT("GSP~      ASP~\n");
      SERIAL_PRINT("ALT~"); 
      SERIAL_PRINT(uav.altitude); 
      SERIAL_PRINT(" RSP~\n");
    }
    else if(uiStatus.state==2){ 
      SERIAL_PRINTLN("\n---------------------");
      SERIAL_PRINT("Roll~"); 
      SERIAL_PRINT(uav.roll); 
      SERIAL_PRINT(" Pitch~"); 
      SERIAL_PRINTLN(uav.pitch); 
      SERIAL_PRINTLN("ASP~      GSP~");
      SERIAL_PRINT("ALT~"); 
      SERIAL_PRINT(uav.altitude); 
      SERIAL_PRINTLN(" DAL~");
      SERIAL_PRINT("CRS~      Batt~"); 
      SERIAL_PRINT(uav.bat_voltage); 
      SERIAL_PRINT("V "); 
      SERIAL_PRINT(uav.bat_current);
      SERIAL_PRINT("A "); 
      SERIAL_PRINT(uav.bat_capacity); 
      SERIAL_PRINTLN("mAh");
    }
    else if(uiStatus.state==3){ 
      SERIAL_PRINTLN("\n---------------------");
      SERIAL_PRINT("Dist2D~"); 
      SERIAL_PRINTLN(home.distance);
      SERIAL_PRINT("Dist3D~"); 
      SERIAL_PRINTLN(home.distance3d);
      SERIAL_PRINT("Angle~"); 
      SERIAL_PRINT(traker.elevation); 
      SERIAL_PRINT(" Alt~"); 
      SERIAL_PRINTLN(home.alt_relative);
      SERIAL_PRINT("Dir~"); 
      SERIAL_PRINTLN(traker.bearing); 
    }    
    uiStatus.statePrevius=uiStatus.state;
  }

  if(uiStatus.action==49){
    uiStatus.event = mnuMain;
  } 


}

#endif


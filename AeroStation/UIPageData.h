#ifndef _AS_UIDATA_H_
#define _AS_UIDATA_H_

void mnuData(){

  if(uiStatus.state==0){
    uiStatus.state=1;
  }//22266
  else if(uiStatus.action==BTN_MINUS && uiStatus.state>1){
    uiStatus.state--;
    //uiStatus.state = constrain(uiStatus.state,1,3);
  } 
  else if(uiStatus.action==BTN_PLUS && uiStatus.state<3){ 
    uiStatus.state++;
    //uiStatus.state = constrain(uiStatus.state,1,3);
  } 

  if(uiStatus.state!=uiStatus.statePrevius) {
    if(uiStatus.state==1){
      DEBUG_PRINTLN("\n---------------------");
      DEBUG_PRINT("WP#~      DIS~"); 
      DEBUG_PRINTLN(home.distance);
      DEBUG_PRINT("CRS~      BER~"); 
      DEBUG_PRINTLN(traker.bearing);
      DEBUG_PRINT("GSP~      ASP~\n");
      DEBUG_PRINT("ALT~"); 
      DEBUG_PRINT(uav.altitude); 
      DEBUG_PRINT(" RSP~\n");
    }
    else if(uiStatus.state==2){ 
      DEBUG_PRINTLN("\n---------------------");
      DEBUG_PRINT("Roll~"); 
      DEBUG_PRINT(uav.roll); 
      DEBUG_PRINT(" Pitch~"); 
      DEBUG_PRINTLN(uav.pitch); 
      DEBUG_PRINTLN("ASP~      GSP~");
      DEBUG_PRINT("ALT~"); 
      DEBUG_PRINT(uav.altitude); 
      DEBUG_PRINTLN(" DAL~");
      DEBUG_PRINT("CRS~      Batt~"); 
      DEBUG_PRINT(uav.bat_voltage); 
      DEBUG_PRINT("V "); 
      DEBUG_PRINT(uav.bat_current);
      DEBUG_PRINT("A "); 
      DEBUG_PRINT(uav.bat_capacity); 
      DEBUG_PRINTLN("mAh");
    }
    else if(uiStatus.state==3){ 
      DEBUG_PRINTLN("\n---------------------");
      DEBUG_PRINT("Dist2D~"); 
      DEBUG_PRINTLN(home.distance);
      DEBUG_PRINT("Dist3D~"); 
      DEBUG_PRINTLN(home.distance3d);
      DEBUG_PRINT("Angle~"); 
      DEBUG_PRINT(traker.elevation); 
      DEBUG_PRINT(" Alt~"); 
      DEBUG_PRINTLN(home.alt_relative);
      DEBUG_PRINT("Dir~"); 
      DEBUG_PRINTLN(traker.bearing); 
    }    
    uiStatus.statePrevius=uiStatus.state;
  }

  if(uiStatus.action==BTN_MENU){
    uiStatus.event = mnuMain;
  } 


}

#endif


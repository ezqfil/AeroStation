#ifndef _AS_UICONFIG_H_
#define _AS_UICONFIG_H_
//16946
void smnBaudrate(){
  boolean change=false;

  if(uiStatus.state==0){
    uiStatus.state++;
    change=true;
  }//20632
  else if(uiStatus.action==BTN_MINUS && cnf.baudrate>0) {
    cnf.baudrate--;
   // cnf.baudrate = constrain( cnf.baudrate,0,7);
    change=true;
  } 
  else if( uiStatus.action==BTN_PLUS && cnf.baudrate<7) {
    cnf.baudrate++;
   // cnf.baudrate = constrain( cnf.baudrate,0,7);
    change=true;
  }
  else if( uiStatus.action==BTN_OK || uiStatus.action==BTN_MENU) {
    uiStatus.event=mnuConfig;
  }    


  if(change){
    for(byte i = 0; i<4; i++) {
      if(cnf.baudrate==i){
        DEBUG_PRINT(" -> ");
      } 
      else {
        DEBUG_PRINT("    ");
      }
      DEBUG_PRINTLN(baudrates[i]);
    }
    for(byte i = 4; i<8; i++) {
      if(cnf.baudrate==i){
        DEBUG_PRINT(" -> ");
      } 
      else {
        DEBUG_PRINT("    ");
      }
      DEBUG_PRINTLN(baudrates[i]);
    }
  } 
}

void smnServos(){
  boolean change=false;
  if(uiStatus.state==0){
    uiStatus.state=1;
    change=true;
  }
  else if(uiStatus.action==BTN_OK){
    uiStatus.state++;
    change=true;
  }
  else if(uiStatus.action==BTN_MENU){
    uiStatus.event = mnuConfig;
  }//20382
  else if(uiStatus.action==BTN_MINUS) {
    change=true;
    if(uiStatus.state== 1){
      cnf.pan_minpwm--;
    }   
    else if(uiStatus.state==2 ){
      cnf.pan_maxpwm--;
    }   
    else if(uiStatus.state==3){
      cnf.pan_minangle--;
    }   
    else if(uiStatus.state==4 ){
      cnf.tilt_maxangle--;
    }   
    else if(uiStatus.state==5){
      cnf.tilt_minpwm--;
    } 
    else if(uiStatus.state==6 ){
      cnf.tilt_maxpwm--;
    }   
    else if(uiStatus.state==7){
      cnf.tilt_minangle--;
    }   
    else if(uiStatus.state==8 ){
      cnf.tilt_maxangle--;
    } 
  }
  else if(uiStatus.action==BTN_PLUS) {
    change=true;
    if(uiStatus.state==1){
      cnf.pan_minpwm++;
    } 
    else if(uiStatus.state==2 ){
      cnf.pan_maxpwm++;
    } 
    else if(uiStatus.state==3 ){
      cnf.pan_minangle++;
    } //16906
    else if(uiStatus.state==4 ){
      cnf.pan_maxangle++;
    }
    else if(uiStatus.state==5 ){
      cnf.tilt_minpwm++;
    } 
    else if(uiStatus.state==6 ){
      cnf.tilt_maxpwm++;
    } 
    else if(uiStatus.state==7 ){
      cnf.tilt_minangle++;
    } //16906
    else if(uiStatus.state==8 ){
      cnf.tilt_maxangle++;
    }  
  }

  if(change) {
    if(uiStatus.state<5){
      DEBUG_PRINTLN("PAN     MIN    MAX ");
      DEBUG_PRINT("PWM     ");
      DEBUG_PRINT(cnf.pan_minpwm);
      DEBUG_PRINT("    ");
      if(uiStatus.state>1)
        DEBUG_PRINT(cnf.pan_maxpwm);

      DEBUG_PRINTLN();
      DEBUG_PRINT("ANGLE   ");
      if(uiStatus.state>2)
        DEBUG_PRINT(cnf.pan_minangle);
      DEBUG_PRINT("      ");
      if(uiStatus.state>3)
        DEBUG_PRINT(cnf.pan_maxangle);
      DEBUG_PRINTLN();
    } 
    else if(uiStatus.state<9){
      DEBUG_PRINTLN("TILT    MIN    MAX ");
      DEBUG_PRINT("PWM     ");
      DEBUG_PRINT(cnf.tilt_minpwm);
      DEBUG_PRINT("    ");
      if(uiStatus.state>5)
        DEBUG_PRINT(cnf.tilt_maxpwm);
      DEBUG_PRINTLN();
      DEBUG_PRINT("ANGLE   ");
      if(uiStatus.state>6)
        DEBUG_PRINT(cnf.tilt_minangle);
      DEBUG_PRINT("      ");
      if(uiStatus.state>7)
        DEBUG_PRINT(cnf.tilt_maxangle);
      DEBUG_PRINTLN();
    } 
    else {
      uiStatus.event = mnuConfig;
      //uiStatus.state=3;
    }
    uiStatus.statePrevius=uiStatus.state;
  }

}

void smnSetHome(){
  if(uiStatus.state==0){
    DEBUG_PRINTLN("Set Home");
    DEBUG_PRINT(" Lat~"); 
    DEBUG_PRINT(uav.latitude); 
    DEBUG_PRINT(" Lon~"); 
    DEBUG_PRINTLN(uav.longitud);
    DEBUG_PRINT("->Confirm?");
    uiStatus.state++;
  } 
  if( uiStatus.action==BTN_OK) {
    saveHomePosition();
    uiStatus.event = mnuData;
  }
  else if( uiStatus.action==BTN_MENU) {
    uiStatus.event = mnuConfig;
  }
}

void mnuConfig(){
  if(uiStatus.state==0){
    uiStatus.state=1;
  }//2264
  else 
    if(uiStatus.action==BTN_MINUS && uiStatus.state>1){
    uiStatus.state--;
    //uiStatus.state = constrain(uiStatus.state,1,6);
  } 
  else if(uiStatus.action==BTN_PLUS && uiStatus.state<6){
    uiStatus.state++;
    //uiStatus.state = constrain(uiStatus.state,1,6);
  }else if(uiStatus.action==BTN_OK && uiStatus.state==2){
    traker.hold=~traker.hold;
    uiStatus.statePrevius=-1;
    //uiStatus.state = constrain(uiStatus.state,1,6);
  } 

  if(uiStatus.state!=uiStatus.statePrevius) {
    char* strHold[ ]={"Antenna Hold on",
  "Antenna Hold off"};
    if(uiStatus.state==1){
      DEBUG_PRINTLN("1 -> Set Home");
      DEBUG_PRINT("2    Antenna Hold ");
      if(traker.hold)
        DEBUG_PRINTLN("OFF");
      else
        DEBUG_PRINTLN("ON");
      DEBUG_PRINTLN("3    PAM/TiLT ");
      DEBUG_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==2){
      DEBUG_PRINTLN("1    Set Home");
      DEBUG_PRINT("2 -> Antenna Hold ");
      if(traker.hold)
        DEBUG_PRINTLN("OFF");
      else
        DEBUG_PRINTLN("ON");
      DEBUG_PRINTLN("3    PAM/TiLT ");
      DEBUG_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==3){
      DEBUG_PRINTLN("1    Set Home");
      DEBUG_PRINT("2    Antenna Hold ");
      if(traker.hold)
        DEBUG_PRINTLN("OFF");
      else
        DEBUG_PRINTLN("ON");
      DEBUG_PRINTLN("3 -> PAM/TiLT ");
      DEBUG_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==4){
      DEBUG_PRINTLN("1    Set Home");
      DEBUG_PRINT("2    ");
      DEBUG_PRINTLN(strHold[traker.hold]);
      DEBUG_PRINTLN("3    PAM/TiLT ");
      DEBUG_PRINTLN("4 -> Reset configuration");
    } 
    else if(uiStatus.state==5){
      DEBUG_PRINTLN("5 -> Save configuration");
      DEBUG_PRINTLN("6    Telemetry Baudrate");
    } 
    else if(uiStatus.state==6){
      DEBUG_PRINTLN("5    Save configuration");
      DEBUG_PRINTLN("6 -> Telemetry Baudrate");
    } 
    uiStatus.statePrevius=uiStatus.state;
  }  

  if(uiStatus.state==1 && uiStatus.action==BTN_OK) {
    uiStatus.event=smnSetHome;
  } 
  else if(uiStatus.state==3 && uiStatus.action==BTN_OK) {
    uiStatus.event = smnServos;
    //return;
  }  
  else if(uiStatus.state==4 && uiStatus.action==BTN_OK) {
    resetConfiguration();
    //uiStatus.state=4;
    //return;
  }  
  else if(uiStatus.state==5 && uiStatus.action==BTN_OK) {
    saveConfiguration();
    // uiStatus.state=5;
    // return;
  }  
  else if(uiStatus.state==6 && uiStatus.action==BTN_OK) {
    uiStatus.event=smnBaudrate;
    //return; 
  } 
  else if(uiStatus.action==BTN_MENU){
    uiStatus.event = mnuMain;
  } 

  //uiStatus.statePrevius=uiStatus.state;
}

#endif





































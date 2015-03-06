#ifndef _AS_UICONFIG_H_
#define _AS_UICONFIG_H_
//16946
void smnBaudrate(){
  boolean change=false;

  if(uiStatus.state==0){
    uiStatus.state++;
    change=true;
  }
  else if(uiStatus.action==BTN_MINUS) {
    cnf.baudrate--;
    cnf.baudrate = constrain( cnf.baudrate,0,7);
    change=true;
  } 
  else if( uiStatus.action==BTN_PLUS) {
    cnf.baudrate++;
    cnf.baudrate = constrain( cnf.baudrate,0,7);
    change=true;
  }
  else if( uiStatus.action==BTN_OK || uiStatus.action==BTN_MENU) {
    uiStatus.event=mnuConfig;
  }    


  if(change){
    for(byte i = 0; i<4; i++) {
      if(cnf.baudrate==i){
        SERIAL_PRINT(" -> ");
      } 
      else {
        SERIAL_PRINT("    ");
      }
      SERIAL_PRINTLN(baudrates[i]);
    }
    for(byte i = 4; i<8; i++) {
      if(cnf.baudrate==i){
        SERIAL_PRINT(" -> ");
      } 
      else {
        SERIAL_PRINT("    ");
      }
      SERIAL_PRINTLN(baudrates[i]);
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
      SERIAL_PRINTLN("PAN     MIN    MAX ");
      SERIAL_PRINT("PWM     ");
      SERIAL_PRINT(cnf.pan_minpwm);
      SERIAL_PRINT("    ");
      if(uiStatus.state>1)
        SERIAL_PRINT(cnf.pan_maxpwm);

      SERIAL_PRINTLN();
      SERIAL_PRINT("ANGLE   ");
      if(uiStatus.state>2)
        SERIAL_PRINT(cnf.pan_minangle);
      SERIAL_PRINT("      ");
      if(uiStatus.state>3)
        SERIAL_PRINT(cnf.pan_maxangle);
      SERIAL_PRINTLN();
    } 
    else if(uiStatus.state<9){
      SERIAL_PRINTLN("TILT    MIN    MAX ");
      SERIAL_PRINT("PWM     ");
      SERIAL_PRINT(cnf.tilt_minpwm);
      SERIAL_PRINT("    ");
      if(uiStatus.state>5)
        SERIAL_PRINT(cnf.tilt_maxpwm);
      SERIAL_PRINTLN();
      SERIAL_PRINT("ANGLE   ");
      if(uiStatus.state>6)
        SERIAL_PRINT(cnf.tilt_minangle);
      SERIAL_PRINT("      ");
      if(uiStatus.state>7)
        SERIAL_PRINT(cnf.tilt_maxangle);
      SERIAL_PRINTLN();
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
    SERIAL_PRINTLN("Set Home");
    SERIAL_PRINT(" Lat~"); 
    SERIAL_PRINT(uav.latitude); 
    SERIAL_PRINT(" Lon~"); 
    SERIAL_PRINTLN(uav.longitud);
    SERIAL_PRINT("->Confirm?");
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
      SERIAL_PRINTLN("1 -> Set Home");
      SERIAL_PRINT("2    Antenna Hold ");
      if(traker.hold)
        SERIAL_PRINTLN("OFF");
      else
        SERIAL_PRINTLN("ON");
      SERIAL_PRINTLN("3    PAM/TiLT ");
      SERIAL_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==2){
      SERIAL_PRINTLN("1    Set Home");
      SERIAL_PRINT("2 -> Antenna Hold ");
      if(traker.hold)
        SERIAL_PRINTLN("OFF");
      else
        SERIAL_PRINTLN("ON");
      SERIAL_PRINTLN("3    PAM/TiLT ");
      SERIAL_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==3){
      SERIAL_PRINTLN("1    Set Home");
      SERIAL_PRINT("2    Antenna Hold ");
      if(traker.hold)
        SERIAL_PRINTLN("OFF");
      else
        SERIAL_PRINTLN("ON");
      SERIAL_PRINTLN("3 -> PAM/TiLT ");
      SERIAL_PRINTLN("4    Reset configuration");
    } 
    else if(uiStatus.state==4){
      SERIAL_PRINTLN("1    Set Home");
      SERIAL_PRINT("2    ");
      SERIAL_PRINTLN(strHold[traker.hold]);
      SERIAL_PRINTLN("3    PAM/TiLT ");
      SERIAL_PRINTLN("4 -> Reset configuration");
    } 
    else if(uiStatus.state==5){
      SERIAL_PRINTLN("5 -> Save configuration");
      SERIAL_PRINTLN("6    Telemetry Baudrate");
    } 
    else if(uiStatus.state==6){
      SERIAL_PRINTLN("5    Save configuration");
      SERIAL_PRINTLN("6 -> Telemetry Baudrate");
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





































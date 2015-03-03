#ifndef _AS_UIMAIN_H_
#define _AS_UIMAIN_H_


void mnuMain(void)
{
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
      SERIAL_PRINTLN("1 -> Data");
      SERIAL_PRINTLN("2    Config");
      SERIAL_PRINTLN("3    Test telemetry");
    } 
    else if(uiStatus.state==2){
      SERIAL_PRINTLN("1    Data");
      SERIAL_PRINTLN("2 -> Config");
      SERIAL_PRINTLN("3    Test telemetry");
    }
    else
      if(uiStatus.state==3 ){
        SERIAL_PRINTLN("1    Data");
        SERIAL_PRINTLN("2    Config");
        SERIAL_PRINTLN("3 -> Test telemetry");
      }
    uiStatus.statePrevius=uiStatus.state;
  }

  if(uiStatus.action==52 ){
    if(uiStatus.state==1){
      uiStatus.event = mnuData;
    } 
    else if( uiStatus.state==2){
      uiStatus.event = mnuConfig;
    }
    else if(uiStatus.state==3){
#ifdef TELEMETRY_TEST
      uiStatus.event = mnuTelemetryTest;
#else
      SERIAL_PRINTLN("Not implemented");
#endif
    } 
  }
  else if(uiStatus.action==49){
    uiStatus.event = mnuData;
  } 


}

#endif




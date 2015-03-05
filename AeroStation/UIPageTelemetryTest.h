#ifndef _AS_UITELEMETRYTEST_H_
#define _AS_UITELEMETRYTEST_H_

void mnuTelemetryTest(void){
  if(uiStatus.state==0){
    uiStatus.state=1;
  }
  else if(uiStatus.action==BTN_MINUS && uiStatus.state>1){
    uiStatus.state--; 
    //uiStatus.state = constrain(uiStatus.state,1,4);
  } 
  else if(uiStatus.action==BTN_PLUS && uiStatus.state<4){
    uiStatus.state++;
    //uiStatus.state = constrain(uiStatus.state,1,4);
  } 

  if(uiStatus.state!=uiStatus.statePrevius) {
    if(uiStatus.state==1){
      SERIAL_PRINTLN("1 -> AQ Telemetry ");
      SERIAL_PRINTLN("2    LTM G Telemetry ");
      SERIAL_PRINTLN("3    LTM S Telemetry ");
      SERIAL_PRINTLN("4    LTM A Telemetry ");
    } 
    else if(uiStatus.state==2){
      SERIAL_PRINTLN("1    AQ Telemetry ");
      SERIAL_PRINTLN("2 -> LTM G Telemetry ");
      SERIAL_PRINTLN("3    LTM S Telemetry ");
      SERIAL_PRINTLN("4    LTM A Telemetry ");
    } 
    else if(uiStatus.state==3){
      SERIAL_PRINTLN("1    AQ Telemetry ");
      SERIAL_PRINTLN("2    LTM G Telemetry ");
      SERIAL_PRINTLN("3 -> LTM S Telemetry ");
      SERIAL_PRINTLN("4    LTM A Telemetry ");
    } 
    else if(uiStatus.state==4){
      SERIAL_PRINTLN("1    AQ Telemetry ");
      SERIAL_PRINTLN("2    LTM G Telemetry ");
      SERIAL_PRINTLN("3    LTM S Telemetry ");
      SERIAL_PRINTLN("4 -> LTM A Telemetry ");
    } 
    uiStatus.statePrevius=uiStatus.state;
  }

  if(uiStatus.action==BTN_OK) {
    if(uiStatus.state==1 ){
      initializeAQTelemetry();
      //slowTelemetryByte=255;
      //    readTelemetryByte=0;
    } 
    else if(uiStatus.state==2 ){
      initializeLTMGTelemetry();
      //slowTelemetryByteLTM = 255;
      //    readTelemetryByte=0;
    } 
    else if(uiStatus.state==3 ){
      initializeLTMSTelemetry();
      //slowTelemetryByteLTM = 255;
      //s=3;
      //    readTelemetryByte=0;
    } 
    else if(uiStatus.state==4){
      initializeLTMATelemetry();
      //slowTelemetryByteLTM = 255;
      //s=3;
      //    readTelemetryByte=0;
    } 
  }
  else if(uiStatus.action==BTN_MENU){
    uiStatus.event = mnuMain;
  } 
}

#endif


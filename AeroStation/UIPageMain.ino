#ifndef _AS_UIMAIN_H_
#define _AS_UIMAIN_H_

#include "UIPageConfig.h"
#include "UIPageData.h"
#include "UIPageTest.h"

void mnuMain(void)
{//2268
  if(uiStatus.state==0){
    uiStatus.state=1;
  }
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
      DEBUG_PRINTLN("1 -> Data");
      DEBUG_PRINTLN("2    Config");
      DEBUG_PRINTLN("3    Test");
    } 
    else if(uiStatus.state==2){
      DEBUG_PRINTLN("1    Data");
      DEBUG_PRINTLN("2 -> Config");
      DEBUG_PRINTLN("3    Test");
    } 
    else if(uiStatus.state==3 ){
      DEBUG_PRINTLN("1    Data");
      DEBUG_PRINTLN("2    Config");
      DEBUG_PRINTLN("3 -> Test");
    }
    uiStatus.statePrevius=uiStatus.state;
  }

  if(uiStatus.action==BTN_OK ){
    if(uiStatus.state==1){
      uiStatus.event = mnuData;
    } 
    else if( uiStatus.state==2){
      uiStatus.event = mnuConfig;
    }
    else if(uiStatus.state==3){
      uiStatus.event = mnuTest;
    } 
  }
  else if(uiStatus.action==BTN_MENU){
    uiStatus.event = mnuData;
  } 


}

#endif






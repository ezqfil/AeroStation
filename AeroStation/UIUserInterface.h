#ifndef _AS_UI_H_
#define _AS_UI_H_

#define BTN_MENU  49
#define BTN_MINUS  50
#define BTN_PLUS  51
#define BTN_OK 52

//Variables:
typedef void (*FuncPtr)(void);

typedef struct {
  byte state;
  byte action;
  byte step;//notused
  byte statePrevius;

  FuncPtr previusEvent; 
  FuncPtr event;
} 
t_UI_STATUS;


extern void mnuMain(void);
extern void mnuConfig(void);	
extern void mnuData(void);	
extern void mnuTelemetryTest(void);

t_UI_STATUS uiStatus={
  0,
  0,0,
  0,mnuMain,mnuMain};  


void initializeUI(){
  // initialize the library with the numbers of the interface pins
  LiquidCrystal screen(12, 11, 5, 4, 3, 2);
  // set up the LCD's number of columns and rows: 
  screen.begin(20, 4);
}



void readButton() {
  //byte s;
  while(SERIAL_AVAILABLE()){
    uiStatus.action=SERIAL_READ();
    SERIAL_READ();
   // SERIAL_READ();
  }
}





void updateUI(){

  readButton();
  if(uiStatus.previusEvent!=uiStatus.event){
    uiStatus.previusEvent=uiStatus.event;
    uiStatus.state=0;
    uiStatus.statePrevius=-1;
  }
  uiStatus.event();
  uiStatus.action=0;
  //uiStatus.statePrevius=uiStatus.state;
}



#endif


















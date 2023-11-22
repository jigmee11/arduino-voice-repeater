#include <U8g2lib.h>
#include <U8x8lib.h>

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

int systemState = 0;
unsigned long startTime = -1;
unsigned long playBackTime = 0;
const int recButton = 7;
const int playButton = 6; // Moved this pin to avoid conflict with LCD connections

void setup() {  

  Serial.begin(9600); 
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_6x10_tf); 
  u8g2.setColorIndex(1);  
  pinMode(recButton, INPUT_PULLUP);
  pinMode(playButton, INPUT_PULLUP);
}

void loop() {  
  if (digitalRead(recButton) == HIGH) {
    if(startTime==-1) startTime = millis() / 1000;

     digitalWrite(playButton, LOW);
    systemState = 1;
  }
  if (digitalRead(playButton) == HIGH) {
    digitalWrite(recButton, LOW);
    systemState = 2;
  }
  if (digitalRead(recButton) == LOW) {
    playBackTime = (millis() / 1000)-startTime;
    startTime = -1;
  }
  if (digitalRead(playButton) == LOW && digitalRead(recButton) == LOW) {
    if(systemState==1){
      systemState = 3;
    }
    else if(systemState != 3){
      systemState = 0;
    }
  }

  u8g2.firstPage();
  do {   
    draw();
  } while( u8g2.nextPage() );
}
  
void draw(){
  if(systemState==0){
    u8g2.drawStr(20, 30, "Ready to record");
    return;
  }

  if(systemState==1){
    u8g2.drawStr(20, 30, "Recording");
    unsigned long timeSpent = (millis() / 1000)-startTime;
    
    Serial.println(timeSpent);
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", 0, 0, timeSpent);
    u8g2.drawStr(20, 50, timeStr);
    return;
  }

  if(systemState==2){
    u8g2.drawStr(20, 30, "Playing");
    return;
  }

  if(systemState==3){
    u8g2.drawStr(20, 30, "Ready for playback");
    return;
  }
}
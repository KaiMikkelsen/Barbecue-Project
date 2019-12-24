#include "U8glib.h"
#include "max6675.h"


int ktcSO = 8;
int ktcCS = 9;
int ktcCLK = 10;
int newpotvalue = 0;
int oldpotvalue = 0;
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE); 

// Function that takes care of whats on the screen
void drawnumber(int goal, int current) 
{
  
  u8g.setFont(u8g_font_fur14);
  u8g.drawStr(100, 30, "C");

  
  u8g.setFont(u8g_font_fur11);
  u8g.setPrintPos(80, 15); 
  u8g.print(goal); 
  
  u8g.setFont(u8g_font_fur20);
  u8g.setPrintPos(2, 30); 
  u8g.print(current); 

}

  


void setup(void) {

 
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);

  oldpotvalue = analogRead(A0);
  
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
}

void loop(void) {

  u8g.firstPage();  

// read temp sensor
  newpotvalue = analogRead(A0);

  if(newpotvalue != 0 && newpotvalue != 1023 && abs((newpotvalue*0.78) - oldpotvalue) > 5)
  {
    oldpotvalue = newpotvalue * 0.78;
  }

// deciding to turn fan on or off
  if(oldpotvalue > ktc.readFarenheit())
  {
    digitalWrite(3,HIGH);
  }
  else
  {
    digitalWrite(3,LOW);
  }


  
  do{
    drawnumber(oldpotvalue, ktc.readFarenheit());
    }while( u8g.nextPage() );
  

  
    // print out the value you read:
   Serial.println(oldpotvalue);
 
   Serial.print("Deg C = "); 
   Serial.print(ktc.readCelsius());
  
   
  // rebuild the picture after some delay
  delay(300);
}

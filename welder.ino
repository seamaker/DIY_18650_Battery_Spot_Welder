/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.
 # This Sample code is for testing the Relay shield V2.1 for Arduino.
  
 # Editor : Phoebe https://www.dfrobot.com/wiki/index.php/Relay_Shield_for_Arduino_V2.1_(SKU:DFR0144)#Sample_Code
 # Date   : 2013.2.28
 # Ver    : 0.1
 # Product: Relay shield for Arduino
 # SKU    : DRI0144
   
 # Hardwares:
 1. Arduino UNO
 2. Relay Shield For Arduino V2.1 
 3  Power Supply:7~ 12V

DIY spot welder by Bob Edmiston
Ingredients:
Sparkfun Redboard
DFRobot Relay Shield
Adafruit RGB LCD button shield
500 Amp truck starter relay
Really big motorcycle or small car lead-acid or SLA battery

Build at your own risk.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
#define BASETIME 35
#define INCREMENT 5
#define RECHARGE 2000

byte relayPin[4] = {2,7,8,10};
int onTime = BASETIME;
int offTime = 2000;
int pushButton = 0;
int triggerSwitch = 0;
//D2 -> RELAY1
//D7 -> RELAY2
//D8 -> RELAY3
//D10 -> RELAY4

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  //int time = millis();
  lcd.print("Ready");
  //time = millis() - time;
  //Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);
  for(int i = 0; i < 4; i++)  pinMode(relayPin[i],OUTPUT);
    // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  digitalWrite(pushButton, HIGH);
  pinMode(11, INPUT_PULLUP);
}

uint8_t i=0;
uint8_t buttons;
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //int buttonState = digitalRead(pushButton); // read push button, if pushed, should go low
  lcd.setCursor(0, 1);
  buttons = lcd.readButtons();
  triggerSwitch = digitalRead(11);
  lcd.print(onTime);
  lcd.print(" ms");
  Serial.println(triggerSwitch);
 
  if (buttons || (!triggerSwitch)) {
    if (buttons & BUTTON_SELECT) onTime = BASETIME;
    if (buttons & BUTTON_UP) onTime += INCREMENT;
    if (buttons & BUTTON_DOWN) {
      if (onTime > INCREMENT) onTime -= INCREMENT;    
    }
    if ((buttons & BUTTON_LEFT) || (!triggerSwitch)){
      digitalWrite(relayPin[2],LOW);
      digitalWrite(relayPin[3],HIGH);
      delay(onTime);
      digitalWrite(relayPin[3],LOW);
      lcd.clear();
      lcd.print("recharging");
      digitalWrite(relayPin[2],HIGH);
      offTime = RECHARGE;
      while (offTime > 0){
        lcd.setCursor(0,1);
        lcd.print(offTime);
        lcd.print(" ms ");
        offTime -= RECHARGE/20;
        delay(RECHARGE/20-95);
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ready");
    }
  }
  while (buttons){  
    buttons = lcd.readButtons();
    delay(onTime);
  }
  while (!triggerSwitch){  
    triggerSwitch = digitalRead(11);
    delay(onTime);
  }
}

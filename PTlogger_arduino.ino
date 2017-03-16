#include <Wire.h>
#include <SD.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MAX31865.h>

// Display definitions, Adafruit Feather OLED
#define OLED_RESET 10
Adafruit_SSD1306 display(OLED_RESET);
#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// Choose your own 128x32 logo
static const unsigned char PROGMEM logo_bmp[] =
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x1f, 0xff, 0x83, 0x8f, 0xf8, 0x1f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xe0, 0x07, 0xfe, 0x00, 0x8f, 0xe0, 0x07, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xc0, 0x03, 0xfc, 0x00, 0x0f, 0xc0, 0x03, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x83, 0xc1, 0xf8, 0x3c, 0x0f, 0x83, 0xc1, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x8f, 0xe1, 0xf8, 0xff, 0x0f, 0x8f, 0xe1, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x0f, 0xf0, 0xf0, 0xff, 0x0f, 0x0f, 0xf0, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x1f, 0xf8, 0xf1, 0xff, 0x8f, 0x1f, 0xf8, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x0f, 0xf0, 0xf0, 0xff, 0x0f, 0x0f, 0xf0, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x8f, 0xf1, 0xf8, 0xff, 0x0f, 0x8f, 0xf1, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x83, 0xc1, 0xf8, 0x3c, 0x0f, 0x83, 0xc1, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xc0, 0x03, 0xfc, 0x00, 0x0f, 0xc0, 0x03, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xe0, 0x07, 0xfe, 0x00, 0x8f, 0xe0, 0x07, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xf8, 0x1f, 0xff, 0x83, 0x8f, 0xf8, 0x1f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0x1f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

unsigned int LoopIntervalSec = 5;  //global SD loop interval variable, seconds

// MAX31865 initialization, CS pin configuration
Adafruit_MAX31865 max1 = Adafruit_MAX31865(A0);
Adafruit_MAX31865 max2 = Adafruit_MAX31865(A2); 
Adafruit_MAX31865 max3 = Adafruit_MAX31865(A4);

void setup()   
{
  // set all SPI CS pins to high, precaution
  pinMode(4,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
  digitalWrite(4,HIGH);
  digitalWrite(A0,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A4,HIGH);
  digitalWrite(20,HIGH);
  digitalWrite(21,OUTPUT);
  
  
  // display initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // display init, I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  // draw init screen
  display.setTextSize(1);  
  display.drawBitmap(40, 0,  logo_bmp, 128, 32, 1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.drawLine(39,0,39,32,WHITE);
  display.println("PT100");
  display.println("PT1000");
  display.println("Temp");
  display.println("logger");
  display.display();
  delay(1000);

  // check 2Wire / 4Wire jumper settings
  if (digitalRead(13) == HIGH)
    max1.begin(MAX31865_4WIRE);
  else
    max1.begin(MAX31865_2WIRE);
  if (digitalRead(12) == HIGH)
    max2.begin(MAX31865_4WIRE);
  else
    max2.begin(MAX31865_2WIRE);
  if (digitalRead(11) == HIGH)
    max3.begin(MAX31865_4WIRE);
  else
    max3.begin(MAX31865_2WIRE);

  // Adafruit feather oled display button initialization
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // Check all 3 channels for errors
  uint8_t status;
  max1.readRTD();
  max2.readRTD();
  max3.readRTD();
  // display channel check
  display.clearDisplay();
  display.setTextSize(1);  
  display.drawBitmap(40, 0,  logo_bmp, 128, 32, 1);
  display.setCursor(0,0);
  display.println("*INIT*");
  display.display();
  if (max1.readFault())
    display.println("CH1:ERR");
  else
      display.println("CH1:ok");
  display.display();
  if (max2.readFault())
    display.println("CH2:ERR");
  else
      display.println("CH2:ok");
  display.display();
  if (max3.readFault())
    display.println("CH3:ERR");
  else
      display.println("CH3:ok");
  display.display();
}



void loop() {
   uint8_t status;
   uint16_t rREF, rPT;
   double temperature, resistance;



   //BUTTON_A: simple print of temperature and resistance to display
   if (! digitalRead(BUTTON_A)) 
   { 
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    for (uint8_t i=0; i<3; i++) 
    {   
      if (i==0)
      {
        if (digitalRead(A1) == HIGH)
        {
          rREF = 4000;
          rPT = 1000;
        }
        else
        {
          rREF = 400;
          rPT = 100;
        }
        temperature = max1.temperature(rPT, rREF);
        resistance = float(max1.readRTD()) / 32768.0 * float(rREF);
        status = max1.readFault();
      }
      else if (i==1)
      {
        if (digitalRead(A3) == HIGH)
        {
          rREF = 4000;
          rPT = 1000;
        }
        else
        {
          rREF = 400;
          rPT = 100;
        }
        temperature = max2.temperature(rPT, rREF);
        resistance = float(max2.readRTD()) / 32768.0 * float(rREF);
        status = max2.readFault();      
      }
      else if (i==2)
      {
        if (digitalRead(A5) == HIGH)
        {
          rREF = 4000;
          rPT = 1000;
        }
        else
        {
          rREF = 400;
          rPT = 100;
        }
        temperature = max3.temperature(rPT, rREF);
        resistance = float(max3.readRTD()) / 32768.0 * float(rREF);
        status = max3.readFault(); 
      }        

    display.print("CH");
    display.print(i+1);
    display.print(": ");
    if (status)
      display.println(" >> ERROR << ");
    else
    {
      display.print(temperature,1);
      display.print(char(247));
      display.print("C  ");
      display.print(resistance,0);
      display.println(char(233));
    }
    display.display();
    }
   delay(500);
  }
  //****END BUTTON_A****

  //BUTTON_B: toggle through loop interval settings in seconds
  if (! digitalRead(BUTTON_B)) 
  {
    switch (LoopIntervalSec)
    {
      case 1: LoopIntervalSec = 5;
        break;
      case 5: LoopIntervalSec = 10;
        break;
      case 10: LoopIntervalSec = 1;
        break;
    }
    display.clearDisplay();
    display.setCursor(10,10);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.print("Loop:");
    display.print(LoopIntervalSec);
    display.println(" s");
    display.display();
    while (! digitalRead(BUTTON_B))   //wait for button depress to prevent cycling
    {
      __asm__("nop\n\t");
    }
  }

  //****BUTTON_C: write resistance to SD card
  if (!digitalRead(BUTTON_C)) 
  {
    if (!SD.begin(4))   // check SD card
    {
      display.clearDisplay();
      display.setCursor(10,10);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.println("SD failed");
      display.display();
    }
    else
    {  
      String fileName = "T_log_";
      bool loopFile = true;
      unsigned long i = 1;
      while (loopFile)  // create new file T_log_XX.txt every time, maximum T_log_99.txt
      {
        if (!SD.exists(fileName+String(i)+".txt"))
        {
          loopFile = false;
        }
        else
        { 
          i += 1;
        }
        if (i==99)
        {
          loopFile = false;
        }
      }
      fileName += String(i) + ".txt";
      File myFile = SD.open(fileName, FILE_WRITE);
      //****loop SD write forever if SD card exists****
      unsigned int loopTimeInterval = LoopIntervalSec*1000;
      unsigned long loopTimeFirst = millis();
      bool loopForever = true;
      while (loopForever)
      {
        unsigned long loopTimeStart = millis();
        if (myFile) 
        {
          myFile.print(float((loopTimeStart-loopTimeFirst)/1000), 1);
          myFile.print(";");
        }
        display.clearDisplay();
        display.drawLine(41,0,41,32,WHITE);
        display.setCursor(43,0);
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.print("logging");
        display.setCursor(45,22);
        display.setTextSize(1);
        display.print(LoopIntervalSec);
        display.print(" seconds");
        display.display();
        display.setCursor(0,0);

        //read temperature values of all 3 MAX31865 cards
        for (uint8_t i=0; i<3; i++) 
        {   
          if (i==0)
          {
            if (digitalRead(A1) == HIGH)
            {
              rREF = 4000;
              rPT = 1000;
            }
            else
            {
              rREF = 400;
              rPT = 100;
            }
            temperature = max1.temperature(rPT, rREF);
            status = max1.readFault();
          }
          else if (i==1)
          {
            if (digitalRead(A3) == HIGH)
            {
              rREF = 4000;
              rPT = 1000;
            }
            else
            {
              rREF = 400;
              rPT = 100;
            }
            temperature = max2.temperature(rPT, rREF);
            status = max2.readFault();      
          }
          else if (i==2)
          {
            if (digitalRead(A5) == HIGH)
            {
              rREF = 4000;
              rPT = 1000;
            }
            else
            {
              rREF = 400;
              rPT = 100;
            }
            temperature = max3.temperature(rPT, rREF);
            status = max3.readFault(); 
          }        
          // print reasonable resistance to display     
          if ((temperature < 200 and temperature > -200) and not (status))
          {
            display.print(temperature, 1);
            display.print(char(247));
            display.println("C");          
          }
          else
          {
            display.print("CH");
            display.print(i+1);
            display.println("err");
          }
          display.display();
          // print temperature to SD file, format SEC;R1;R2;R3
          if (myFile) 
          {
            if (i<2)
            {
             myFile.print(temperature, 1);
             myFile.print(";");            
            }
            else
            {
              myFile.println(temperature, 1);
            }
          }
        }
        myFile.flush();  // always write completed lines
        // wait until loop interval time is reached, or a button is pressed
        while ((millis()-loopTimeStart < loopTimeInterval) and loopForever)
        {
          if ((!digitalRead(BUTTON_A)) or (!digitalRead(BUTTON_B)) or (!digitalRead(BUTTON_C)))
          {
            loopForever = false;
            myFile.close();
          }
        }
      }
    }
  }
  //****END BUTTON_C****
  
}




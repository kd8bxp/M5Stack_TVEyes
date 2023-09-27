//#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "math.h"
//#include "eyecenter.jpg.c"
#include "eyes.c"

//Thank you Emily Velasco https://github.com/emilyvelasco/TVeyeball for providing the idea and png files for this.

//https://javl.github.io/image2cpp/
//background color: black
//dithering: binary
//scaling: original size
//output: Arduino code (but have to change const uint16_t to const unsigned char, also add pgmspace.h and move PROGMEM)
//output: Horizontal - 2 byter per pixel (565)
//output: swap bits in byte: checked

#define M5STACKFIRE_MICROPHONE_PIN 34 //0
#define HORIZONTAL 216

M5UnitRCA gfx_rca;

extern const unsigned char eyecenter[];

extern const unsigned char eyeright1[];
extern const unsigned char eyeright2[];

extern const unsigned char eyeleft1[];
extern const unsigned char eyeleft2[];

extern const unsigned char eyeblink1[];
extern const unsigned char eyeblink2[];
extern const unsigned char eyeblink3[];
extern const unsigned char eyeblink4[];
extern const unsigned char eyeblink5[];



int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 3.3; //5.0;
 
// Integer for ADC value
int adc_value = 0;
byte eye_x, eye_y;
 
void draweyes(void*) {
    while (true) {
         eye_x = 15;
  eye_y = 15;
  
  
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyecenter);  
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyecenter); 
  delay(500);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeright1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeright1);
  delay(200);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeright2);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeright2);
  delay(500);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeright1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeright1);
  delay(200);
  gfx_rca.pushImage(eye_x, eye_y,88,88, eyecenter);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyecenter);
  delay(200);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeleft1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeleft1);
  delay(200);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeleft2);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeleft2);
  delay(500);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeleft1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeleft1);
  delay(200);

if (random(0,100) % 50 > 35) {
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeblink1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink1);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeblink2);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink2);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeblink3);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink3);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeblink4);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink4);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88, eyeblink5);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink5);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeblink4);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink4);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeblink3);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink3);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeblink2);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink2);
  delay(50);
  gfx_rca.pushImage(eye_x, eye_y, 88,88,eyeblink1);
  gfx_rca.pushImage(eye_x+100, eye_y, 88,88,eyeblink1);
  delay(50);
        }
    }
}

void setup() {
    Serial.begin(115200);
    //pinMode(M5STACKFIRE_MICROPHONE_PIN, INPUT_PULLUP);
    analogReadResolution(10);
    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
    //gfx_rca.setTextColor(TFT_WHITE, TFT_BLACK);
    //gfx_rca.drawCenterString("M5 RCA Module :)", 120, 50);
     xTaskCreatePinnedToCore(draweyes, "draweyes", 8192, NULL, 1, NULL, 1);
}


void loop() {
  
  
for (int i=0; i<HORIZONTAL; i++) {
       int /*float*/ readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
	   adc_voltage  = (readMicValue * ref_voltage) / 1024.0;
       in_voltage = (adc_voltage*(R1+R2)/R2); // * 2.5;
       Serial.print(readMicValue); //in_voltage);
       Serial.print(','); Serial.print(in_voltage); Serial.print(',');
       
       int temp = map(readMicValue, 0, 200, 0, 10); //works, display is inverted, works with (oldCapture[i+1]/2)+75 etc

       //add some random noise
       if (temp == 0) {capture[i] = random(0,2); } else {
       capture[i] = temp; }
       Serial.println(temp); 
   }
        
for (int i=1; i<HORIZONTAL-1; i++) {

  //this works but fabs is kind of pointless here
  //    gfx_rca.drawLine(i, (fabs((oldCapture[i])/1.65))+75, i, -(fabs((oldCapture[i+1])/1.65))+75, BLACK);
  //    gfx_rca.drawLine(i, (fabs((capture[i])/1.65))+75, i, -(fabs((capture[i+1])/1.65))+75, GREEN);

  //this also works, and I kind of like it
  //gfx_rca.drawLine(i, (((oldCapture[i])/1.65))+75, i, -(((oldCapture[i+1])/1.65))+75, BLACK);
  //gfx_rca.drawLine(i, (((capture[i])/1.65))+75, i, -(((capture[i+1])/1.65))+75, GREEN);

  
  //gfx_rca.drawLine(i, -(((oldCapture[i])))+125, i, -(((oldCapture[i+1])))+125, BLACK);
  //gfx_rca.drawLine(i, -(((capture[i])))+125, i, -(((capture[i+1])))+125, GREEN);
  //gfx_rca.drawLine(i, (((oldCapture[i])))+115, i, (((oldCapture[i+1])))+115, BLACK);
  //gfx_rca.drawLine(i, (((capture[i])))+75, i, (((capture[i+1])))+75, GREEN);

  gfx_rca.drawLine(i, oldCapture[i]+105, i, oldCapture[i+1]+105, BLACK);
  gfx_rca.drawLine(i, capture[i]+105, i, capture[i+1]+105, GREEN);
      oldCapture[i] = capture[i];
  }
 
}

//voltage sensor info
//https://lastminuteengineers.com/voltage-sensor-arduino-tutorial/
//https://www.electronicshub.org/interfacing-voltage-sensor-with-arduino/

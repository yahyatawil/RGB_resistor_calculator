/*
Yahya tawil
2013-2014
Licence:CC-BY-SA
*/

#include <Keypad.h>


/* TIPs
adjust values with steps until you have the right color 
http://www.instructables.com/id/RGB-LED-Color-Selector/?ALLSTEPS 

GUI res calc http://www.dannyg.com/examples/res2/resistor.htm

ASCII : http://en.wikipedia.org/wiki/ASCII

keypad : http://playground.arduino.cc/Code/Keypad

*/

#define red_led_pin  9 
#define green_led_pin 6 
#define blue_led_pin 5 

#define second_chip_en  3 //also used for common anode of 7seg third_digit 
#define first_chip_en  7 ////also used for common anode of 7seg second_digit
#define led_sel 2 ////also used for common anode of 7seg first_digit

#define ROW0 8
#define ROW1 10
#define ROW2 11
#define ROW3 12
#define COL0 A5
#define COL1 A4
#define COL2 A3

//address for 7seg IC

#define AD0 4
#define AD1 A2
#define AD2 A1
#define AD3 A0

/*
first band is connected to first LED channed x , second with channedl y and third is connected with second chip channel x 
*/

/*  
#define Black 0
#define Brown 1
#define Red 2
#define Orange 3
#define Yellow 4
#define Green 5
#define Blue 6
#define Violet 7
#define Grey 8
#define White 9
*/


int press ; 
char key ;
int digits [6] = {0,0,0,0,0,0}; // first (first number) , second (maybe ',' or '.' or just a number or '\n' or 'k') , third ('\n' or 'k' or 0)
int colorcode[3]; // store the real DEC values of band here in this array  
int val;//used for reading values
int i=0;//global counter , used for input
int numofdigits = 0 ;//counting of real numbers of input values excluding ',' or 'k' or '.'
boolean Do =false; // explained in loop , enable loop to enter number using serial 
boolean Do2 =false; // enable the loop for entering number by keypad , make it false to disaple keypad option 

byte seven_seg_digits[10][4] =                             {//a,b,c,d,e,f,g
                                                        { 0,0,0,0 },  // = 0
                                                           { 0,0,0,1 },  // = 1
                                                           { 0,0,1,0 },  // = 2
                                                           { 0,0,1,1 },  // = 3
                                                           { 0,1,0,0 },  // = 4
                                                           { 0,1,0,1 },  // = 5
                                                           { 0,1,1,0},  // = 6
                                                           { 0,1,1,1 },  // = 7
                                                           { 1,0,0,0 },  // = 8
                                                           { 1,0,0,1 }   // = 9
                                                           };
                                                           
                                                          
                              


const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {',','0','k'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { ROW0, ROW1, ROW2, ROW3 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { COL0, COL1, COL2 }; 

Keypad KPD = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
void setup()
{
 Serial.begin(9600);
 pinMode(AD0,OUTPUT);
 pinMode(AD1,OUTPUT);
 pinMode(AD2,OUTPUT);
 pinMode(AD3,OUTPUT);
 pinMode(red_led_pin,OUTPUT);
 pinMode(green_led_pin,OUTPUT);
 pinMode(blue_led_pin,OUTPUT);
 pinMode(second_chip_en,OUTPUT);
 pinMode(first_chip_en,OUTPUT);
 pinMode(led_sel,OUTPUT);
 digitalWrite(second_chip_en,LOW);
 digitalWrite(first_chip_en,LOW);
 digitalWrite(led_sel,LOW);
 digitalWrite(second_chip_en,HIGH);
 digitalWrite(first_chip_en,HIGH);
 
 
  KPD.setDebounceTime(50);
}

void seven_seg_disp(int num){
 
   digitalWrite(AD0,seven_seg_digits[num][3]);
   digitalWrite(AD1,seven_seg_digits[num][2]);
   digitalWrite(AD2,seven_seg_digits[num][1]);
   digitalWrite(AD3,seven_seg_digits[num][0]);
 }
  


void rgbled(int r,int g,int b) //255-value because I used common anode RGB leds
{
     analogWrite(red_led_pin, 255-r);
     analogWrite(green_led_pin, 255-g);
     analogWrite(blue_led_pin, 255-b);
}


void colortorgb(int color){
  //token after tests 
  switch (color) {
    case 0 :
    rgbled(0,0,0);
    break ;
    case 1 :
    rgbled(180,15,0);
    break ;
    case 2 :
    rgbled(255,0,0);
    break ;
    case 3 :
    rgbled(255,31,0);
    break ;
    case 4 :
    rgbled(145,24,0);
    break ;
    case 5 :
    rgbled(0,255,0);
    break ;
    case 6 :
    rgbled(0,0,255);
    break ;
    case 7 :
    rgbled(204,0,53);
    break ;
    case 8 :
    rgbled(204,100,80);
    break ;
    case 9 :    
    rgbled(240,190,100);
    break ;  
 } 
}

void rgbdisp()
{
  /*
  the idea is we turn off second chip and sel the channels of first chip for 1st and 2nd band
  then trurn off first chip and sel channel for third band
  */
  
  //RGB section
 digitalWrite(second_chip_en,HIGH); 
 digitalWrite(first_chip_en,LOW);
 digitalWrite(led_sel,LOW);
 colortorgb(colorcode[0])  ;
 
 delay(1);
 
 digitalWrite(second_chip_en,HIGH);
 digitalWrite(first_chip_en,LOW); 
 digitalWrite(led_sel,HIGH);
 colortorgb(colorcode[1])  ;
 
 delay(1);
 
 digitalWrite(second_chip_en,LOW);
 digitalWrite(first_chip_en,HIGH);
 digitalWrite(led_sel,LOW);
 colortorgb(colorcode[2])  ;

 delay(1);

 /*
 //7seg section
 digitalWrite(second_chip_en,HIGH); 
 digitalWrite(first_chip_en,LOW);
 digitalWrite(led_sel,LOW);
 rgbled(0,0,0);
 seven_seg_disp(colorcode[2]) ;
 
 delay(1);
  digitalWrite(second_chip_en,LOW);
 digitalWrite(first_chip_en,HIGH);
 digitalWrite(led_sel,LOW);
 //rgbled(0,0,0);
 seven_seg_disp(colorcode[1]) ;
 
 delay(1); 
  digitalWrite(second_chip_en,LOW);
 digitalWrite(first_chip_en,LOW);
 digitalWrite(led_sel,HIGH);
 //rgbled(0,0,0);
 seven_seg_disp(colorcode[0]) ;
 
delay(1); 

 */
 
}

int printbandcolor(int band)
{
 switch (band) {
   case 0 :
    Serial.print("Black "); 
    return 0;
   case 1 :
    Serial.print("Brown ");
    return 1;
   case 2 :
    Serial.print("Red ");
    return 2;
   case 3 :
     Serial.print("Orange ");
    return 3;
   case 4 :
     Serial.print("Yellow ");   
    return 4;
   case 5 :
     Serial.print("Green ");   
    return 5;
   case 6 :
     Serial.print("Blue ");   
    return 6;
   case 7 :
     Serial.print("Violet ");   
    return 7;
   case 8 :
     Serial.print("Gray ");   
    return 8;
   case 9 :   
     Serial.print("white ");   
    return 9;

 }  
}
void value(){
  
  //ex:1,5k or 1.5k
 if(numofdigits==2 && (digits[1]==('.'-'0') || digits[1]==(','-'0')) &&  digits[3]==('k'-'0') )
 {
   digits[1]=digits[2];
   digits[2]=2;
  for(int j=0;j<3;j++) {colorcode[j]=printbandcolor(digits[j]) ;}   
 }
 
 //ex:1ohm 
  else if(numofdigits==1 && digits[1]!=('k'-'0')  )
 {  
  digits[1] = digits[0] ;digits[0] = 0 ;digits[2]=0;
  for(int j=0;j<3;j++) {colorcode[j]=printbandcolor(digits[j]);}
 }
 
 //ex:27ohm
 else if(numofdigits==2 && digits[2]==0)
 {
   digits[2]=0;
  for(int j=0;j<3;j++) { colorcode[j]=printbandcolor(digits[j]);}
 }
 
 
 //ex:470ohm
 else if(numofdigits==3 && digits[3]==0)
 {
   if(digits[2]!=0) Serial.println("error1") ; //must be zero 3rd brand don't have values
   else{
   digits[2]=1;
  for(int j=0;j<3;j++) {colorcode[j]=printbandcolor(digits[j]);}
 }
 }
 
 
 //ex:1k
 else if(numofdigits==1 && digits[1]==('k'-'0') ) 
 {
   digits[1]=0 ; digits[2]=2;
  for(int j=0;j<3;j++) {colorcode[j]=printbandcolor(digits[j]); }
 }
 
 //ex:15k
 else if(numofdigits==2 && digits[2]==('k'-'0'))
 {
   digits[2]=3;
  for(int j=0;j<3;j++){ colorcode[j]=printbandcolor(digits[j]);  }
 }
 
  //ex:100k
 else if(numofdigits==3 && digits[3]==('k'-'0')  )
 {
   if(digits[2]!=0) Serial.println("error2") ;
   else{
   digits[2]= 4;
  for(int j=0;j<3;j++) {colorcode[j]=printbandcolor(digits[j]); }
 }
 }

}

void serialEvent() 
{
   while(Serial.available()) {
   val=Serial.read() -'0';
   if(val==('\n'-'0')) Do=true ; 
   else{digits[i]=val; 
   if(val!=('.'-'0') && val!=(','-'0') && val!=('k'-'0')) numofdigits++;  
   i++ ;
   }
 }
}

void start()
{
Serial.println(" \n ---------- ");
numofdigits = 0;
i=0;
press=0;
digits[5]=digits[4]=digits[3]= digits[2] = digits[1]= digits[0]=0;

}

    
    

void loop()
{
  // do is for making show color execute just one time when the resistor value is revived (\n recived)
  while(Do==true) {value() ; Do=false; start();}
     
     if(KPD.getKey()==',') {Serial.println("keypad unlocked");Do2=true; };
     
     for (int j=0 ; j<6 ; j++){
       if(Do2 == false) break ;
          key = KPD.waitForKey() ;
         
       digits[j]=key - '0' ; 
        if(key!='k' && key!=',') numofdigits++; 
      
      if(digits[j]==(','-'0') && digits[j-1]==(','-'0')){
        digits[j]=digits[j-1]=0;
     Serial.println("value is entered");
     Do2=false ;
     Do=true;
     break;
    }
    
    if(key == ',')  Serial.print(',');
        else if(key=='k')  Serial.print('k'); 
            else  Serial.print(key)  ;
    
        }
        
  rgbdisp();

}



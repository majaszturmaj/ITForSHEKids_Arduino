//**************************************************************//
//  Name    : SevenSegmentLEDdisplays101.ino                                
//  Author  : Mel Lester Jr. 
//  Date    : 2017-02-11
//  Modified:                                  
//  Version : 0.2                                             
//  Notes   : Sketch to demonstrate how 7 Segment LED Displays works. 
//          : This is intended as a Tutorial, not a production quality
//          : example.  No attempt has been made to optomize the code.                          
//****************************************************************

// Globals
/* Arduino Uno/Nano I/O pin assignments, You may change these
 *  but the program logic expects these pins to be in an unbroken 
 *  numerical sequence if the eight segments are to light correctly */
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;
const int segP = 9;  // Decimal point or Dp

// uncomment one of the following lines that describes your display
const char common = 'a';    // common anode
//const char common = 'c';    // common cathode

bool decPt = true;  // decimal point display flag

void setup() {
  // initialize I/O pins used in this sketch
  for (int i = segA; i <= segP; i++) {
    pinMode(i, OUTPUT);
    if (common == 'c') {  // test light every segment
      digitalWrite(i, HIGH);    // display is common cathode 
    } else {
      digitalWrite(i, LOW);     // common anode display
    }  
    delay(500);
  }
}

void loop() {
  decPt = !decPt; // display decimal point every other trip through loop
  
  // generate characters to display for hexidecimal numbers 0 to F
  for (int i = 0; i <= 15; i++) {
    byte bits = myfnNumToBits(i) ;
    if (decPt) {
      bits = bits | B00000001;
    }
    myfnUpdateDisplay(bits);
    delay(750);
  }
}

void myfnUpdateDisplay(byte eightBits) {
  int thisBit = 7;  // eight bits in a byte counting from 0
  for (int i = segA; i <= segP; i++) {
    byte lightSegment = bitRead(eightBits, thisBit--);
    if (common == 'a') {
      digitalWrite(i, !lightSegment); 
    } else {
      digitalWrite(i, lightSegment);   
    }
  }
}

byte myfnNumToBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B11111100;
      break;
    case 1:
      return B01100000;
      break;
    case 2:
      return B11011010;
      break;
    case 3:
      return B11110010;
      break;
    case 4:
      return B01100110;
      break;
    case 5:
      return B10110110;
      break;
    case 6:
      return B10111110;
      break;
    case 7:
      return B11100000;
      break;
    case 8:
      return B11111110;
      break;
    case 9:
      return B11110110;
      break;
    case 10:
      return B11101110; // Hexidecimal A
      break;
    case 11:
      return B00111110; // Hexidecimal B
      break;
    case 12:
      return B10011100; // Hexidecimal C or use for Centigrade
      break;
    case 13:
      return B01111010; // Hexidecimal D
      break;
    case 14:
      return B10011110; // Hexidecimal E
      break;
    case 15:
      return B10001110; // Hexidecimal F or use for Fahrenheit
      break;  
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;   
  }
}
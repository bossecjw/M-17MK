/*
Morse program for Marker M-17MK
 */

// 2016-10-09

// Pin used as output for morse signal
int morsepin = 0;
// Pin used as input for address coding
int adrpin1 = 1;
int adrpin2 = 2;
int adrpin3 = 3;
int adrpin4 = 4;
// Pin used as output for CW keyed lf 2000 Hz signal
int lfpin = 5;

/*
Delays used for morse code
A "short" beep (.) is 1 period, a "long" beep (-) is 3 periods.
The time between two fragments of a sign is 1 period.
The time between two signs is 3 periods.
The time between two words is 7 periods.
*/
int delayBase  = 15;  // In ms - 7.5ms for speed 16 WPM
int delayShort = 10*delayBase;
int delayLong  = 30*delayBase;
int delaySpace = 70*delayBase;

// The setup function runs once when you press reset or power the board
void setup() {
  
// Initialize morse digital pin as an output.
  pinMode(morsepin, OUTPUT);
  // Initialize lf digital pin as an output.
  pinMode(lfpin, OUTPUT);
    // Set up four pins as digital inputs. These will be used as address pins
  pinMode(adrpin1, INPUT);
   //configure pin6 as an input and enable the internal pull-up resistor
  pinMode(adrpin1, INPUT_PULLUP);
  pinMode(adrpin2, INPUT);
   //configure pin7 as an input and enable the internal pull-up resistor
  pinMode(adrpin2, INPUT_PULLUP);
  pinMode(adrpin3, INPUT);
   //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(adrpin3, INPUT_PULLUP);
  pinMode(adrpin4, INPUT);
   //configure pin3 as an input and enable the internal pull-up resistor
  pinMode(adrpin4, INPUT_PULLUP);
}


/*
Functions for making telegraphy signs
*/
void ShortBeep() {
  digitalWrite(morsepin, HIGH);   // turn the LED on (HIGH is the voltage level). Start of the TX oscillator  
  analogWrite(lfpin, 128);
  delay(delayShort);              // wait for a short period
  digitalWrite(morsepin, LOW);    // turn the LED off by making the voltage LOW. Stop of the TX oscillator
  delay(10*delayBase);               // Wait for antenna voltage to decay 
  analogWrite(lfpin, 0);
}
void LongBeep() {
  digitalWrite(morsepin, HIGH);   // turn the LED on (HIGH is the voltage level). Start of the TX oscillator
  analogWrite(lfpin, 128);
  delay(delayLong);              // wait for a long period
  digitalWrite(morsepin, LOW);    // turn the LED off by making the voltage LOW. Stop of the TX oscillator
  delay(10*delayBase);               // Wait for antenna voltage to decay
  analogWrite(lfpin, 0);
}
void InternalBreak() {
  delay(delayShort);              // wait for a short period
}
void SignBreak() {
  delay(delayLong);              // wait for a long period
}
void SpaceBreak() {
  delay(delaySpace);              // wait for a long period
}



/*
Functionality for converting text to morse code.
*/
const int NUMSIGNS = 45;
char morseText[] = "abcdefghijklmnopqrstuvwxyz0123456789.,?-=/+@!";
char *morseSign[NUMSIGNS] = {".-","-...","-.-.","-..",".","..-.","--.",
				  "....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----.",".-.-.-","--..--","..--..","-....-", "-...-","-..-.",".-.-.","...---...","..--."};

// Function that takes a letter/number (sign) and returns a string of characters representing that 
// letter/number in morse code: '.' for short, '-' for long or ' ' (space) if sign isn't recognized
char* texttomorse(char sign)
{
  int i = 0;
  // Iterate over the known letters/numbers and return the correct morse representation once found
  while(i < NUMSIGNS) {
  	if(morseText[i] == sign) return morseSign[i];
  	i++;
  }
  // As a fallback, return ' '
  return " ";
}

// Function that plays one morse sign on the output pin
// Takes the output of texttomorse as input
void PrintSign(char *array) {
  for(; *array != '\0'; array++) { // Loop until we reach the end of the sign string
  	if(*array == '.') //Short
  	  ShortBeep();
  	else if(*array == '-') //Long
  	  LongBeep();
  	else //Default/space
  	  SpaceBreak();
    char next = *(array+1);
    if(next != '\0')
      InternalBreak();
  }
}

// Functions that converts a string of real text into morse and plays the morse representation on the output pin
void PrintString(char *array) {
  for(; *array != '\0'; array++) { // Loop until we reach the end of the sentence
    if(*array == ' ') {
      SpaceBreak();
    }
    else {
      PrintSign( texttomorse(*array) );
      char next = *(array+1);
      if(next != ' ' && next != '\0')
        SignBreak();
    }
  }
}

// Function that reads the address input pins and combines them into an aggregate number that is returned, 0-15
int GetInputs() {
  int input = 0;
  if(!digitalRead(adrpin1)) input |= 1;
  if(!digitalRead(adrpin2)) input |= 2;
  if(!digitalRead(adrpin3)) input |= 4;
  if(!digitalRead(adrpin4)) input |= 8; 
  return input;
}

// the loop function runs over and over again forever
void loop() {
  // First, get the input address
  int input = GetInputs();

  //input = 0; //TODO REMOVE
  
  // Depending on what the input is, perform different functions
  switch(input) {
    
	case 0:
	  PrintString("ps ");
	  	  break; //Always end with break;
	
  case 1:
    PrintString("dg ");
        break;

  case 2:
    PrintString("aj ");
        break;

  case 3:
    PrintString("sg ");
        break;

  case 4:
    PrintString("kl ");
        break;  

  case 5:          
    PrintString("pj ");
        break;

  case 6:
    PrintString("os ");
        break;
        
  case 7:
    PrintString("js ");
        break;

  case 8:
    PrintString("mt ");
        break;
  
  case 9:
    PrintString("rs ");
        break;

  case 10:
    PrintString("aa ");
        break;

  case 11:
    PrintString("dt ");
        break;

  case 12:
    PrintString("sp ");
        break;  

  case 13:          
    PrintString("oeh ");
        break;

  case 14:
    PrintString("sb ");
        break;
        
  case 15:
    PrintString("am ");
        break;
        
	default: // Default case
	  PrintString("not implemented ");
	break;
  }
}

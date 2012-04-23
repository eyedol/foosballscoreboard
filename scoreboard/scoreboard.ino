/*
 *Foosball scoreboard. 
 *Tracks the scores of a foosball game.
 *
 *- Uses input from push button to register score
 *- Pushes score to an webserver
 *- Print's score to a 7 segment LED display 
 *
 */
 
#include <SPI.h>
#include <Ethernet.h>


//Number of LED's segments
const int ledCount = 7;
int pushButtonA = 2;
int pushButtonB = 3;
int pinA = 13;
int pinB = 12;

int red_score = 0;
int blue_score = 0;

int buttonStateA = 0;
int buttonStateB = 0;

//Digitial output pins for LED display
//One Display
int ledPins[] = {
  3,4,5,6,7,8,9
};


byte seven_seg_digits[10][7] = { 
  { 
    1,1,1,1,1,1,0   }
  ,  // = 0
  { 
    0,1,1,0,0,0,0   }
  ,  // = 1
  { 
    1,1,0,1,1,0,1   }
  ,  // = 2
  { 
    1,1,1,1,0,0,1   }
  ,  // = 3
  { 
    0,1,1,0,0,1,1   }
  ,  // = 4
  { 
    1,0,1,1,0,1,1   }
  ,  // = 5
  { 
    1,0,1,1,1,1,1   }
  ,  // = 6
  { 
    1,1,1,0,0,0,0   }
  ,  // = 7
  { 
    1,1,1,1,1,1,1   }
  ,  // = 8
  { 
    1,1,1,0,0,1,1   }
  ,  // = 9
};


byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x28 };
IPAddress ip(192,168,137,10);
IPAddress server (192,168,137,1);
EthernetClient client;


// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int lastButtonStateA = LOW;   // the previous reading from the input pin
int lastButtonStateB = LOW;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:

  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButtonA, INPUT);
  pinMode(pushButtonB, INPUT);
  Ethernet.begin(mac,ip);
  displayScore(0, 3);
}

boolean pressedA = 1;
boolean pressedB = 1;
// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:

  int readingA = digitalRead(pushButtonA);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (readingA != lastButtonStateA) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonStateA = readingA;
  }

  // set the LED using the state of the button:

  if(!buttonStateA){
    if(!pressedA){   
      Serial.println("Team A");
      pressedA = HIGH; 

      red_score ++;
      post_score_online(pushButtonA);
      displayScore(red_score, 3,);
    }
  }
  else{
    pressedA = LOW;
  }
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonStateA = readingA;

  int readingB =0;
  // digitalRead(pushButtonB);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (readingB != lastButtonStateB) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonStateB = readingB;
  }

  // set the LED using the state of the button:

  if(!buttonStateB){
    if(!pressedB){   
      Serial.println("Team B");

      blue_score ++;
      post_score_online(pushButtonB);
      pressedB = HIGH; 
    }
  }
  else{
    pressedB = LOW;
  }
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonStateB = readingB;
}


/*
Post the scores online?
 */
void post_score_online(int teamID){

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    if(teamID==pushButtonA){
      char *scorestr = "      ";
      char *hdrstr = "GET /webserver/?team=r&score=";
      itoa(blue_score, scorestr, 10);  
      //hdrstr.concat(scorestr);
      //client.println(scorestr);
      client.print("GET /webserver/?team=r&score=");
      client.println(scorestr);
      Serial.println(blue_score);
    }
    else{
      //String score = blue_score; 

      char *scorestr = "      ";
      char *hdrstr = "GET /webserver/?team=r&score=";
      itoa(blue_score, scorestr, 10);  
      //hdrstr.concat(scorestr);
      //client.println(scorestr);
      client.print("GET /webserver/?team=r&score=");
      client.println(scorestr);
      Serial.println(red_score);     
    }
    client.println();
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }  

  client.stop();
}

//Prints out the score to the LED display screen
void displayScore(byte digit, int startPin) {
  //Serial.println(digit);
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(startPin, seven_seg_digits[digit][segCount]);
    //Serial.println(seven_seg_digits[digit][segCount]);
    ++startPin;
  }
}


#include <Servo.h>
#include <IRremote.hpp> 

void flash_led(int pin){
  //flash led indefinitely
  while (true){
    delay(50);
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
  }
}
// declare servos
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

Servo myservo5;

//declare IR stuff
const int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

// declare some globals
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 2000;  //the value is a number of milliseconds

int count;
// char chords[] = {'G', 'D', 'C', 'D', 'G', 'D', 'C', 'D'};
int * Time;
// Riptide
// int Strum[] = {450, 450, 150,150,150,450,450,150,150,150};
// int Pos[] = {70,120,70,120,70,120,70,120,70,120,70};

// Feels Like we only go backwards
// int Strum[] = {500, 500, 500, 500,500,500, 500,500, 500,500,500,500, 250,250,250,250};
// int Pos[] = {70,120,70,120,70,120,70,120,70,120,70,120,70,120,70,120};

// Im yours
int Strum[] = {400, 400,400, 400,400, 400,400, 400,400, 400,400, 400,400, 400,400, 400};
int Pos[] = {70,120,70,120,70,120,70,120,70,120,70,120,70,120,70,120};
unsigned long startStrum;
int s_count;

int pos = 0;
char * chords;
bool chosen = false; //waiting for user to choose the song
bool stuff = false;

int count_reset = 0;

char * c1 = "EDGEDGEDGEDG";

int t1[] = {1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516};

char * c2 = "CGCGCGEGCGCGCGEDGCGCGCGEGCGCGCGEDCGCGCGEDCGCGCGEGCGCGCGEGCGCGCGEDGCGCGCGEGCGCGCGCGEDGCGCGCGEGCGCGEDCGCGCGEGCGCGCGEDGCGCGCGEGCGCGCGEDCGCGCGEGCG";

int t2[] = {759729 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935 ,935};

char * c3 = "EDGCDEDGCD";

int t3[] = {1302 ,1302 ,1302 ,1302 ,1302 ,1302 ,1302 ,1302};

void playChord(char chord, int on) {

  if (chord == 'C') {
    if(on){
      myservo1.write(180);
    }
    else{
      myservo1.write(-180);
    }

  }
  else if (chord == 'G') {
    if(on){
      myservo2.write(180);
    }
    else{
      myservo2.write(-180);
    }
      // attach servo motor for note G          // play note for 500ms

    // release servo motor for note G
  }
  else if (chord == 'E') {
    if(on){
      myservo3.write(180);
    }
    else{
      myservo3.write(-180);
    } // attach servo motor for note E
              // play note for 500ms

   // release servo motor for note E
  }
  else if (chord == 'D') {
    if(on){
      myservo4.write(180);
    }
    else{
      myservo4.write(-180);
    }  // attach servo motor for note A          // play note for 500ms
   // release servo motor for note A
  }
}

// bool chosen = false; //waiting for user to choose the song
// bool stuff = false;

void setup() {
  Serial.begin(9600);  //start Serial in case we need to print debugging info
  // put some values on our stuff
  pos = 180;
  count = 0 ;
  s_count = 0;
  count_reset = 0;
  // playChord(chords[count], 1);
  // myservo5.write(120);
  startMillis = millis();  //initial start time
  startStrum = millis();

  pinMode(13, OUTPUT);        //init internal led
  digitalWrite(13, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);       //init external led

  pinMode(8, INPUT);          // init kill switch
  
  // init all servos
  myservo1.attach(9);
  myservo1.write(0);
  myservo2.attach(10);
  myservo2.write(0);
  myservo3.attach(11);
  myservo3.write(0);
  myservo4.attach(12);
  myservo4.write(0);
  myservo5.attach(7);
  myservo5.write(0);
  delay(1000);

  //init ir reciever
  irrecv.enableIRIn();
  irrecv.blink13(true);

  //wait for chords
  // int status = wait_for_chords();

  // if (status != 1){
  //   flash_led(2);
  // }
  // resume ir recieving
  // irrecv.resume();
  choose_song();
}

void loop() {
  // set status flag
  int status = 1;

  // check if kill switch has been pressed
  int buttonState = digitalRead(8);
  if(buttonState == HIGH){
    flash_led(13);
  }

  // check remote to see if process should be killed
  if (irrecv.decode()){
    int data = irrecv.decodedIRData.command;
    if(data == '0xE'){
      flash_led(13);
    }
  }

  digitalWrite(13, LOW);

  //play chords
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if(currentMillis - startMillis >= Time[count]){
    playChord(chords[count] , 0);
    count ++;
    playChord(chords[count] , 1);
    startMillis = currentMillis;
  }
  if(currentMillis - startStrum >= Strum[s_count]){
    myservo5.write(Pos[s_count]);
    s_count ++;
    startStrum = currentMillis;
  }
  if(s_count == 16){
      s_count = 0;
  }
  if (count == count_reset){
    count = 0;
  }

}

void choose_song(){
  chosen = false;
  while(!chosen){
    if (irrecv.decode()){
        int data = irrecv.decodedIRData.command;
        data = parse_IR_data(data);
        if(data == 0){
          chords = c1;
          Time = t1;
          count_reset = 9;
        }
        if(data == 1){
          chords = c2;
          Time = t2;
          count_reset = 142;
        }
        if(data == 2){
          chords = c3;
          Time = t3;
          count_reset = 8;
        }
        // Serial.write(data);
        chosen = true;
    }
  }
}

int parse_IR_data(int number){
    if (number == 0x16){
      return 0;
    }
    else if (number == 0x0c){
      return 1;
    }
    else if(number == 0x18){
      return 2;
    }
    else if (number  == 0x5e){
        return 3;
    }
    else if (number  == 0x08){
        return 4;
    }
    else if (number  == 0x1c){
        return 5;
    }
    else if (number  == 0x5A){
        return 6;
    }
    else if (number  == 0x42){
        return 7;
    }
    else if (number  == 0x52){
        return 8;
        }
    else if (number  == 0x4A){
        return 9;
      }
    else if (number  == 0x0E){
        return -1;
    }
    else{
        return -2;
    }
  }

int wait_for_chords(){
       //becomes true when stuff is returned from the python script
  int k = 0;          //counting how many chords there are
  int time_idx = 0; // to store when the time is being read
  bool delim_passed = false; //store if we passed the '$' or not
  char read;
  
  //wait for user to choose a song
  while(!chosen){
    if (irrecv.decode()){
      int data = irrecv.decodedIRData.command;

        if(data == '0xE'){
          return 0;
        }
        
        Serial.write(data);
        chosen = true;
    }
      // if something bad happens, flash led indefinitely
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
  }

  // wait for computer to send song data
  while(!stuff){
    // loop until we read something
        
    if (Serial.available() > 0){
      read = Serial.read();
      if(read == '\n' && !delim_passed){
        // Serial.write("Done chords");
        myservo1.write(80);
        delay(150);
        myservo1.write(30);
        delim_passed = true;
        time_idx = k;
      }
      else if(read == '\n' && delim_passed){
        stuff = true;
        break;
      }
      if(!delim_passed){
        chords[k] = read;
      }
      else{
        if(read == ',' || read == ']' || read == '[')
        Time[k - time_idx] = read;
      }
      k++;
    }
    
  }
  //signal that some data has been recieved
  digitalWrite(13, HIGH);
  startMillis = millis();  //initial start time
  startStrum = millis();

  return 1;
}

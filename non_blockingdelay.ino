#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

Servo myservo5;

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


unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 2000;  //the value is a number of milliseconds
   //using the built in LED
int pos;
int count;
char Chords[] = {'G', 'D', 'C', 'D', 'G', 'D', 'C', 'D'};
int Time[]= {1516, 1516, 1516, 1516, 1516, 1516, 1516, 1516};
int Strum[] = {450, 450, 200,200,200,450,450,200,200,200};
int Pos[] = {70,120,70,120,70,120,70,120,70,120,70};
unsigned long startStrum;
int s_count;

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(11);
  myservo4.attach(12);
  myservo5.attach(13);
  
  pos = 180;
  count = 0 ;
  s_count = 0;
  playChord(Chords[count], 1);
  myservo5.write(120);
  startMillis = millis();  //initial start time
  startStrum = millis();
  pinMode(8, INPUT);

}

void loop()
{
  int buttonState = digitalRead(8);
  if(buttonState == HIGH){
    while (true){
      }
    }

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if(currentMillis - startMillis >= Time[count]){
      playChord(Chords[count] , 0);
      count ++;
      Serial.println(count);
      playChord(Chords[count] , 1);
      startMillis = currentMillis;
   }
   if(currentMillis - startStrum >= Strum[s_count]){
      myservo5.write(Pos[s_count]);
      s_count ++;
      Serial.println(s_count);
      startStrum = currentMillis;
    }
    if(s_count == 10){
        s_count = 0;
      }
}

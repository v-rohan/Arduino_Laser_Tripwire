// 5 pins trip wire

// Variables
int mode = 1;
int ambiant;
int trip = 1000; // The light value I get when using my laser
int minLight = 900;
int makeBeep = 1; //0 for no beep, 1 for beep!
int atAverage;
long millisCount;

// Output Pins
int laserPin = 12;
int ledPin = 13;
int buzzerPin = 3;
String  modeNames[3] = {"SETTINGS","ARMED","TRIP"};

// Input Pins
int modePin = 4; 
int tripPin = A0;
int ambiantPin = A1;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(modePin, INPUT);
  Serial.begin(9600);
}


void loop() {

  // When the button is pushed
  if (digitalRead(modePin) == 1) {
    trip = analogRead(tripPin);
    mode=mode + 1;
    if (mode > 2) {
      mode = 0;
    }
    if(mode == 2) {
      mode = 1;
    }
    beep(1);
    delay(300);
  }

  
  //does something when the mode changes
  switch (mode) {
    case 0: //calibration mode
      digitalWrite(laserPin,HIGH);
      
      ambiant = analogRead(ambiantPin);
      trip = analogRead(tripPin);
      atAverage = ambiant + ((trip - ambiant)/2);
      stats();
      
      if (trip  >= minLight) {
        digitalWrite(ledPin,HIGH);
      } else {
        digitalWrite(ledPin,LOW);
      }

    break;

    case 1: // Armed mode
      digitalWrite(laserPin,HIGH);
      digitalWrite(ledPin,LOW);
      ambiant = analogRead(ambiantPin);
      atAverage = ambiant + ((trip - ambiant)/2);
      if (analogRead(tripPin) < atAverage) {
        mode = 2;
      }
      if ((millis() - millisCount) >= 3000) {
        millisCount = millis();
        stats();
      }
    break;
    
    case 2: //Trip Mode
      if ((millis() - millisCount) >= 1000) {
        millisCount = millis();
        stats();
        beep(3);
      }
    break;
    }

    
  delay(1);                       // wait for a bit
}

//It Beeps
void beep(int qty) {
  int count;
  if (makeBeep == 1) {
    for (count = 1;count<=qty;count++) {
      digitalWrite(buzzerPin, HIGH);
      delay(50);
      digitalWrite(buzzerPin, LOW);
      delay(50);
    }
  }
}

//Writes stats to the Serial Port
void stats() {
  Serial.print("A:");
  Serial.print(ambiant);
  Serial.print(" T:");
  Serial.print(trip);
  Serial.print(" AT:");
  Serial.print(atAverage);
  Serial.print(" MODE:");
  Serial.print(modeNames[mode]);
  Serial.println("");
}

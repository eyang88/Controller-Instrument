#include <LiquidCrystal.h>
#include "pitches.h"
#define melodyPin 7
#define L_button 6
#define D_button 8
#define A_button 9
#define E_button 10


int starting_millis, current_millis;
int note, frequency;
const int X_pin = 0;
const int Y_pin = 1;
int x_vector, y_vector; 
double x_prime, y_prime;
double theta_rad, theta_deg, theta;
double magnitude; 
int wholeNote = 4000;
//Notes and durations of the song
int melody[] = {
  D4, D4, A4, A4, B4, B4, A4, G4, G4, FS4, FS4, E4, E4, D4, A4, A4, G4, G4, FS4, FS4, E4, A4, A4, G4, G4, FS4, FS4, E4, D4, D4, A4, A4, B4, B4, A4, G4, G4, FS4, FS4, E4, E4, D4
};
int melodyDurations[] = {
  4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2
};

char button[sizeof(melody)/2];
int absolute[sizeof(melody)/2];
int arrow[sizeof(melody)/2];
int count[sizeof(melody)/2];
int millis_timer;
int j = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte down[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte downleft[] = {
  B00000,
  B00001,
  B10010,
  B10100,
  B11000,
  B11110,
  B00000,
  B00000
};
byte upleft[] = {
  B00000,
  B11110,
  B11000,
  B10100,
  B10010,
  B00001,
  B00000,
  B00000
};
byte up[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte upright[] = {
  B00000,
  B01111,
  B00011,
  B00101,
  B01001,
  B10000,
  B00000,
  B00000
};
byte downright[] = {
  B00000,
  B10000,
  B01001,
  B00101,
  B00011,
  B01111,
  B00000,
  B00000
};

void setup() {
  // put your setup code here, to run once:
    lcd.createChar(0, up);
    lcd.createChar(1, upright);
    lcd.createChar(3, downright);
    lcd.createChar(4, down);
    lcd.createChar(5, downleft);
    lcd.createChar(6, upleft);

  pinMode(melodyPin, OUTPUT);
pinMode(L_button, INPUT_PULLUP);
pinMode(D_button, INPUT_PULLUP);
pinMode(A_button, INPUT_PULLUP);
pinMode(E_button, INPUT_PULLUP);
    
    Serial.begin(9600);
  count[-1] = 16;

  for (int i = 0; i < sizeof(melody)/2; i++) 
  {
    if (melody[i] >= D4 && melody[i] < A4) {
      button[i] = 'D';
      absolute[i] = D4;
    }
    else if (melody[i] >= A4 && melody[i] < E5 || melody[i] > B5 && melody[i] < E6) {
      button[i] = 'A';
      absolute[i] = A4;
    }
    else if (melody[i] >= E5 && melody[i] <= B5 || melody[i] >= E6 && melody[i] <= B6) {
      button[i] = 'E';
      absolute[i] = E5;
    }

    if (melody[i] == absolute[i]) {
      arrow[i] = 0;
    }
    else if (melody[i] > absolute[i] && melody[i] < absolute[i]* 1.1) {
      arrow[i] = 6;
    }
    else if (melody[i] > absolute[i]*1.1 && melody[i] < absolute[i]*1.15) {
      arrow[i] = 127;
    }
    else if (melody[i] > absolute[i]*1.15 && melody[i] < absolute[i]*1.2) {
      arrow[i] = 5;
    }
    else if (melody[i] > absolute[i]*1.2 && melody[i] < absolute[i]*1.3) {
      arrow[i] = 4;
    }
    else if (melody[i] > absolute[i]*1.3 && melody[i] < absolute[i]*1.37) {
      arrow[i] = 3;
    }
    else if (melody[i] > absolute[i]*1.37 && melody[i] < absolute[i]*1.45) {
      arrow[i] = 126;
    }
    else if (melody[i] == absolute[i]*1.5) {
      arrow[i] = 1;
    }

    count[i] = count[i-1] + 16/melodyDurations[i-1];
    
  }
  lcd.begin(16,2);
  starting_millis = millis();  
  millis_timer = wholeNote/16;  
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  current_millis = millis();
  x_vector = analogRead(X_pin) - 511;
  y_vector = analogRead(Y_pin)- 511;
  note = play_instrument(x_vector, y_vector);  

  if (current_millis - starting_millis >= millis_timer) {
    for(j = 0; j < sizeof(melody)/2; j++) {
      move_note(button[j],arrow[j],count[j]);
      count[j]--;
    }    
    starting_millis = current_millis;
  }
}

void move_note(char button, int arrow, int count) {
  lcd.home();
  if (count >= -1 && count <= 15) {
    lcd.setCursor(count, 1);
    lcd.write(byte(arrow));
    lcd.setCursor(count + 1, 1);
    lcd.write(" ");

    lcd.setCursor(count, 0);
    lcd.print(button);
    lcd.setCursor(count + 1, 0);
    lcd.write(" ");
  }
}

double shift_coordinates(int x_vector, int y_vector) {
  

  x_prime = x_vector*cos(-0.393)-y_vector*sin(-0.393);//radian equivalent of 22.5 deg
  y_prime = x_vector*sin(-0.393)+y_vector*cos(-0.393);
  
// x' = x cos f - y sin f
//y' = y cos f + x sin f
  
  double theta_rad = -atan(y_prime/x_prime);
  double theta_deg = theta_rad*180/3.14159265;
  if (x_prime > 0) {
    theta_deg = theta_deg+90;
  }
  else if (x_prime < 0) {
    theta_deg = theta_deg + 270;
  }
  return theta_deg;
}

double play_instrument(int x_vector, int y_vector) {
  double theta_deg = shift_coordinates(x_vector, y_vector);

  if (digitalRead(8) == LOW && digitalRead(9) == HIGH && digitalRead(10) == HIGH){
    frequency = play_tone(285.3,theta_deg);
  } 
  else if (digitalRead(9) == LOW && digitalRead(8) == HIGH && digitalRead(10) == HIGH) {
    frequency = play_tone(427.475, theta_deg);
  }
  else if (digitalRead(10) == LOW && digitalRead(8) == HIGH && digitalRead(9) == HIGH){
    frequency = play_tone(640.484, theta_deg);
  }
  else {
  noTone(7);
  }
  return frequency;
}
double play_tone(int starting_freq, double deg) {
  double freq = starting_freq*pow(1.059464,deg/45);
  if (digitalRead(6) == LOW) {
    freq = freq*2;
  }
  else{
    freq = freq;
  }

  tone(7, freq);
  return freq;
}

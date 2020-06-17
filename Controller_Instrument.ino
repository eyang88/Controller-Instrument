/*This program plays an electronic instrument fabricated using a video game 
controller as inspiration. A piezo buzzer is used to create noise and different
inputs change the pitch. LCD menu, tutorial and "sheet music tabs" are 
currently under development*/

#define melodyPin 7
#define L_button 6
#define D_button 8
#define A_button 9
#define E_button 10
#include <math.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int X_pin = 0;
const int Y_pin = 1;
int x_vector, y_vector; 
double x_prime, y_prime;
double theta_rad, theta_deg, theta;
double magnitude;


void setup() {
  pinMode(melodyPin, OUTPUT);
  pinMode(L_button, INPUT_PULLUP);
  pinMode(D_button, INPUT_PULLUP);
  pinMode(A_button, INPUT_PULLUP);
  pinMode(E_button, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  
  x_vector = analogRead(X_pin) - 511;
  y_vector = analogRead(Y_pin) - 511;
  
  theta = shift_coordinates(x_vector, y_vector);
  //D button is pressed
  if (digitalRead(D_button) == LOW && digitalRead(A_button) == HIGH && digitalRead(E_button) == HIGH){
    play_tone(285.3,theta_deg);//starting tone is D4
  } 
  //A button is pressed
  else if (digitalRead(A_button) == LOW && digitalRead(D_button) == HIGH && digitalRead(E_button) == HIGH) {
    play_tone(427.475, theta_deg);//starting tone is A4
  }
  //E button is pressed
  else if (digitalRead(E_button) == LOW && digitalRead(D_button) == HIGH && digitalRead(A_button) == HIGH){
    play_tone(640.484, theta_deg);//starting tone is E5
  }
  else {
  noTone(7);
  }

}

/*Converts joystick position values into degree measurements*/
int shift_coordinates(int x, int y) {

  x_prime = x*cos(-0.393)+y*sin(-0.393);//radian equivalent of 22.5 deg
  y_prime = -x*sin(-0.393)+y*cos(-0.393);
  
  theta_rad = -atan(y_prime/x_prime);
  theta_deg = theta_rad*180/3.14159265;
  if (x_prime > 0) {
    theta_deg = theta_deg+90;
  }
  else if (x_prime < 0) {
    theta_deg = theta_deg + 270;
  }
  return theta_deg;
}

/*plays a tone at the specified frequency*/
void play_tone(int starting_freq, double deg) {
  double freq = starting_freq*pow(1.059464,deg/45);//calculated by moving one half step in 45 degrees
  if (digitalRead(L_button) == LOW) {
    freq = freq*2;//tone raises an octave when L button is pressed
  }
  else{
    freq = freq;
  }
  tone(7, freq);
}
  

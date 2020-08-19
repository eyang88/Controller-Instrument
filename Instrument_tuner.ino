#include <LiquidCrystal.h>
#define melodyPin 7
#define L_button 6
#define D_button 8
#define A_button 9
#define E_button 10

byte flat[] = {//Creates custom character for the flat symbo
  B01000,
  B01000,
  B01011,
  B01101,
  B01001,
  B01001,
  B01110,
  B00000
}; 
int note, frequency;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int X_pin = 0;
const int Y_pin = 1;
int x_vector, y_vector;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2); 
  lcd.createChar(0, flat);
  lcd.display();
  pinMode(melodyPin, OUTPUT);
  pinMode(L_button, INPUT_PULLUP);
  pinMode(D_button, INPUT_PULLUP);
  pinMode(A_button, INPUT_PULLUP);
  pinMode(E_button, INPUT_PULLUP);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  x_vector = analogRead(X_pin) - 511;// Origin is at center of joystick
  y_vector = analogRead(Y_pin) - 511;        
  note = play_instrument(x_vector, y_vector);
  tuner(note);
  
}

  int play_instrument(int x, int y) {
  theta = shift_coordinates(x, y);

  //D button is pressed
  if (digitalRead(D_button) == LOW && digitalRead(A_button) == HIGH && digitalRead(E_button) == HIGH) {
    frequency = play_tone(285.3, theta); //starting tone is D4
  }
  //A button is pressed
  else if (digitalRead(A_button) == LOW && digitalRead(D_button) == HIGH && digitalRead(E_button) == HIGH) {
    frequency = play_tone(427.475, theta);//starting tone is A4
  }
  //E button is pressed
  else if (digitalRead(E_button) == LOW && digitalRead(D_button) == HIGH && digitalRead(A_button) == HIGH) {
    frequency = play_tone(640.484, theta);//starting tone is E5
  }
  else {
    noTone(7);
  }
  return frequency;
  }
//Rotates coordinate axis for the controller for easier manipulation
double shift_coordinates(int x_vector, int y_vector) {
  
  double x_prime = x_vector*cos(-0.393)-y_vector*sin(-0.393);//radian equivalent of 22.5 deg
  double y_prime = x_vector*sin(-0.393)+y_vector*cos(-0.393);
//x' = x cos f - y sin f
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

int play_tone(int starting_freq, double deg) {
  double freq = starting_freq * pow(1.059464, deg / 45); //calculated by moving one half step in 45 degrees
  if (digitalRead(L_button) == LOW) {
    freq = freq * 2; //tone raises an octave when L button is pressed
  }
  else {
    freq = freq;
  }
  tone(7, freq);
  return freq;
}
//Sets up tuner in the LCD screen
void tuner(int c) {
  String note;
  float absolute;
  char sharp = 0x00 + 34;
  String topRow = " Hz";  
  if (c > 277.18 && c < 311.13) {
    note = "D4";
    absolute = 293.66;
  }
  else if (c > 311.13 && c < 340) {
    note = "E4";
    absolute = 329.63;
  }
  else if (c > 340 && c < 369.99) {
    note = "F4";
    absolute = 349.23;
  }
  else if (c > 369.99 && c < 415.3) {
    note = "G4";
    absolute = 392;
  }
  else if (c > 415.3 && c < 466.16) {
    note = "A4";
    absolute = 440;
  }
  else if (c > 466.16 && c < 505) {
    note = "B4";
    absolute = 493.88;
  }
  else if (c > 505 && c < 554.37) {
    note = "C5";
    absolute = 523.25;
  }
  else if (c > 554.37 && c < 622.25) {
    note = "D5";
    absolute = 587.33;
  }
  else if (c > 622.25 && c < 680) {
    note = "E5";
    absolute = 659.25;
  }
  else if (c > 680 && c < 739.99) {
    note = "F5";
    absolute = 698.46;
  }
  else if (c > 739.99 && c < 830.61) {
    note = "G5";
    absolute = 783.99;
  }
  else if (c > 830.61 && c < 932.33) {
    note = "A5";
    absolute = 880;
  }
  else if (c > 932.33 && c <1010) {
    note = "B5";
    absolute = 987.77;
  }
  else if (c > 1010 && c <1108.73) {
    note = "C6";
    absolute = 1046.5;
  }  
  else if (c > 1108.73 && c <1244.51) {
    note = "D6";
    absolute = 1174.66;
  }
  else if (c > 1244.51 && c < 1350) {
    note = "E6";
    absolute = 1318.51;
  }
  else if (c > 1350 && c < 1479.98) {
    note = "F6";
    absolute = 1396.91;
  }
  else if (c > 1479.98 && c < 1661.2) {
    note = "G6";
    absolute = 1567.98;
  }
  else if (c > 1661.22 && c < 1864.66) {
    note = "A6";
    absolute = 1760;
  }
  else if (c > 1864.66 && c < 2093) {
    note = "B6";
    absolute = 1975.53;
  }
  else {
    note = "ERROR";
    absolute = 0;
  }
  lcd.clear();
  lcd.home();
  lcd.print(c + topRow);
  lcd.setCursor(8,0);
  lcd.write(byte(0));
  lcd.setCursor(11,0);
  lcd.write(42);
  lcd.setCursor(14,0);
  lcd.write(35);
  lcd.setCursor(0,1);
  lcd.print(note);
  if (c < absolute/1.05) {
    lcd.setCursor(8,1);
  }
  else if (c > absolute/1.05 && c < absolute/1.03) {
    lcd.setCursor(9,1);
  }
  else if (c > absolute/1.03 && c < absolute/1.01) {
    lcd.setCursor(10,1);
  }
  else if (c > absolute/1.01 && c < absolute*1.01) {
    lcd.setCursor(11,1);
  }
  else if (c > absolute*1.01 && c < absolute*1.03) {
    lcd.setCursor(12,1);
  }
  else if (c > absolute*1.03 && c < absolute*1.05) {
    lcd.setCursor(13,1);
  }
  else if (c > absolute*1.05) {
    lcd.setCursor(14,1);
  }
  lcd.write(255);
}

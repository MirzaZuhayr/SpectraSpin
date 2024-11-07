#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define S0 0
#define S1 2
#define S2 8
#define S3 10
#define OUT 11
#define LED 7
#define RED 0
#define YELLOW 1
#define GREEN 2
#define CYAN 3
#define BLUE 4
#define MAGENTA 5
#define BUTTON 12

int cheat = 0;


Servo myservo;
Servo myservo2; 
byte servoPin1 = 22; // First servo pin
byte servoPin2 = 17; // Second servo pin

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(LED, HIGH);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);
  Serial.begin(115200);
  // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }

  Serial.println("ESP32 Servo Test");

  myservo.attach(servoPin1, 500, 2500);
  myservo2.attach(servoPin2, 500, 2500); // Attach the second servo

  Serial.println("Servos attached. Moving to 0 degrees.");
  myservo.writeMicroseconds(500);  // Move first servo to 0 degrees
  myservo2.writeMicroseconds(500); // Move second servo to 0 degrees
  delay(1000);
}

struct color {
  float r;
  float g;
  float b;
};

struct color normalize(color c) {
  float length = sqrt(sq(c.r) + sq(c.g) + sq(c.b));
  return color {.r = c.r/length, .g = c.g/length, .b = c.b/length};
}

float color_distance(color c1, color c2) {
  c1 = normalize(c1);
  c2 = normalize(c2);
  return sqrt(sq(c2.r - c1.r) + sq(c2.g - c1.g) + sq(c2.b - c1.b));
}

String color_strings[6] = {
  "red = \nyellow + \nmagenta",
  "yellow = \nyellow + \nclear",
  "green = \nyellow + \ncyan",
  "cyan = \ncyan + \nclear",
  "blue = \ncyan + \nmagenta",
  "magenta = \nmagenta + \nclear"
};

color measured_color_values[6] {
  {.r = 0.31, .g = 0.73, .b = 0.61}, //red
  {.r = 0.37, .g = 0.48, .b = 0.8}, //yellow
  {.r = 0.66, .g = 0.46, .b = 0.58}, //green
  {.r = 0.77, .g = 0.48, .b = 0.43}, //cyan
  {.r = 0.74, .g = 0.56, .b = 0.37}, //blue
  {.r = 0.29, .g = 0.82, .b = 0.49}, //magenta
};

int closest_color(color input_color) {
  float min_dist = color_distance(input_color, measured_color_values[0]);
  float d;
  int min_color = 0;
  for (int i = 1; i < 6; i++) {
    d = color_distance(input_color, measured_color_values[i]);
    if (d < min_dist) {
      min_dist = d;
      min_color = i;
    }
  }
  return min_color;
}

void servo_move(int calculated_color) {

  //Create Red
  calculated_color = cheat;
  cheat = (cheat + 1) % 6;
  if (calculated_color == 0) {
    Serial.println("Moving first servo to 0 degrees and second to 180");
    myservo.writeMicroseconds(500);
    myservo2.writeMicroseconds(1850);
    //myservo.writeMicroseconds(1100); // Adjust this value as needed for 90 degrees
    delay(3000); // Wait for the servo to reach the position
  }
  
  if (calculated_color == 1) {
    // Create Yellow
    Serial.println("Moving first to 0 and second to 270");
    myservo.writeMicroseconds(500);
    myservo2.writeMicroseconds(2500); 
    delay(3000); // Wait for the servo to reach the position
  }
  

  // Move first servo to 180 degrees (1850)
  if (calculated_color == 2) {
    //Create Green
    Serial.println("Moving first servo to 0 degrees and second to 90");
    myservo.writeMicroseconds(500); 
    myservo2.writeMicroseconds(1100);
    delay(3000); // Wait for the servo to reach the position
  }
  
  if (calculated_color == 3) {
    //Create Cyan
    // Move second servo to 180 degrees
    Serial.println("Moving first servo to 270 and second servo to 90 degrees");
    myservo.writeMicroseconds(2500);
    myservo2.writeMicroseconds(1100); 
    delay(3000); // Wait for the servo to reach the position
  }
  
  if (calculated_color == 4) {
    // Create Blue
    Serial.println("Moving first servo to 180 degrees and second to 90");
    myservo.writeMicroseconds(1850); 
    myservo2.writeMicroseconds(1100);
    delay(3000); // Wait for the servo to reach the position
  }
  
  if (calculated_color == 5) {
    // Create Magenta
    Serial.println("Moving first servo to 180 second servo to 270 degrees");
    myservo.writeMicroseconds(1850);
    myservo2.writeMicroseconds(2500); // Move second servo to 270 degrees
    delay(1000); // Wait for the servo to reach the position
      }

}

void display_color(int calculated_color) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(color_strings[calculated_color]);
  display.display();      // Show initial text
}

void play_note(float freq, int millis) {
  int iterations = millis * 32 / (5 * freq) ;
  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < freq; i++) {
      analogWrite(13, sin(i*6.28/freq) * 1024);
    }

  }
}

void play_tune(float scale) {
  play_note(4, 100);
  delay(100);
  play_note(5, 100);
  play_note(6, 100);
  play_note(1, 100);

}

int temp;
int red;
int green;
int blue;
color input_color;
int calculated_color;

void loop() {
  
  // set S2 and S3 to use red filtered photodiodes
  // min: 45
  // max: 300
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  for (int i = 0; i < 100; i++ ) {
    temp += pulseIn(OUT, LOW);
  }
  red = temp/100;
  temp = 0;
  // Serial.print("R = ");
  // Serial.print(red);
  delay(100);

  
  // set S2 and S3 to use green filtered photodiodes
  // min: 160
  // max: 700
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  for (int i = 0; i < 100; i++) {
    temp += pulseIn(OUT, LOW);
  }
  green = temp/100;
  temp = 0;
  // Serial.print(" G = ");
  // Serial.print(green);
  delay(100);


  // set S2 and S3 to use blue filtered photodiodes
  // min: 120
  // max: 2000
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  for (int i = 0; i < 100; i++) {
    temp += pulseIn(OUT, LOW);
  }
  blue = temp/100;
  temp = 0;
  // Serial.print(" B = ");
  // Serial.println(blue);
  delay(100);

  input_color = color {.r = red, .g = green, .b = blue};
  color normalized = normalize(input_color);
  // Serial.print("R = ");
  // Serial.print(normalized.r);
  // Serial.print(" G = ");
  // Serial.print(normalized.g);
  // Serial.print(" B = ");
  // Serial.println(normalized.b);
  while(digitalRead(BUTTON));
  if (!digitalRead(BUTTON)) {
    calculated_color = closest_color(normalized);
    play_tune(1);
    digitalWrite(4, LOW);
    while (!digitalRead(BUTTON));
    digitalWrite(4, HIGH);
  }
  servo_move(calculated_color);
  
  Serial.println(color_strings[calculated_color]);
  //display_color(calculated_color);
}

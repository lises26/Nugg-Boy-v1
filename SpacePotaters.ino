#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//button var setup
int b_shoot = 2;
int b_right = 3;
int b_left = 4;

int shoot_val;
int right_val;
int left_val;

int buzzpin = 11;

int px = 62;
int by = 15;

int spawnNum;
int moveNum;
int ey = 0;
int ex;

int score = 0;

bool start;
bool reset;


//set x-coordinate for enemy spawn
void spawn_enemy(){
  spawnNum = random(1, 6);
  switch (spawnNum) {
    case 1:
      ex = 12;
      break;
    case 2:
      ex = 37;
      break;
    case 3:
      ex = 62;
      break;
    case 4:
      ex = 87;
      break;
    case 5:
      ex = 112;
      break;
  }
}


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();

  //button initialization
  pinMode(b_shoot, INPUT);
  pinMode(b_right, INPUT);
  pinMode(b_left, INPUT);

  digitalWrite(b_shoot, HIGH);
  digitalWrite(b_right, HIGH);
  digitalWrite(b_left, HIGH);

  menu();
}


//game over function
void game_over(){
  reset = false;
  while (reset == false){
    shoot_val = digitalRead(b_shoot);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("GAME OVER");
    display.println("score:");
    display.println(score);
    if (shoot_val == 0){
      reset = true;
      score = 0;
      ey = 0;
      px = 62;
      spawn_enemy();
      delay(500);
      loop();
    }
    display.display();
  }
}


//this function determines the enemy's movement
void enemy_move(){
  moveNum = random(1, 10);

  switch (moveNum){
    case 1:
      ex -= 25;
      if (ex < 12){
        ex = 112;
      }
      break;
      
    case 2:
      ex += 25;
      if (ex > 112) {
        ex = 12;
      }
      break;
      
    case 3:
      ey += 3;
      if (ey >= 15){
        game_over();
      }
  }
}


//the titlescreen/menu
void menu(){
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  start = false;
  
  while (start == false) {
    display.clearDisplay();
    display.setCursor(20, 12);
    display.print("SPACE POTATERS");
    display.display();
    shoot_val = digitalRead(b_shoot); 

    if (shoot_val == 0) {
      start = true;
      shoot_val = 1;
      delay(500);
    }
  } 
}


//this is the main game loop
void loop() {
 
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.clearDisplay();

  shoot_val = digitalRead(b_shoot);
  right_val = digitalRead(b_right);
  left_val = digitalRead(b_left);

  //moving left
  if (left_val == 0){
    px -= 25;
    if (px < 12){
      px = 112;
    }
  }

  //moving right
  if (right_val == 0){
    px += 25;
    if (px > 112) {
      px = 12;
    }
  }

  //shooting
  if (shoot_val == 0){

    //displaying everything
    display.setCursor(ex, ey);
    display.print("o");

    display.setCursor(0, 25);
    display.print("score:");
    display.println(score);
    
    display.setCursor(px, 21);
    display.print("^");

    //lazer path
    for (int i = 0; i <= 5 ; i++){
      tone(buzzpin, 1000);
      display.setCursor(px, by);
      display.print("|");
      display.display();
      by -= 3;

      //hit checking
      if (by == ey and px == ex){
        tone(buzzpin, 2000);
        delay(500);
        noTone(buzzpin);
        ey = 0;
        score += 3;
        spawn_enemy();
      }
    }
    noTone(buzzpin);
    by = 15;
  }

  enemy_move();

  display.setCursor(ex, ey);
  display.print("o");
  
  display.setCursor(px, 21);
  display.print("^");

  display.setCursor(0, 25);
  display.print("score:");
  display.println(score);
  
  display.display(); 
}

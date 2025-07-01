#include <LedControl.h>

// Need a snake struct
typedef struct S S;
struct S{
  int head[2];     // head position
  int body[64][2]; // Array for the rest of the body positions
  int len;         // Snake length
  int dir[2];      // Snake direction
};

// Need an apple struct
typedef struct A A;
struct A{
  int rPos; //The row index of the apple
  int cPos; //The column index of the apple
};

//MAX72XX led Matrix
const int DIN = 12;
const int CS = 11;
const int CLK = 10;
LedControl l_matrix = LedControl(DIN, CLK, CS,1);

const int varXPin = A3; //X Value  from Joystick
const int varYPin = A4; //Y Value from Joystick

byte pic[8] = {0,0,0,0,0,0,0,0}; //The 8 rows of the LED Matrix

S snake = {{1,1},{{0,1}, {1,1}}, 2, {1,0}}; //Initialize a snake object
A apple = {random(0,8),random(0,8)}; //Initialize an apple object

//Variables To Handle The Game Time
float oldTime = 0;
float timer = 0;
float updateRate = 3;

int i,j; // Counters

void setup() {
  // Proper wake-up call
  l_matrix.shutdown(0,false);
  /* Set the brightness to a medium values */
  l_matrix.setIntensity(0,8);
  /* and clear the display */
  l_matrix.clearDisplay(0);

  //Set Joystick Pins as INPUTs
  pinMode(varXPin, INPUT);
  pinMode(varYPin, INPUT);
}


void loop() {
  timer += calculateTimeChange();

  // Check for inputs
  int xVal = analogRead(varXPin);
  int yVal = analogRead(varYPin);

  if(xVal<100 && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = -1;
  }else if(xVal>923 && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = 1;
  }else if(yVal<100 && snake.dir[0]==0){
    snake.dir[0] = -1;
    snake.dir[1] = 0;
  }else if(yVal>923 && snake.dir[0]==0){
    snake.dir[0] = 1;
    snake.dir[1] = 0;
  }

  // We do not want to update too frequently
  if (timer > 1000/updateRate){
    timer = 0;
    Update();
  }

  Render();
}


float calculateTimeChange() {
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}


float reset() {
  for (int j=0; j<8; j++) {
    pic[j]=0;
  }
}


void Update() {
  reset();

  int newHead[2] = {snake.head[0]+snake.dir[0], snake.head[1]+snake.dir[1]};

  // Handle Borders (we can't go diagonally so it's fine)
  bool isCollision = ((newHead[0] == -1 || newHead[0] == 8 || 
     newHead[1] == -1 || newHead[1] == 8) || (snake.body[j][0] == newHead[0] && snake.body[j][1] == newHead[1]));

  for (j=0; j<snake.len; j++) {
    if (isCollision) {
      delay(1000);
      snake = {{1,1}, {{0,1},{1,1}}, 2, {1,0}}; // Reinitialise the snake object
      apple = {random(0,8), random(0,8)}; // Reinitialise the apple object
      return;
    }
  }

  // Check if the snake ate the apple
  if (newHead[0] == apple.rPos && newHead[1] == apple.cPos) {
    snake.len = snake.len+1;
    apple.rPos = random(0,8);
    apple.cPos = random(0,8);
  }
  else {
    removeFirst(); // Shift the array to the left
  }

  snake.body[snake.len-1][0] = newHead[0];
  snake.body[snake.len-1][1] = newHead[1];

  snake.head[0] = newHead[0];
  snake.head[1] = newHead[1];

  //Update the pic Array to Display(snake and apple)
  for(j=0; j<snake.len; j++){
    pic[snake.body[j][0]] |= 128 >> snake.body[j][1];
  }
  pic[apple.rPos] |= 128 >> apple.cPos;
}


void Render(){
   for(i=0;i<8;i++){
    l_matrix.setRow(0, i, pic[i]);
   }
}


void removeFirst(){
  for(j=1; j<snake.len; j++){
    snake.body[j-1][0] = snake.body[j][0];
    snake.body[j-1][1] = snake.body[j][1];
  }
}




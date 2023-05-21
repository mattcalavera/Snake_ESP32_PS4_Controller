#include <PS4Controller.h>
#include "Arduino.h"
#include "heltec.h"
#include "images.h"


#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT        64 // OLED display height, in pixels

typedef enum {
  LEFT,
  UP,
  RIGHT,
  DOWN
} Direction;

typedef enum {
  START,
  RUNNING,
  GAMEOVER
} State;

State gameState;

#define SNAKE_PIECE_SIZE     3
#define MAX_SANKE_LENGTH   165
#define MAP_SIZE_X          40
#define MAP_SIZE_Y          20
#define STARTING_SNAKE_SIZE  5

//you can change snake speed with R1 and L1 (I plan to insert a sort of debounce for that keys)
int SNAKE_MOVE_DELAY = 25;

int8_t snake[MAX_SANKE_LENGTH][2];
uint8_t snake_length;
Direction dir;
Direction newDir;

int8_t fruit[2];

int r = 0;
int g = 255;
int b = 0;

void setup() {
  Serial.begin(115200);
  PS4.begin("00:00:00:00:00:00");   //<--- insert here BT MAC Address of the device to connect (see : https://github.com/aed3/PS4-esp32#pairing-the-ps4-controller)
  Serial.println("Ready....");

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  do {
    Heltec.display->clear();
    Heltec.display->display();
    delay(250);
    Heltec.display->drawXbm(34, 7, BTLogo_width, BTLogo_height, BTLogo_bits);
    Heltec.display->display();
    delay(400);
  } while (PS4.isConnected() == false);
  setupGame();
}

void resetSnake() {
  snake_length = STARTING_SNAKE_SIZE;
  for (int i = 0; i < snake_length; i++) {
    snake[i][0] = MAP_SIZE_X / 2 - i;
    snake[i][1] = MAP_SIZE_Y / 2;
  }
}

void setupGame() {
  gameState = START;
  dir = RIGHT;
  newDir = RIGHT;
  resetSnake();
  generateFruit();
  Heltec.display->clear();
  for (int i = 0; i < 5; i++) {
    Serial.println("ecccoci");
    Heltec.display->clear();
    Heltec.display->display();
    delay(100);
    Heltec.display->drawXbm(50, 10, Snake_width, Snake_height, Snake_bits);
    Heltec.display->display();
    delay(200);
  }
  drawMap();
  drawScore();
  PS4.setLed(0, 255, 0);
  PS4.sendToController();
  delay(10);
  PS4.setRumble(125, 0);
  PS4.sendToController();
  delay(250);
  PS4.setRumble(0, 0);
  PS4.sendToController();
  //drawPressToStart();
}

int moveTime = 0;
void loop() {
  switch (gameState) {
    case START:
      if (buttonPress()) gameState = RUNNING;
      break;

    case RUNNING:
      moveTime++;
      readDirection();
      if (PS4.R1()) SNAKE_MOVE_DELAY = SNAKE_MOVE_DELAY - 1;
      if (PS4.L1()) SNAKE_MOVE_DELAY = SNAKE_MOVE_DELAY + 1;
      if (moveTime >= SNAKE_MOVE_DELAY) {
        dir = newDir;
        Heltec.display->clear();

        if (moveSnake()) {
          gameState = GAMEOVER;
          drawGameover();
          delay(1000);
        }
        drawMap();
        drawScore();
        Heltec.display->display();
        checkFruit();
        moveTime = 0;
      }
      break;

    case GAMEOVER:
      if (buttonPress()) {
        delay(500);
        setupGame();
        gameState = START;
      }
      break;
  }

  delay(10);
}

bool buttonPress() {
  if (PS4.Right()) {
    return true;
  } else if (PS4.Down()) {
    return true;
  } else if (PS4.Up()) {
    return true ;
  } else if (PS4.Left()) {
    return true;
  }
  return false;
}

void readDirection() {
  if (PS4.Right()) {
    newDir = (Direction)2;
    return;
  }
  if (PS4.Down()) {
    newDir = (Direction)3;
    return;
  }
  if (PS4.Up()) {
    newDir = (Direction)1;
    return;
  }
  if (PS4.Left()) {
    newDir = (Direction)0;
    return;
  }
}

bool moveSnake() {
  int8_t x = snake[0][0];
  int8_t y = snake[0][1];

  switch (dir) {
    case LEFT:
      x -= 1;
      break;
    case UP:
      y -= 1;
      break;
    case RIGHT:
      x += 1;
      break;
    case DOWN:
      y += 1;
      break;
  }

  if (collisionCheck(x, y))
    return true;
  for (int i = snake_length - 1; i > 0; i--) {
    snake[i][0] = snake[i - 1][0];
    snake[i][1] = snake[i - 1][1];
  }

  snake[0][0] = x;
  snake[0][1] = y;
  return false;
}

void checkFruit() {
  if (fruit[0] == snake[0][0] && fruit[1] == snake[0][1])
  {
    if (snake_length + 1 <= MAX_SANKE_LENGTH)
      snake_length++;
    //Small rumble?
    generateFruit();
  }
}

void generateFruit() {
  bool b = false;
  do {
    b = false;
    fruit[0] = random(0, MAP_SIZE_X);
    fruit[1] = random(0, MAP_SIZE_Y);
    for (int i = 0; i < snake_length; i++) {
      if (fruit[0] == snake[i][0] && fruit[1] == snake[i][1]) {
        b = true;
        continue;
      }
    }
  } while (b);
}

bool collisionCheck(int8_t x, int8_t y) {
  for (int i = 1; i < snake_length; i++) {
    if (x == snake[i][0] && y == snake[i][1]) return true;
  }
  if (x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y) return true;
  return false;
}

void drawMap() {
  PS4.setLed(0, 255, 0);
  PS4.sendToController();
  int offsetMapX = SCREEN_WIDTH - SNAKE_PIECE_SIZE * MAP_SIZE_X - 2;
  int offsetMapY = 2;
  Heltec.display->drawRect(fruit[0] * SNAKE_PIECE_SIZE + offsetMapX, fruit[1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE);
  Heltec.display->drawRect(offsetMapX - 2, 0, SNAKE_PIECE_SIZE * MAP_SIZE_X + 4, SNAKE_PIECE_SIZE * MAP_SIZE_Y + 4);
  for (int i = 0; i < snake_length; i++) {
    Heltec.display->fillRect(snake[i][0] * SNAKE_PIECE_SIZE + offsetMapX, snake[i][1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE);
  }
}

void drawGameover() {
  PS4.setLed(255, 0, 0);
  PS4.setRumble(0, 200);
  PS4.sendToController();
  delay(150);
  PS4.setRumble(0, 0);
  PS4.sendToController();
  delay(150);
  PS4.setRumble(0, 200);
  PS4.sendToController();
  delay(150);
  PS4.setRumble(0, 0);
  PS4.sendToController();
  Heltec.display->drawXbm(MAP_SIZE_X / 2 + 5, MAP_SIZE_Y / 2, gameover_width, gameover_height,  gameover_bits);
  Heltec.display->display();
  delay(500);
}

void drawScore() {
  Heltec.display->drawString(0, 0, String(snake_length - STARTING_SNAKE_SIZE));
}

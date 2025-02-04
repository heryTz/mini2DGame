#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int currentTimestamp() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void drawGrid(int screenWidth, int screenHeight, int landmarkUnitX,
              int landmarkUnitY) {
  for (int i = 0; i < screenWidth; i += landmarkUnitX) {
    DrawLine(i, 0, i, screenHeight, LIGHTGRAY);
  }
  for (int i = 0; i < screenHeight; i += landmarkUnitY) {
    DrawLine(0, i, screenWidth, i, LIGHTGRAY);
  }
}

void drawFood(Vector2 *food, int landmarkUnitX, int landmarkUnitY) {
  if (food != NULL) {
    DrawRectangle(food->x * landmarkUnitX, food->y * landmarkUnitY,
                  landmarkUnitX, landmarkUnitY, GREEN);
  }
}

void drawSnake(Vector2 *snake, int snakeLength, int landmarkUnitX,
               int landmarkUnitY) {
  for (int i = 0; i < snakeLength; i++) {
    DrawRectangle(snake[i].x * landmarkUnitX, snake[i].y * landmarkUnitY,
                  landmarkUnitX, landmarkUnitY, DARKGRAY);
  }
}

typedef enum { UP, RIGHT, DOWN, LEFT } Direction;

typedef enum { PAUSED, RUNNING, GAMEOVER, FINISHED } PlayState;

void moveSnake(Vector2 *snake, int *snakeLength, int nbGridX, int nbGridY,
               int landmarkUnitX, int landmarkUnitY, Direction direction) {
  Vector2 prevSnakePart;
  int offsetX = nbGridX - 1;
  int offsetY = nbGridY - 1;
  for (int i = 0; i < *snakeLength; i++) {
    Vector2 currentSnakePart = snake[i];
    if (i == 0) {
      if (direction == UP) {
        if (snake[i].y - 1 < 0) {
          snake[i].y = offsetY;
        } else {
          snake[i].y -= 1;
        }
      } else if (direction == RIGHT) {
        if (snake[i].x + 1 > offsetX) {
          snake[i].x = 0;
        } else {
          snake[i].x += 1;
        }
      } else if (direction == DOWN) {
        if (snake[i].y + 1 > offsetY) {
          snake[i].y = 0;
        } else {
          snake[i].y += 1;
        }
      } else if (direction == LEFT) {
        if (snake[i].x - 1 < 0) {
          snake[i].x = offsetX;
        } else {
          snake[i].x -= 1;
        }
      }
    } else {
      snake[i].x = prevSnakePart.x;
      snake[i].y = prevSnakePart.y;
    }
    prevSnakePart = currentSnakePart;
  }
  drawSnake(snake, *snakeLength, landmarkUnitX, landmarkUnitY);
}

bool snakeFindFood(Vector2 *snake, Vector2 *food, int snakeLength) {
  Vector2 snakeHead = snake[0];
  if (food != NULL && snakeHead.x == food->x && snakeHead.y == food->y) {
    return true;
  }
  return false;
}

void eatFood(Vector2 *snake, int *snakeLength, Vector2 *food) {
  snake[*snakeLength] = (Vector2){food->x, food->y};
  (*snakeLength)++;
}

bool isVectorInclude(Vector2 *vectors, int vectorsLength, Vector2 vector) {
  for (int i = 0; i < vectorsLength; i++) {
    if (vectors[i].x == vector.x && vectors[i].y == vector.y) {
      return true;
    }
  }
  return false;
}

bool isEatSelf(Vector2 *snake, int snakeLength) {
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      return true;
    }
  }
  return false;
}

bool isGameFinished(int snakeLength, int nbGridX, int nbGridY) {
  if (snakeLength == nbGridX * nbGridY) {
    return true;
  }
  return false;
}

Vector2 createFood(Vector2 *snake, int snakeLength, int nbGridX, int nbGridY) {
  double randomX = -1;
  double randomY = -1;
  int offsetX = nbGridX - 1;
  int offsetY = nbGridY - 1;

  do {
    randomX = GetRandomValue(0, offsetX);
    randomY = GetRandomValue(0, offsetY);
  } while (isVectorInclude(snake, snakeLength, (Vector2){randomX, randomY}));

  return (Vector2){randomX, randomY};
}

int main() {
  const int defaultScreenWidth = 800;
  const int defaultScreenHeight = 450;

  InitWindow(defaultScreenWidth, defaultScreenHeight, "Snake Game");

  SetTargetFPS(60);

  int screenHeight = GetScreenHeight();
  int screenWidth = GetScreenWidth();

  int nbGridX = 40;
  int nbGridY = 30;
  int landmarkUnitX = screenWidth / nbGridX;
  int landmarkUnitY = screenHeight / nbGridY;
  Vector2 screenTopLeft = {0, 0};
  Vector2 screenTopRight = {screenWidth, 0};
  Vector2 screenBottomLeft = {0, screenHeight};
  Vector2 screenBottomRight = {screenWidth, screenHeight};
  Vector2 *snake = malloc(nbGridX * nbGridY * sizeof(Vector2));
  snake[0] = (Vector2){0, 0};
  int snakeLength = 1;
  Vector2 *food = (Vector2 *)malloc(sizeof(Vector2));
  *food = createFood(snake, snakeLength, nbGridX, nbGridY);
  int snakeSpeedInMs = 100;
  int prevRefreshSnakeTime = currentTimestamp();
  int changeDirectionSpeed = 2000;
  Direction direction = RIGHT;
  PlayState playState = RUNNING;

  drawSnake(snake, snakeLength, landmarkUnitX, landmarkUnitY);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawGrid(screenWidth, screenHeight, landmarkUnitX, landmarkUnitY);
    drawSnake(snake, snakeLength, landmarkUnitX, landmarkUnitY);
    drawFood(food, landmarkUnitX, landmarkUnitY);

    int keyPressed = GetKeyPressed();
    Direction nextDirection = direction;
    int currentTime = currentTimestamp();

    if (playState == RUNNING) {
      switch (keyPressed) {
      case KEY_SPACE:
        playState = PAUSED;
        continue;
        break;
      case KEY_UP:
      case KEY_RIGHT:
      case KEY_DOWN:
      case KEY_LEFT:
        if (keyPressed == KEY_UP) {
          nextDirection = UP;
        } else if (keyPressed == KEY_RIGHT) {
          nextDirection = RIGHT;
        } else if (keyPressed == KEY_DOWN) {
          nextDirection = DOWN;
        } else if (keyPressed == KEY_LEFT) {
          nextDirection = LEFT;
        }

        if (((direction == UP || direction == DOWN) &&
             (nextDirection == RIGHT || nextDirection == LEFT)) ||
            ((direction == RIGHT || direction == LEFT) &&
             (nextDirection == UP || nextDirection == DOWN))) {
          direction = nextDirection;
          moveSnake(snake, &snakeLength, nbGridX, nbGridY, landmarkUnitX,
                    landmarkUnitY, direction);
          if (isEatSelf(snake, snakeLength)) {
            playState = GAMEOVER;
            continue;
          }
          if (isGameFinished(snakeLength, nbGridX, nbGridY)) {
            playState = FINISHED;
            continue;
          }
          prevRefreshSnakeTime = currentTimestamp();
        }
        break;
      }

      if (currentTime - prevRefreshSnakeTime > snakeSpeedInMs) {
        moveSnake(snake, &snakeLength, nbGridX, nbGridY, landmarkUnitX,
                  landmarkUnitY, direction);
        if (isEatSelf(snake, snakeLength)) {
          playState = GAMEOVER;
          continue;
        }
        if (isGameFinished(snakeLength, nbGridX, nbGridY)) {
          playState = FINISHED;
          continue;
        }
        prevRefreshSnakeTime = currentTimestamp();
      }

      if (food != NULL && snakeFindFood(snake, food, snakeLength)) {
        eatFood(snake, &snakeLength, food);
        *food = createFood(snake, snakeLength, nbGridX, nbGridY);
        drawFood(food, landmarkUnitX, landmarkUnitY);
        drawSnake(snake, snakeLength, landmarkUnitX, landmarkUnitY);
      }
    } else if (playState == PAUSED) {
      DrawText("Paused", screenWidth / 2 - 50, screenHeight / 2, 20, BLUE);
      switch (keyPressed) {
      case KEY_SPACE:
        playState = RUNNING;
        break;
      }
    } else if (playState == GAMEOVER) {
      DrawText("Game Over", screenWidth / 2 - 50, screenHeight / 2, 20, RED);
      switch (keyPressed) {
      case KEY_SPACE:
        playState = RUNNING;
        free(snake);
        snake = malloc(nbGridX * nbGridY * sizeof(Vector2));
        snake[0] = (Vector2){0, 0};
        snakeLength = 1;
        free(food);
        food = (Vector2 *)malloc(sizeof(Vector2));
        *food = createFood(snake, snakeLength, nbGridX, nbGridY);
        direction = RIGHT;
        continue;
        break;
      }
    } else if (playState == FINISHED) {
      DrawText("Finished", screenWidth / 2 - 50, screenHeight / 2, 20, GREEN);
      switch (keyPressed) {
      case KEY_SPACE:
        playState = RUNNING;
        free(snake);
        snake = malloc(nbGridX * nbGridY * sizeof(Vector2));
        snake[0] = (Vector2){0, 0};
        snakeLength = 1;
        free(food);
        food = (Vector2 *)malloc(sizeof(Vector2));
        *food = createFood(snake, snakeLength, nbGridX, nbGridY);
        direction = RIGHT;
        continue;
        break;
      }
    }

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, RED);

    EndDrawing();
  }

  free(snake);
  free(food);
  CloseWindow();

  return 0;
}

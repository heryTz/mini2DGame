#include "include/raylib.h"
#include <math.h>
#include <stdlib.h>

float vectorDistance(Vector2 a, Vector2 b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int main() {
  const int defaultScreenWidth = 800;
  const int defaultScreenHeight = 450;

  InitWindow(defaultScreenWidth, defaultScreenHeight,
             "raylib [core] example - basic window");

  SetTargetFPS(60);

  int screenHeight = GetScreenHeight();
  int screenWidth = GetScreenWidth();
  int radius = 10.0;
  Vector2 ball = {radius, radius};
  int ballSpeed = 10;
  Vector2 ballDirection = {ballSpeed, ballSpeed};

  int ballPathsSize = 10;
  Vector2 *ballPaths = (Vector2 *)malloc(ballPathsSize * sizeof(Vector2));
  int ballPathsCount = 0;
  ballPaths[ballPathsCount] = ball;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    screenHeight = GetScreenHeight();
    screenWidth = GetScreenWidth();

    Vector2 ballNextPos = {ball.x + ballDirection.x, ball.y + ballDirection.y};

    if (ballNextPos.y > screenHeight - radius) {
      if (ballDirection.x > 0 && ballDirection.y > 0) {
        ballDirection = (Vector2){ballSpeed, -ballSpeed};
      } else {
        ballDirection = (Vector2){-ballSpeed, -ballSpeed};
      }
    } else if (ballNextPos.x > screenWidth - radius) {
      if (ballDirection.x > 0 && ballDirection.y < 0) {
        ballDirection = (Vector2){-ballSpeed, -ballSpeed};
      } else {
        ballDirection = (Vector2){-ballSpeed, ballSpeed};
      }
    } else if (ballNextPos.y < radius) {
      if (ballDirection.x < 0 && ballDirection.y < 0) {
        ballDirection = (Vector2){-ballSpeed, ballSpeed};
      } else {
        ballDirection = (Vector2){ballSpeed, ballSpeed};
      }
    } else if (ballNextPos.x < radius) {
      if (ballDirection.x < 0 && ballDirection.y > 0) {
        ballDirection = (Vector2){ballSpeed, ballSpeed};
      } else {
        ballDirection = (Vector2){ballSpeed, -ballSpeed};
      }
    }

    ballNextPos = (Vector2){ball.x + ballDirection.x, ball.y + ballDirection.y};

    if (ballPathsCount + 1 > ballPathsSize) {
      ballPathsSize *= 2;
      ballPaths =
          (Vector2 *)realloc(ballPaths, ballPathsSize * sizeof(Vector2));
    }

    ballPathsCount += 1;
    ballPaths[ballPathsCount] = ballNextPos;
    if (ballPathsCount > 1) {
      for (int i = 0; i < ballPathsCount; i++) {
        DrawLine(ballPaths[i].x, ballPaths[i].y, ballPaths[i + 1].x,
                 ballPaths[i + 1].y, RED);
      }
    }

    ball = ballNextPos;
    DrawCircle(ball.x, ball.y, radius, RED);

    EndDrawing();
  }

  free(ballPaths);
  CloseWindow();

  return 0;
}

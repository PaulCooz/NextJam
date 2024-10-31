#include "level.h"
#include "settings.h"
#include "vmath.h"
#include <algorithm>
#include <raylib.h>
#include <stdio.h>

int main() {
  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
  InitWindow(377, 610, "Next Jam");

  font = LoadFont("res/monogram-extended.ttf");

  auto level = new Level();
  level->width = 10;
  level->height = 10;

  struct VisualRect {
    Rectangle rect;
    Color color;
  };
  VisualRect grid[10][10];
  VisualRect steps[3];

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(ClrBack);

      float screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();

      {
        float w = screenWidth * 0.6, h = screenHeight * 0.7;
        float x = 0, y = 0;
        DrawRectangle(x, y, w, h, ClrA);
        int gap = 3;
        int itemWidth = (w - gap * (level->width + 1.0)) / (float)level->width,
            itemHeight = (h - gap * (level->height + 1.0)) / (float)level->height;
        for (int i = 0; i < level->height; i++) {
          for (int j = 0; j < level->width; j++) {
            float posX = x + itemWidth * i + gap * (i + 1);
            float posY = y + itemHeight * j + gap * (j + 1);
            grid[i][j].rect.x = posX;
            grid[i][j].rect.y = posY;
            grid[i][j].rect.width = itemWidth;
            grid[i][j].rect.height = itemHeight;
            grid[i][j].color = ClrC;
          }
        }

        for (int i = 0; i < level->height; i++) {
          for (int j = 0; j < level->width; j++) {
            auto item = grid[i][j];
            DrawRectangle(item.rect.x, item.rect.y, item.rect.width, item.rect.height, item.color);
          }
        }
      }

      {
        float w = screenWidth * 0.4, h = screenHeight * 0.7;
        float x = screenWidth * 0.6, y = 0;
        DrawRectangle(x, y, w, h, ClrC);

        int gap = 3;
        int itemWidth = (w - gap * (1 + 1.0)) / (float)1, itemHeight = (h - gap * (3 + 1.0)) / (float)3;
        for (int j = 0; j < 3; j++) {
          steps[j].rect.x = x + itemWidth * 0 + gap * (0 + 1);
          steps[j].rect.y = y + itemHeight * j + gap * (j + 1);
          steps[j].rect.width = itemWidth;
          steps[j].rect.height = itemHeight;
          steps[j].color = {.r = 100, .g = (unsigned char)(0 * 10), .b = (unsigned char)(j * 10), .a = 255};
        }

        for (int j = 0; j < 3; j++) {
          auto item = steps[j];
          DrawRectangle(item.rect.x, item.rect.y, item.rect.width, item.rect.height, item.color);
        }
      }

      { DrawRectangle(0, screenHeight * 0.7, screenWidth, screenHeight * 0.3, ClrB); }

      Vector2 deltaMove;
      deltaMove.x = deltaMove.y = 0;
      float delta = GetFrameTime();
      if (IsKeyDown(KEY_W))
        deltaMove.y -= delta;
      if (IsKeyDown(KEY_A))
        deltaMove.x -= delta;
      if (IsKeyDown(KEY_S))
        deltaMove.y += delta;
      if (IsKeyDown(KEY_D))
        deltaMove.x += delta;

      deltaMove = Mult(Normalize(deltaMove), 10);

      static float x = 0, y = 0;
      x += deltaMove.x;
      y += deltaMove.y;
      DrawRectangle(x, y, 50, 50, {.r = 255, .g = 66, .b = 55, .a = 255});

      DrawFPS(0, 0);
    }
    EndDrawing();
  }
  delete level;

  CloseWindow();

  return 0;
}

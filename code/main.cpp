#include "colors.h"
#include "raylib.h"
#include "vmath.h"
#include <stdio.h>
#include <visual_node.h>

int main() {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(610, 987, "raylib [core] example - basic window");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(ColorC);

      auto node = VisualNode::FromFile("res/ui.xml");
      node->RenderTree();

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

  CloseWindow();

  return 0;
}

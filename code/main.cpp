#include "level.h"
#include "settings.h"
#include "visual_node.h"
#include "vmath.h"
#include <raylib.h>
#include <stdio.h>

int main() {
  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
  InitWindow(377, 610, "Next Jam");

  font = LoadFont("res/monogram-extended.ttf");

  auto tree = VisualNode::FromFile("res/ui.xml");

  auto gridNode = tree->FindByName("grid");

  auto visual = VisualNode::FromFile("res/grid_block.xml");
  visual->color = ClrC;
  gridNode->AddChild(visual);

  visual = VisualNode::FromFile("res/grid_block.xml");
  visual->color = ClrE;
  gridNode->AddChild(visual);

  auto level = new Level();
  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(ClrBack);

      tree->RenderTree(GetScreenWidth(), GetScreenHeight());

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
  delete tree;

  CloseWindow();

  return 0;
}

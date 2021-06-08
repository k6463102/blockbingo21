#ifndef ETRC21_GAME_DISP_H_
#define ETRC21_GAME_DISP_H_

#include "game_info.h"

class BingoShow {
public:
  BingoShow(BingoArea* bingo_area);
  void Draw();

private:
  void DrawLinks();
  void DrawCircles();
  void DrawBlocks();
  void DrawRobot();
  BingoArea* bingo_area_;
};

#endif // ETRC21_GAME_DISP_H_

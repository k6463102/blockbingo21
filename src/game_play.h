#ifndef ETRC21_GAME_PLAY_H_
#define ETRC21_GAME_PLAY_H_

#include "game_info.h"

#include <vector>
#include <string>
using namespace std;

class MovingInfo {
public:
  MovingInfo(BingoArea* bingo_area);
  vector<DrivingParam> GetDrivingParam(string moving_route);

private:
  float CalcDtheta(float dx, float dy);
  Color ConvertCharToColor(char circle_color);
  BingoArea* bingo_area_;
};

class RouteSearch {
public:
  RouteSearch(BingoArea* bingo_area);
  string GetMovingRoute(Circle* circle);

private:
  void ResetCircleInfo();
  BingoArea* bingo_area_;
};

class BlockDecision {
public:
  BlockDecision(BingoArea* bingo_area);
  Block* NextCarryBlock();

private:
  BingoArea* bingo_area_;
};

class BingoAgent {
public:
  BingoAgent(BingoArea* bingo_area, BlockDecision* block_decision, RouteSearch* route_search, MovingInfo* moving_info);
  void InitBlockPos();
  void InitRobotPos();
  void SolveBlockBingo();

private:
  BingoArea* bingo_area_;
  BlockDecision* block_decision_;
  RouteSearch* route_search_;
  MovingInfo* moving_info_;
};

#endif // ETRC21_GAME_PLAY_H_

#ifndef ETRC21_GAME_INFO_H_
#define ETRC21_GAME_INFO_H_

#include <cstdint>
#include <vector>

enum BlockId {
  kR1 = 0,
  kR2,
  kG1,
  kG2,
  kB1,
  kB2,
  kY1,
  kY2,
  kK1,
  kInvalidBlockId,
};

struct Block;

struct Circle {
  char id;
  float x;
  float y;
  char color;
  std::vector<Circle*> next;
  Block* block;
  int cost;
  Circle* prev;
  bool is_fixed;
};

struct Block {
  BlockId id;
  char color;
  Circle* circle;
  bool is_carried;
};

struct Robot {
  float x;
  float y;
  float theta;
  Circle* circle;
};

struct DrivingParam {
  int a;
};

class BingoArea {
public:
  BingoArea(bool is_Rcourse);
  ~BingoArea();
  void SetBlockDefaultPos(BlockId block_id, char circle_id);
  void SetRobotDefaultTheta(float theta);
  Circle* FindCircle(char circle_id);
  std::vector<Circle> circles_;
  std::vector<Block> blocks_;
  Robot* robot_;

private:
  void InitCircles();
  void InitBlocks();
  void InitRobot();
  bool is_Rcourse_;
};

#endif // ETRC21_GAME_INFO_H_

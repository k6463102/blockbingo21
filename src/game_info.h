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

enum Direction {
  kNorth = 0,
  kSouth,
  kEast,
  kWest,
  kNorthEast,
  kSouthEast,
  kNorthWest,
  kSouthWest,
  kInvalidDirection,
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
  Circle* target;
  bool is_carried;
};

struct Robot {
  float x;
  float y;
  float theta;
  Circle* circle;
};

enum Move {
  kTraceLeftEdge = 0,
  kTraceRightEdge,
  kGoForward,
  kGoBackward,
  kRotateLeft,
  kRotateRight,
  kInvalidMove,
  kMoveNum
};

struct Gain {
  float kp;
  float ki;
  float kd;
};

enum End {
  kColorEnd = 0,
  kDistanceEnd,
  kThetaEnd,
  kIvalidEnd,
  kEndNum
};

enum Color {
  kGreen = 0,
  kBlack,
  kRed,
  kYellow,
  kBlue,
  kWhite,
  kInvalidColor,
  kColorNum
};

struct DrivingParam {
  Move move_type;
  int8_t ref_power;
  float ref_value;
  Gain gain;
  End end_type;
  Color end_color;
  float end_threshold;
  bool is_started;
  bool is_finished;
};

class BingoArea {
public:
  BingoArea(bool is_Rcourse);
  ~BingoArea();
  void SetBlockDefaultPos(BlockId block_id, char circle_id);
  void SetRobotDefaultTheta(float theta);
  Circle* FindCircle(char circle_id);
  void DecideTargetOfBlock();
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

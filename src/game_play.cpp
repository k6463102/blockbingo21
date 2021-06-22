#include "game_play.h"

#include <limits>

const float PI = 3.14159;
const float kDecisionCoeff = 1.5;
const int kMax = numeric_limits<int>::max();
const float kInf = numeric_limits<float>::infinity();

MovingInfo::MovingInfo(BingoArea* bingo_area)
    : bingo_area_(bingo_area) {
}

vector<DrivingParam> MovingInfo::GetDrivingParam(string moving_route) {
  vector<DrivingParam> v;

  for (size_t i = 0; i < moving_route.size() - 1; ++i) {
    Circle* curr_circle = bingo_area_->FindCircle(moving_route[i]);
    Circle* next_circle = bingo_area_->FindCircle(moving_route[i + 1]);

    if (curr_circle == NULL || next_circle == NULL)
      break;

    float dx = next_circle->x - curr_circle->x;
    float dy = next_circle->y - curr_circle->y;
    float dtheta = CalcDtheta(dx, dy);

    DrivingParam p1 = { kGoForward, 10, 0, { }, kDistanceEnd, kInvalidColor, 50 };
    v.push_back(p1);

    if (dtheta > 0) {
      DrivingParam p2 = { kRotateLeft, 5, 0, { }, kThetaEnd, kInvalidColor, dtheta };
      v.push_back(p2);
    } else if (dtheta < 0) {
      DrivingParam p2 = { kRotateRight, 5, 0, { }, kThetaEnd, kInvalidColor, dtheta };;
      v.push_back(p2);
    }

    DrivingParam p3 = { kGoForward, 10, 0, { }, kDistanceEnd, kInvalidColor, 50 };
    v.push_back(p3);

    Color color = ConvertCharToColor(next_circle->color);
    DrivingParam p4 = { kTraceRightEdge, 30, 50, { 0.4, 0, 0.05 }, kColorEnd, color, 0 };
    v.push_back(p4);

    Robot* robot = bingo_area_->robot_;
    robot->theta += dtheta;
    robot->x = next_circle->x;
    robot->y = next_circle->y;
    robot->circle = next_circle;
  }

  return v;
}

float MovingInfo::CalcDtheta(float dx, float dy) {
  Robot* robot = bingo_area_->robot_;
  Direction robot_dir = kInvalidDirection;
  if (robot->theta == 0) {
    robot_dir = kEast;
  } else if (robot->theta == PI/2) {
    robot_dir = kNorth;
  } else if (robot->theta == PI || robot->theta == -PI) {
    robot_dir = kWest;
  } else if (robot->theta == -PI/2) {
    robot_dir = kSouth;
  }

  float dtheta = 0;
  if (dy > 0) {
    switch (robot_dir) {
      case kEast: dtheta = PI/2; break;
      case kWest: dtheta = -PI/2; break;
      case kSouth: dtheta = PI; break;
      default: break;
    }
  } else if (dy < 0) {
    switch (robot_dir) {
      case kEast: dtheta = -PI/2; break;
      case kWest: dtheta = PI/2; break;
      case kNorth: dtheta = -PI; break;
      default: break;
    }
  }
  else if (dx > 0) {
    switch (robot_dir) {
      case kNorth: dtheta = -PI/2; break;
      case kSouth: dtheta = PI/2; break;
      case kWest: dtheta = -PI; break;
      default: break;
    }
  }
  else if (dx < 0) {
    switch (robot_dir) {
      case kNorth: dtheta = PI/2; break;
      case kSouth: dtheta = -PI/2; break;
      case kEast: dtheta = PI; break;
      default: break;
    }
  }
  return dtheta;
}

Color MovingInfo::ConvertCharToColor(char circle_color) {
  Color color = kInvalidColor;

  switch (circle_color) {
    case 'R': color = kRed; break;
    case 'G': color = kGreen; break;
    case 'B': color = kBlue; break;
    case 'Y': color = kYellow; break;
    default: break;
  }

  return color;
}

RouteSearch::RouteSearch(BingoArea* bingo_area)
    : bingo_area_(bingo_area) {
}

string RouteSearch::GetMovingRoute(Circle* circle) {
  ResetCircleInfo();

  Circle* curr_circle = circle;
  curr_circle->cost = 0;
  size_t i = 0;
  size_t cost = 1;
  bool dead_end = true;
  Robot* robot = bingo_area_->robot_;

  while (true) {
    vector<Circle*>& next_circle = curr_circle->next;
    if (next_circle[i]->cost == -1 || next_circle[i]->id == robot->circle->id) {
      next_circle[i]->cost = cost;
      next_circle[i]->prev = curr_circle;
      dead_end = false;
      if (next_circle[i]->id == robot->circle->id)
        break;
    }

    ++i;
    if (i >= curr_circle->next.size()) {
      curr_circle->is_fixed = true;
      if (dead_end) {
        curr_circle = curr_circle->prev;
        --cost;
      }

      for (size_t j = 0; j < curr_circle->next.size(); ++j) {
        if (!curr_circle->next[j]->is_fixed && curr_circle->next[j]->cost != kMax) {
          curr_circle = curr_circle->next[j];
          i = 0;
          ++cost;
          dead_end = true;
          break;
        }
      }
    }
  }

  string s = "";
  curr_circle = robot->circle;
  while (true) {
    s += { curr_circle->id };
    curr_circle = curr_circle->prev;
    if (curr_circle == circle) {
      s += { curr_circle->id };
      break;
    }
  }
  return s;
}

void RouteSearch::ResetCircleInfo() {
  vector<Circle>& circles = bingo_area_->circles_;
  vector<Circle>::iterator c;
  for (c = circles.begin(); c != circles.end(); ++c) {
    c->prev = NULL;
    c->is_fixed = false;

    if ('1' <= c->id && c->id <= '9') {
      c->cost = kMax;
    } else if (c->block != NULL) {
      c->cost = kMax;
    } else {
      c->cost = -1;
    }
  }
}

BlockDecision::BlockDecision(BingoArea* bingo_area)
    : bingo_area_(bingo_area) {
}

Block* BlockDecision::NextCarryBlock() {
  Block* block = NULL;
  Robot* robot = bingo_area_->robot_;

  float min = kInf;
  vector<Block>& blocks = bingo_area_->blocks_;
  vector<Block>::iterator b;
  for (b = blocks.begin(); b != blocks.end(); ++b) {
    if (b->is_carried)
      continue;

    float x1 = robot->x;
    float y1 = robot->y;
    float x2 = b->circle->x;
    float y2 = b->circle->y;

    float d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    if (x1 != x2 && y1 != y2)
      d *= kDecisionCoeff;

    if (min > d) {
      block = &*b;
      min = d;
    }
  }

  return block;
}

BingoAgent::BingoAgent(BingoArea* bingo_area, BlockDecision* block_decision, RouteSearch* route_search, MovingInfo* moving_info)
    : bingo_area_(bingo_area), block_decision_(block_decision),
      route_search_(route_search), moving_info_(moving_info) {
}

void BingoAgent::InitBlockPos() {
  bingo_area_->SetBlockDefaultPos(kY1, 'B');
  bingo_area_->SetBlockDefaultPos(kB1, 'D');
  bingo_area_->SetBlockDefaultPos(kR1, 'E');
  bingo_area_->SetBlockDefaultPos(kG1, 'G');
  bingo_area_->SetBlockDefaultPos(kY2, 'K');
  bingo_area_->SetBlockDefaultPos(kR2, 'M');
  bingo_area_->SetBlockDefaultPos(kG2, 'P');
  bingo_area_->SetBlockDefaultPos(kB2, 'R');
  bingo_area_->SetBlockDefaultPos(kK1, '0');

  bingo_area_->DecideTargetOfBlock();
}

void BingoAgent::InitRobotPos() {
  bingo_area_->SetRobotDefaultTheta(PI);
}

void BingoAgent::SolveBlockBingo() {
  while (true) {
    Block* block = block_decision_->NextCarryBlock();
    if (block == NULL)
      break;

    string moving_route = route_search_->GetMovingRoute(block->circle);
    moving_info_->GetDrivingParam(moving_route);

    string carry_route = route_search_->GetMovingRoute(block->target);
    moving_info_->GetDrivingParam(carry_route);

    break;
  }
}

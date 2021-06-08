#include "game_play.h"

#include <limits>

const double PI = 3.14159;
const float kDecisionCoeff = 1.5;
const int kMax = numeric_limits<int>::max();
const float kInf = numeric_limits<float>::infinity();

MovingInfo::MovingInfo(BingoArea* bingo_area)
    : bingo_area_(bingo_area) {
}

vector<DrivingParam> MovingInfo::GetDrivingParam(string moving_route) {
  // ロボットの姿勢を取得する

  // 開始地点から次の地点へ移動する方向を確認し
  // ロボットの回転が必要ならまずその回転をパラメータに追加する

  // 開始地点のサークルを抜ける前進をパラメータに追加する

  // 目的地点の色を見つけるまでライントレースするのでそれをパラメータに追加する
  vector<DrivingParam> v;
  return v;
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
    if (next_circle[i]->cost == -1) {
      next_circle[i]->cost = cost;
      next_circle[i]->prev = curr_circle;
      dead_end = false;
      if (next_circle[i]->id == robot->circle_->id)
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
        if (!curr_circle->next[j]->is_fixed) {
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
  curr_circle = robot->circle_;
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

    if (c->block == NULL) {
      c->cost = -1;
    } else {
      c->cost = kMax;
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

    float x1 = robot->x_;
    float y1 = robot->y_;
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
}

void BingoAgent::InitRobotPos() {
  bingo_area_->SetRobotDefaultTheta(static_cast<float>(PI / 2));
}

void BingoAgent::SolveBlockBingo() {
  while (true) {
    Block* block = block_decision_->NextCarryBlock();
    if (block == NULL)
      break;

    string moving_route = route_search_->GetMovingRoute(block->circle);
    moving_info_->GetDrivingParam(moving_route);

    break;
  }
}

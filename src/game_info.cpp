#include "game_info.h"

#include "settings.h"

BingoArea::BingoArea(bool is_Rcourse) : is_Rcourse_(is_Rcourse) {
  InitCircles();
  InitBlocks();
  InitRobot();
}

BingoArea::~BingoArea() {
  if (robot_ != NULL)
    delete robot_;
}

void BingoArea::SetBlockDefaultPos(BlockId block_id, char circle_id) {
  using namespace std;
  vector<Block>::iterator b;
  for (b = blocks_.begin(); b != blocks_.end(); ++b) {
    if (b->id != block_id)
      continue;

    vector<Circle>::iterator c;
    for (c = circles_.begin(); c != circles_.end(); ++c) {
      if (c->id == circle_id) {
        b->circle = &*c;
        c->block = &*b;
        return;
      }
    }
  }
}

void BingoArea::SetRobotDefaultTheta(float theta) {
  robot_->theta = theta;
}

Circle* BingoArea::FindCircle(char circle_id) {
  using namespace std;
  vector<Circle>::iterator c;
  for (c = circles_.begin(); c != circles_.end(); ++c) {
    if (c->id == circle_id)
      return &*c;
  }
  return NULL;
}

void BingoArea::InitCircles() {
  char id, color;
  float x, y;

  for (int i = 0; i < kCircleNum; ++i) {
    if (is_Rcourse_) {
      sscanf(kRcourseCircleData[i], "%c,%f,%f,%c", &id, &x, &y, &color);
    }
    else {
      sscanf(kLcourseCircleData[i], "%c,%f,%f,%c", &id, &x, &y, &color);
    }
    Circle circle = { id, x, y, color };
    circles_.push_back(circle);
  }

  using namespace std;
  vector<Circle>::iterator c1;
  vector<Circle>::iterator c2;
  for (c1 = circles_.begin(); c1 != circles_.end(); ++c1) {
    for (c2 = circles_.begin(); c2 != circles_.end(); ++c2) {
      if (c1 == c2)
        continue;

      if ('1' <= c1->id && c1->id <= '9' || '1' <= c2->id && c2->id <= '9') {
        if(sqrt(pow(c1->x - c2->x, 2) + pow(c1->y - c2->y, 2)) <= 3)
          c1->next.push_back(&*c2);

      } else {
        if (c1->x == c2->x && abs(c1->y - c2->y) <= 7)
          c1->next.push_back(&*c2);

        if (c1->y == c2->y && abs(c1->x - c2->x) <= 7)
          c1->next.push_back(&*c2);
      }
    }
  }
}

void BingoArea::InitBlocks() {
  int id;
  char color;

  for (int i = 0; i < kBlockNum; ++i) {
    sscanf(kBlockData[i], "%d,%c", &id, &color);
    Block block = { static_cast<BlockId>(id), color };
    blocks_.push_back(block);
  }
}

void BingoArea::InitRobot() {
  robot_ = NULL;

  char circle_id;
  if (is_Rcourse_) {
    sscanf(kRcourseRobotData, "%c", &circle_id);
  }
  else {
    sscanf(kLcourseRobotData, "%c", &circle_id);
  }

  using namespace std;
  vector<Circle>::iterator c;
  for (c = circles_.begin(); c != circles_.end(); ++c) {
    if (c->id == circle_id) {
      float x = static_cast<float>(c->x);
      float y = static_cast<float>(c->y);
      float theta = 0;
      robot_ = new Robot { x, y, theta, &*c };
      break;
    }
  }
}

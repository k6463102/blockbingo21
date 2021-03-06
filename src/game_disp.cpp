#include "game_disp.h"

#include "ofMain.h"

const ofColor kOfSilver(0xC0, 0xC0, 0xC0);
const ofColor kOfBlack(0x00, 0x00, 0x00);
const ofColor kOfRed(0xFF, 0x00, 0x00);
const ofColor kOfGreen(0x00, 0xFF, 0x00);
const ofColor kOfBlue(0x00, 0x00, 0xFF);
const ofColor kOfYellow(0xFF, 0xFF, 0x00);
const ofColor kOfRobotColor(0x00, 0x00, 0x00, 0xA0);
const ofColor kOfRobotInnerColor(0xC0, 0xC0, 0xC0, 0xA0);

const int kLinkLineWidth = 3;
const int kMagnification = 33;
const int kCircleRadius = 23;
const float kInnerCirclePct = 0.8;
const int kBlockRadius = 16;
const int kRobotRadius = 33;
const float kInnerRobotPct = 0.8;
const int kRobotLineWidth = 7;

BingoShow::BingoShow(BingoArea* bingo_area) : bingo_area_(bingo_area) {
}

void BingoShow::Draw() {
  ofTranslate(1024 / 2, 768 / 2);
  ofScale(1, -1);

  ofBackground(kOfSilver);

  DrawLinks();
  DrawCircles();
  DrawBlocks();
  DrawRobot();
}

void BingoShow::DrawLinks() {
  ofSetColor(kOfBlack);
  ofSetLineWidth(kLinkLineWidth);

  using namespace std;
  vector<Circle>& circles = bingo_area_->circles_;
  vector<Circle>::iterator c;
  for (c = circles.begin(); c != circles.end(); ++c) {
    int x1 = c->x * kMagnification;
    int y1 = c->y * kMagnification;

    for (size_t i = 0; i < c->next.size(); ++i) {
      int x2 = c->next[i]->x * kMagnification;
      int y2 = c->next[i]->y * kMagnification;
      ofDrawLine(x1, y1, x2, y2);
    }
  }
}

void BingoShow::DrawCircles() {
  using namespace std;
  vector<Circle>& circles = bingo_area_->circles_;
  vector<Circle>::iterator c;
  for (c = circles.begin(); c != circles.end(); ++c) {
    int x = c->x * kMagnification;
    int y = c->y * kMagnification;

    switch (c->color) {
      case 'R':
        ofSetColor(kOfRed);
        break;
      case 'G':
        ofSetColor(kOfGreen);
        break;
      case 'B':
        ofSetColor(kOfBlue);
        break;
      case 'Y':
        ofSetColor(kOfYellow);
        break;
      default:
        continue;
    }
    ofDrawCircle(x, y, kCircleRadius);
    ofSetColor(kOfSilver);
    ofDrawCircle(x, y, kCircleRadius * kInnerCirclePct);
  }
}

void BingoShow::DrawBlocks() {
  using namespace std;
  vector<Block>& blocks = bingo_area_->blocks_;
  vector<Block>::iterator b;
  for (b = blocks.begin(); b != blocks.end(); ++b) {
    const Circle* circle = b->circle;
    if (circle == NULL)
      continue;

    int x = circle->x * kMagnification;
    int y = circle->y * kMagnification;

    switch (b->color) {
      case 'R':
        ofSetColor(kOfRed);
        break;
      case 'G':
        ofSetColor(kOfGreen);
        break;
      case 'B':
        ofSetColor(kOfBlue);
        break;
      case 'Y':
        ofSetColor(kOfYellow);
        break;
      case 'K':
        ofSetColor(kOfBlack);
        break;
      default:
        continue;
    }
    ofDrawCircle(x, y, kBlockRadius);
  }
}

void BingoShow::DrawRobot() {
  const Robot* robot = bingo_area_->robot_;

  float x = robot->x * kMagnification;
  float y = robot->y * kMagnification;
  float theta = robot->theta;

  ofSetColor(kOfRobotColor);
  ofDrawCircle(x, y, kRobotRadius);

  ofSetColor(kOfRobotInnerColor);
  ofDrawCircle(x, y, kRobotRadius * kInnerRobotPct);

  ofSetColor(kOfRobotColor);
  ofSetLineWidth(kRobotLineWidth);
  ofDrawLine(x, y, x + kRobotRadius * cos(theta), y + kRobotRadius * sin(theta));
}

#pragma once

#include "ofMain.h"

#include "game_info.h"
#include "game_disp.h"
#include "game_play.h"

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();
  void exit();
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  BingoArea* bingo_area_;
  BingoShow* bingo_show_;
  BlockDecision* block_decision_;
  RouteSearch* route_search_;
  MovingInfo* moving_info_;
  BingoAgent* bingo_agent_;
};

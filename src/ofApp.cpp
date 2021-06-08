#include "ofApp.h"

void ofApp::setup() {
  bool kRcourse = false;
  bingo_area_ = new BingoArea(kRcourse);
  bingo_show_ = new BingoShow(bingo_area_);
  block_decision_ = new BlockDecision(bingo_area_);
  route_search_ = new RouteSearch(bingo_area_);
  moving_info_ = new MovingInfo(bingo_area_);
  bingo_agent_ = new BingoAgent(bingo_area_, block_decision_, route_search_, moving_info_);

  bingo_agent_->InitBlockPos();
  bingo_agent_->InitRobotPos();
  bingo_agent_->SolveBlockBingo();
}

void ofApp::update() {
}

void ofApp::draw() {
  bingo_show_->Draw();
}

void ofApp::exit() {
  delete bingo_agent_;
  delete moving_info_;
  delete route_search_;
  delete block_decision_;
  delete bingo_show_;
  delete bingo_area_;
}

void ofApp::keyPressed(int key) {}

void ofApp::keyReleased(int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::mouseEntered(int x, int y) {}

void ofApp::mouseExited(int x, int y) {}

void ofApp::windowResized(int w, int h) {}

void ofApp::gotMessage(ofMessage msg) {}

void ofApp::dragEvent(ofDragInfo dragInfo) {}

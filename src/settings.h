#ifndef ETRC21_SETTINGS_H_
#define ETRC21_SETTINGS_H_

const int kCircleNum = 28;
const int kBlockNum = 9;

const char* kLcourseCircleData[kCircleNum] = {
  "A,-6,6,R",
  "B,-2,6,R",
  "C,2,6,B",
  "D,6,6,B",
  "E,-6,2,R",
  "F,-2,2,R",
  "G,2,2,B",
  "H,6,2,B",
  "J,-6,-2,Y",
  "K,-2,-2,Y",
  "L,2,-2,G",
  "M,6,-2,G",
  "P,-6,-6,Y",
  "Q,-2,-6,Y",
  "R,2,-6,G",
  "S,6,-6,G",
  "W,-9,-10,G",
  "X,-2,-10,Y",
  "1,-4,4,Y",
  "2,0,4,G",
  "3,4,4,R",
  "4,-4,0,B",
  "5,4,0,Y",
  "6,-4,-4,G",
  "7,0,-4,R",
  "8,4,-4,B",
  "9,0,0,N",
  "0,-9,-5,R",
};

const char* kRcourseCircleData[] = {
  "",
};

const char* kBlockData[kBlockNum] = {
  "0,R",
  "1,R",
  "2,G",
  "3,G",
  "4,B",
  "5,B",
  "6,Y",
  "7,Y",
  "8,K",
};

const char* kLcourseRobotData = "X";

const char* kRcourseRobotData = "";

#endif  // ETRC21_SETTINGS_H_

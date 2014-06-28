#include "iodefine.h"
#include "MainFrame/parameters.h"
#include "MainFrame/ToolBox.cpp"
#include "MainFrame/Body.cpp"
#include "MainFrame/Brain.cpp"

class mainFrame {
public:
  body controller;
  brain thinker;
  void run();
};

void mainFrame::run() {
  int *actions;
  int *tmp;
  int size;
  int maximum_size;
  while(1) {
    size = 0;
    maximum_size = 10;
    actions = new int [maximum_size];
    do {
      actions[size] = thinker.next_action();
      size++;
      if (size == maximum_size) { //Dynamically allocate memory because of the SH7125's too little RAM.
        tmp = new int [size];
        for (int i=0;i<size;i++) {
          tmp[i] = actions[i];
        }
        delete [] actions;
        maximum_size += 10;
        actions = new int [maximum_size];
        for (int i=0;i<maximum_size;i++) {
          actions[i] = tmp[i];
        }
        delete [] tmp;
      }
    } while (actions[size-1]!=SCAN && actions[size-1]!=IDLE);
    controller.execute(actions);
    if (actions[size-1]==SCAN) {
      thinker.update_memorized_maze(controller.scanner);
    }
    else if (actions[size-1]==IDLE) {
      thinker.init_as_solve_mode();
      break; //Successfully returned to the starting point.
    }
    delete [] actions;
  }
}

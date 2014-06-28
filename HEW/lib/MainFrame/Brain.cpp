#include "Brain/Wall.cpp"
#include "Brain/Route.cpp"
#include "Brain/Strategy/Search.cpp"
#include "Brain/Strategy/Solve.cpp"

class brain {
  unsigned short int mode;
  search searcher;
  solve solver;
  wall maze_memorizer;
  route route_memorizer;
  unsigned short int position[2];
  unsigned short int direction;
public:
  brain();
  unsigned short int next_action();
  void update_memorized_maze ( adc &adc_result );
  void init_as_solve_mode();
};

brain::brain () : maze_memorizer( MAZE ) {
  mode = SEARCH;
  position[0] = 15;
  position[1] = 0;
  direction = UP;
  route_memorizer.update_visited_places( position );
}

unsigned short int brain::next_action() {
  unsigned short int next_action;
  if ( position[0] == 15 && position[1] == 0 && direction == DOWN ) {
    return IDLE;
  }
  if ( mode == SEARCH ) {
    next_action = searcher.next_action( maze_memorizer, route_memorizer, position, direction );
  }
  else if ( mode == SOLVE ) {
    next_action = solver.next_action( maze_memorizer, route_memorizer, position, direction );
  }
  if ( next_action != SCAN ) {
    route_memorizer.update_status( position, direction, next_action );
  }
  route_memorizer.update_visited_places( position );
  return next_action;
}

void brain::update_memorized_maze ( adc &adc_result ) {
  maze_memorizer.update( position, direction, FRONT, adc_result.wall.front );
  maze_memorizer.update( position, direction, RIGHT, adc_result.wall.right );
  maze_memorizer.update( position, direction, LEFT,  adc_result.wall.left );
}

void brain::init_as_solve_mode() {
  if ( mode == SEARCH ) {
    mode = SOLVE;
  }
  else {
    solver.reset();
  }
  position[0] = 15;
  position[1] = 0;
  direction = UP;
  route_memorizer.reset();
  route_memorizer.update_visited_places( position );
}

#include "Solve/Normal.cpp"
#include "Solve/Reverse.cpp"

class solve {
  unsigned short int mode;
  normal_s _normal;
  reverse_s _reverse;
  void map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction );
public:
  solve();
  unsigned short int next_action ( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction );
  void reset();
};

solve::solve() {
  mode = NORMAL;
}

void solve::reset() {
  mode = NORMAL;
  _normal.is_done = 0;
  _reverse.is_done = 0;
}

void solve::map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction ) {
  while ( !route_memorizer.has_potential_map ) {
    if ( mode == NORMAL ) {
      _normal.map_potentials( maze_memorizer, route_memorizer, position, direction );
      if ( _normal.is_done ) {
        mode = REVERSE;
        _normal.is_done = 0;
      }
    }
    else if ( mode == REVERSE ) {
      _reverse.map_potentials( maze_memorizer, route_memorizer, position, direction );
      if ( _reverse.is_done ) {
        mode = NORMAL;
        _reverse.is_done = 0;
      }
    }
  }
}

unsigned short int solve::next_action( wall &maze_memorizer, route &route_memorizer,
                                       unsigned short int position[], unsigned short int &direction ) {
  if ( !route_memorizer.has_potential_map ) {
    map_potentials( maze_memorizer, route_memorizer, position, direction );
  }
  if ( (position[0] == GOAL_I_MIN || position[0] == GOAL_I_MAX) && (position[1] == GOAL_J_MIN || position[1] == GOAL_J_MAX) ) {
    if ( mode == NORMAL ) {
      mode = REVERSE;
      return GO_BACKWARD;
    }
  }
  if ( mode == NORMAL ) {
    route_memorizer.put_a_sign_to_reverse( position, direction );
    return route_memorizer.get_the_direction_to_solve( position, direction );
  }
  else if ( mode == REVERSE ) {
    return route_memorizer.get_the_direction_to_reverse( position, direction );
  }
  return DUMMY;
}

#include "Search/Normal.cpp"
#include "Search/Reverse.cpp"

class search {
  unsigned short int mode;
  normal _normal;
  reverse _reverse;
  wall virtual_walls;
  route virtual_route;
  unsigned short int virtual_position[2];
  unsigned short int virtual_direction;
  unsigned short int aimed_position[2];
  unsigned short int aimed_direction;
  void map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int direction );
public:
  search();
  unsigned short int next_action ( wall &maze_memorizer, route &route_memorizer,unsigned short int position[], unsigned short int direction );
};

search::search() : virtual_walls( VIRTUAL ) {
  mode = NORMAL;
}

void search::map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int direction ) {
  unsigned short int next_action;
  virtual_position[0] = aimed_position[0] = position[0];
  virtual_position[1] = aimed_position[1] = position[1];
  virtual_direction = aimed_direction = direction;
  route_memorizer.update_status( aimed_position, aimed_direction, GO_BACKWARD );
  while( !_reverse.is_done && !(aimed_position[0] == 15 && aimed_position[1] == 0 && aimed_direction == DOWN) )  {
    next_action = _reverse.next_action( maze_memorizer, route_memorizer, aimed_position, aimed_direction, virtual_walls );
    route_memorizer.update_status( aimed_position, aimed_direction, next_action );
  }
  _reverse.is_done = 0;
  while ( !virtual_route.has_potential_map ) {
    if ( mode == NORMAL ) {
      _normal.map_potentials( maze_memorizer, virtual_route, virtual_position, virtual_direction, position, direction,
                              aimed_position, aimed_direction );
      if ( _normal.is_done ) {
        mode = REVERSE;
        _normal.is_done = 0;
      }
    }
    else if ( mode == REVERSE ) {
      _reverse.map_potentials( maze_memorizer, virtual_route, virtual_position, virtual_direction, position, direction,
                               aimed_position, aimed_direction );
      if ( _reverse.is_done ) {
        mode = NORMAL;
        _reverse.is_done = 0;
      }
    }
  }
}

unsigned short int search::next_action( wall &maze_memorizer, route &route_memorizer,
                                        unsigned short int position[], unsigned short int direction ) {
  unsigned short int next_action;
  if ( mode == REVERSE && virtual_route.get_the_direction_to_solve( position, direction ) == DUMMY ) {
    mode = NORMAL;
    virtual_route.has_potential_map = 0;
    virtual_route.reset_the_map_to_solve();
  }
  if ( mode == NORMAL ) {
    next_action = _normal.next_action( maze_memorizer, route_memorizer, position, direction, virtual_walls );
    if ( _normal.is_done ) {
      mode = REVERSE;
      _normal.is_done = 0;
    }
  }
  if ( mode == REVERSE ) {
    if ( !virtual_route.has_potential_map ) {
      mode = NORMAL;
      map_potentials( maze_memorizer, route_memorizer, position, direction );
      mode = REVERSE;
    }
    next_action = virtual_route.get_the_direction_to_solve( position, direction );
  }
  return next_action;
}

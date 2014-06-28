class normal_s {
public:
  unsigned short int is_done;
  normal_s();
  void map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction );
};

void normal_s::map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction ) {
  unsigned short int next_action = DUMMY;
  unsigned short int the_count_of_facing_unvisited_blocks = 0; //precisely wrong name
  unsigned short int potential_to_set = 0;
  if ( maze_memorizer.safe_to_go( position, direction, LEFT ) ) {
    if ( route_memorizer.visited( position, direction, LEFT ) ) {
      if ( route_memorizer.get_potential( position, direction, LEFT ) > route_memorizer.get_potential( position ) + route_memorizer.t ) {
        the_count_of_facing_unvisited_blocks++;
        next_action = GO_LEFTWARD;
        potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
      }
    }
    else {
      the_count_of_facing_unvisited_blocks++;
      next_action = GO_LEFTWARD;
      potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
    }
  }
  if ( maze_memorizer.safe_to_go( position, direction, UP ) ) {
    if ( route_memorizer.visited( position, direction, UP ) ) {
      if ( route_memorizer.get_potential( position, direction, UP ) > route_memorizer.get_potential( position ) + route_memorizer.s ) {
        the_count_of_facing_unvisited_blocks++;
        if ( next_action == DUMMY ) {
          next_action = GO_FORWARD;
          potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.s;
        }
      }
    }
    else {
      the_count_of_facing_unvisited_blocks++;
      if ( next_action == DUMMY ) {
        next_action = GO_FORWARD;
        potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.s;
      }
    }
  }
  if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
    if ( route_memorizer.visited( position, direction, RIGHT ) ) {
      if ( route_memorizer.get_potential( position, direction, RIGHT ) > route_memorizer.get_potential( position ) + route_memorizer.t ) {
        the_count_of_facing_unvisited_blocks++;
        if ( next_action == DUMMY ) {
          next_action = GO_RIGHTWARD;
          potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
        }
      }
    }
    else {
      the_count_of_facing_unvisited_blocks++;
      if ( next_action == DUMMY ) {
        next_action = GO_RIGHTWARD;
        potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
      }
    }
  }
  if ( next_action != DUMMY ) {
    route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_facing_unvisited_blocks - 1 );
    route_memorizer.update_status( position, direction, next_action );
    route_memorizer.put_a_sign_to_reverse( position, direction );
    route_memorizer.update_visited_places( position );
    route_memorizer.set_potential( position, potential_to_set );
  }
  else {
    route_memorizer.remove_a_sign_to_reverse( position );
    route_memorizer.update_status( position, direction, GO_BACKWARD );
    is_done = 1;
  }
}

normal_s::normal_s() {
  is_done = 0;
}

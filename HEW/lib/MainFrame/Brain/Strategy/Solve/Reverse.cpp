class reverse_s {
public:
  unsigned short int is_done;
  reverse_s();
  void map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction );
};

void reverse_s::map_potentials( wall &maze_memorizer, route &route_memorizer, unsigned short int position[], unsigned short int &direction ) {
  unsigned short int next_action = DUMMY;
  unsigned short int potential_to_set = 0;
  if ( position[0] == 15 && position[1] == 0 ) {
    route_memorizer.has_potential_map = 1;
    route_memorizer.solve_the_maze( maze_memorizer );
    route_memorizer.reset();
    direction = UP;
    is_done = 1;
  }
  else {
    if ( route_memorizer.signed_crossroad( position ) ) {
      route_memorizer.decrement_the_count_of_facing_unvisited_blocks( position );
      if ( maze_memorizer.safe_to_go( position, direction, LEFT ) ) {
        if ( route_memorizer.signed_crossroad( position ) ) {
          next_action = GO_LEFTWARD;
          potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.s;
        }
        else if ( route_memorizer.visited( position, direction, LEFT ) ) {
          if ( route_memorizer.get_potential( position, direction, LEFT ) > route_memorizer.get_potential( position ) + route_memorizer.s ) {
            next_action = GO_LEFTWARD;
            if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
              if ( route_memorizer.get_potential( position ) > route_memorizer.get_potential( position, direction, RIGHT ) ) {
                potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.s;
              }
              else {
                potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
              }
            }
            else {
              potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
            }
          }
        }
        else {
          next_action = GO_LEFTWARD;
          if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
            if ( route_memorizer.get_potential( position ) > route_memorizer.get_potential( position, direction, RIGHT ) ) {
              potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.s;
            }
            else {
              potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
            }
          }
          else {
            potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
          }
        }
      }
      if ( next_action == DUMMY ) {
        next_action = GO_FORWARD;
        potential_to_set = route_memorizer.get_potential( position ) + route_memorizer.t;
      }
      route_memorizer.update_status( position, direction, next_action );
      route_memorizer.put_a_sign_to_reverse( position, direction );
      route_memorizer.set_potential( position, potential_to_set );
      route_memorizer.update_visited_places( position );
      is_done = 1;
    }
    else {
      next_action = route_memorizer.get_the_direction_to_reverse( position, direction );
      route_memorizer.remove_a_sign_to_reverse( position );
      route_memorizer.update_status( position, direction, next_action );
    }
  }
}

reverse_s::reverse_s() {
  is_done = 0;
}

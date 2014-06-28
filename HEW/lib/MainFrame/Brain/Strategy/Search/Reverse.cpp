class reverse {
  unsigned short int virtual_position[2];
  unsigned short int virtual_direction;
public:
  unsigned short int is_done;
  reverse();
  unsigned short int next_action( wall &maze_memorizer, route &route_memorizer,
                                  unsigned short int position[], unsigned short int direction, wall &virtual_walls );
  void map_potentials( wall &maze_memorizer, route &route_memorizer,
                       unsigned short int position[], unsigned short int &direction,
                       unsigned short int starting_position[], unsigned short int starting_direction,
                       unsigned short int aimed_position[], unsigned short int aimed_direction );
};

void reverse::map_potentials( wall &maze_memorizer, route &route_memorizer,
                              unsigned short int position[], unsigned short int &direction,
                              unsigned short int starting_position[], unsigned short int starting_direction,
                              unsigned short int aimed_position[], unsigned short int aimed_direction ) {
  unsigned short int next_action = DUMMY;
  unsigned short int potential_to_set = 0;
  if ( position[0] == starting_position[0] && position[1] == starting_position[1] ) {
    if ( direction == starting_direction ) {
      route_memorizer.has_potential_map = 1;
      route_memorizer.solve_the_maze( maze_memorizer, starting_position, starting_direction, aimed_position, aimed_direction );
      route_memorizer.reset();
      is_done = 1;
    }
    else {
      if ( maze_memorizer.safe_to_go( position, direction, LEFT ) ) {
        next_action = GO_LEFTWARD;
      }
      else if ( maze_memorizer.safe_to_go( position, direction, UP ) ) {
        next_action = GO_FORWARD;
      }
      else if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
        next_action = GO_RIGHTWARD;
      }
      route_memorizer.update_status( position, direction, next_action );
      if ( direction == starting_direction ) {
        potential_to_set = route_memorizer.s;
      }
      else {
        potential_to_set = route_memorizer.t;
      }
      route_memorizer.put_a_sign_to_reverse( position, direction );
      route_memorizer.set_potential( position, potential_to_set );
      route_memorizer.update_visited_places( position );
      is_done = 1;      
    }
  }
  else {
    if ( route_memorizer.signed_crossroad( position ) ) {
      route_memorizer.decrement_the_count_of_facing_unvisited_blocks( position );
      virtual_position[0] = position[0];
      virtual_position[1] = position[1];
      virtual_direction = direction;
      route_memorizer.update_status( virtual_position, virtual_direction, GO_LEFTWARD );
      if ( maze_memorizer.safe_to_go( position, direction, LEFT ) &&
           !(virtual_position[0] == starting_position[0] && virtual_position[1] == starting_position[1]) ) {
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

unsigned short int reverse::next_action ( wall &maze_memorizer, route &route_memorizer,
                                          unsigned short int position[], unsigned short int direction, wall &virtual_walls ) {
  unsigned short int succeeded_virtual_foolish_solution;
  if ( route_memorizer.signed_crossroad( position ) ) {
    virtual_walls.update( position, direction, DOWN, UNSAFE );
    if ( maze_memorizer.safe_to_go( position, direction, LEFT ) && virtual_walls.safe_to_go( position, direction, LEFT ) ) {
      route_memorizer.decrement_the_count_of_facing_unvisited_blocks( position );
      virtual_position[0] = position[0];
      virtual_position[1] = position[1];
      virtual_direction = direction;
      route_memorizer.update_status( virtual_position, virtual_direction, GO_LEFTWARD );
      if ( maze_memorizer.need_to_scan( virtual_position ) ) {
        succeeded_virtual_foolish_solution =
          route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
        route_memorizer.update_status( virtual_position, virtual_direction, TURN_L_90 );
        if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
          virtual_walls.update( virtual_position, virtual_direction, LEFT, UNSAFE );
        }
        else {
          is_done = 1;
          return GO_LEFTWARD;
        }
      }
      else {
        route_memorizer.update_visited_places( virtual_position );
        route_memorizer.update_for_virtual_walls( virtual_position, virtual_direction, maze_memorizer, virtual_walls );
        virtual_walls.update( position, direction, LEFT, UNSAFE );
      }
    }
    if ( route_memorizer.signed_crossroad( position ) &&
         maze_memorizer.safe_to_go( position, direction, UP ) && virtual_walls.safe_to_go( position, direction, UP ) ) {
      route_memorizer.decrement_the_count_of_facing_unvisited_blocks( position );
      virtual_position[0] = position[0];
      virtual_position[1] = position[1];
      virtual_direction = direction;
      route_memorizer.update_status( virtual_position, virtual_direction, GO_FORWARD );
      if ( maze_memorizer.need_to_scan( virtual_position ) ) {
        succeeded_virtual_foolish_solution =
          route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
        route_memorizer.update_status( virtual_position, virtual_direction, TURN );
        if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
          virtual_walls.update( virtual_position, virtual_direction, UP, UNSAFE );
        }
        else {
          is_done = 1;
          return GO_FORWARD;
        }
      }
      else {
        route_memorizer.update_visited_places( virtual_position );
        route_memorizer.update_for_virtual_walls( virtual_position, virtual_direction, maze_memorizer, virtual_walls );
        virtual_walls.update( position, direction, UP, UNSAFE );
      }
    }
    if ( route_memorizer.signed_crossroad( position ) &&
         maze_memorizer.safe_to_go( position, direction, RIGHT ) && virtual_walls.safe_to_go( position, direction, RIGHT ) ) {
      route_memorizer.decrement_the_count_of_facing_unvisited_blocks( position );
      virtual_position[0] = position[0];
      virtual_position[1] = position[1];
      virtual_direction = direction;
      route_memorizer.update_status( virtual_position, virtual_direction, GO_RIGHTWARD );
      if ( maze_memorizer.need_to_scan( virtual_position ) ) {
        succeeded_virtual_foolish_solution =
          route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
        route_memorizer.update_status( virtual_position, virtual_direction, TURN_R_90 );
        if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
          virtual_walls.update( virtual_position, virtual_direction, RIGHT, UNSAFE );
        }
        else {
          is_done = 1;
          return GO_RIGHTWARD;
        }
      }
      else {
        route_memorizer.update_visited_places( virtual_position );
        route_memorizer.update_for_virtual_walls( virtual_position, virtual_direction, maze_memorizer, virtual_walls );
        virtual_walls.update( position, direction, RIGHT, UNSAFE );
      }
    }
  }
  if ( maze_memorizer.safe_to_go( position, direction, LEFT ) && virtual_walls.safe_to_go( position, direction, LEFT ) ) {
    return GO_LEFTWARD;
  }
  else if ( maze_memorizer.safe_to_go( position, direction, UP ) && virtual_walls.safe_to_go( position, direction, UP ) ) {
    return GO_FORWARD;
  }
  else if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) && virtual_walls.safe_to_go( position, direction, RIGHT ) ) {
    return GO_RIGHTWARD;
  }
  return DUMMY;
}

reverse::reverse() {
  is_done = 0;
}

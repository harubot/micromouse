class normal {
  unsigned short int virtual_position[2];
  unsigned short int virtual_direction;
public:
  unsigned short int is_done;
  normal();
  unsigned short int next_action( wall &maze_memorizer, route &route_memorizer,
                                  unsigned short int position[], unsigned short int direction, wall &virtual_walls );
  void map_potentials( wall &maze_memorizer, route &route_memorizer,
                       unsigned short int position[], unsigned short int &direction,
                       unsigned short int starting_position[], unsigned short int starting_direction,
                       unsigned short int aimed_position[], unsigned short int aimed_direction );
};

void normal::map_potentials( wall &maze_memorizer, route &route_memorizer,
                             unsigned short int position[], unsigned short int &direction,
                             unsigned short int starting_position[], unsigned short int starting_direction,
                             unsigned short int aimed_position[], unsigned short int aimed_direction ) {
  unsigned short int next_action = DUMMY;
  unsigned short int the_count_of_facing_unvisited_blocks = 0; //precisely wrong name
  unsigned short int potential_to_set = 0;
  virtual_position[0] = position[0];
  virtual_position[1] = position[1];
  virtual_direction = direction;
  route_memorizer.update_status( virtual_position, virtual_direction, GO_LEFTWARD );
  if ( maze_memorizer.safe_to_go( position, direction, LEFT ) &&
       !(virtual_position[0] == starting_position[0] && virtual_position[1] == starting_position[1]) ) {
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
  virtual_position[0] = position[0];
  virtual_position[1] = position[1];
  virtual_direction = direction;
  route_memorizer.update_status( virtual_position, virtual_direction, GO_FORWARD );
  if ( maze_memorizer.safe_to_go( position, direction, UP ) &&
       !(virtual_position[0] == starting_position[0] && virtual_position[1] == starting_position[1]) ) {
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
  virtual_position[0] = position[0];
  virtual_position[1] = position[1];
  virtual_direction = direction;
  route_memorizer.update_status( virtual_position, virtual_direction, GO_RIGHTWARD );
  if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) &&
       !(virtual_position[0] == starting_position[0] && virtual_position[1] == starting_position[1]) ) {
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
    if ( position[0] == starting_position[0] && position[1] == starting_position[1] ) {
      route_memorizer.update_status( position, direction, GO_BACKWARD );
      route_memorizer.put_a_sign_to_reverse( position, direction );
      route_memorizer.update_visited_places( position );
      route_memorizer.set_potential( position, route_memorizer.t );
    }
    else {
      route_memorizer.remove_a_sign_to_reverse( position );
      route_memorizer.update_status( position, direction, GO_BACKWARD );
      is_done = 1;
    }
  }
}

unsigned short int normal::next_action ( wall &maze_memorizer, route &route_memorizer,
                                         unsigned short int position[], unsigned short int direction, wall &virtual_walls ) {
  unsigned short int the_count_of_safe_to_go_blocks;
  unsigned short int succeeded_virtual_foolish_solution;
  if ( maze_memorizer.need_to_scan( position ) ) {
    return SCAN;
  }
  else {
    route_memorizer.update_for_virtual_walls( position, direction, maze_memorizer, virtual_walls );
    the_count_of_safe_to_go_blocks =
      maze_memorizer.the_count_of_safe_to_go_blocks(position,direction) - virtual_walls.the_count_of_unsafe_to_go_blocks(position,direction);
    if ( the_count_of_safe_to_go_blocks ) {
      if ( maze_memorizer.safe_to_go( position, direction, LEFT ) && virtual_walls.safe_to_go( position, direction, LEFT ) ) {
        virtual_position[0] = position[0];
        virtual_position[1] = position[1];
        virtual_direction = direction;
        route_memorizer.update_status( virtual_position, virtual_direction, GO_LEFTWARD );
        if ( maze_memorizer.need_to_scan( virtual_position ) ) {
          if ( the_count_of_safe_to_go_blocks > 1 ) {
            succeeded_virtual_foolish_solution =
              route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
            route_memorizer.update_status( virtual_position, virtual_direction, TURN_L_90 );
            if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
              the_count_of_safe_to_go_blocks--;
              virtual_walls.update( virtual_position, virtual_direction, LEFT, UNSAFE );
            }
            else {
              route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
              return GO_LEFTWARD;
            }
          }
          else {
            route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
            return GO_LEFTWARD;
          }
        }
        else {
          the_count_of_safe_to_go_blocks--;
          route_memorizer.update_visited_places( virtual_position );
          route_memorizer.update_for_virtual_walls( position, direction, maze_memorizer, virtual_walls );
          virtual_walls.update( position, direction, LEFT, UNSAFE );
        }
      }
      if ( maze_memorizer.safe_to_go( position, direction, UP ) && virtual_walls.safe_to_go( position, direction, UP ) ) {
        virtual_position[0] = position[0];
        virtual_position[1] = position[1];
        virtual_direction = direction;
        route_memorizer.update_status( virtual_position, virtual_direction, GO_FORWARD );
        if ( maze_memorizer.need_to_scan( virtual_position ) ) {
          if ( the_count_of_safe_to_go_blocks > 1 ) {
            succeeded_virtual_foolish_solution =
              route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
            route_memorizer.update_status( virtual_position, virtual_direction, TURN );
            if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
              the_count_of_safe_to_go_blocks--;
              virtual_walls.update( virtual_position, virtual_direction, UP, UNSAFE );
            }
            else {
              route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
              return GO_FORWARD;
            }
          }
          else {
            route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
            return GO_FORWARD;
          }
        }
        else {
          the_count_of_safe_to_go_blocks--;
          route_memorizer.update_visited_places( virtual_position );
          route_memorizer.update_for_virtual_walls( position, direction, maze_memorizer, virtual_walls );
          virtual_walls.update( position, direction, UP, UNSAFE );
        }
      }
      if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) && virtual_walls.safe_to_go( position, direction, RIGHT ) ) {
        virtual_position[0] = position[0];
        virtual_position[1] = position[1];
        virtual_direction = direction;
        route_memorizer.update_status( virtual_position, virtual_direction, GO_RIGHTWARD );
        if ( maze_memorizer.need_to_scan( virtual_position ) ) {
          if ( the_count_of_safe_to_go_blocks > 1 ) {
            succeeded_virtual_foolish_solution =
              route_memorizer.virtual_foolish_solution( maze_memorizer, virtual_position, virtual_direction, position );
            route_memorizer.update_status( virtual_position, virtual_direction, TURN_R_90 );
            if ( virtual_direction == direction && succeeded_virtual_foolish_solution ) {
              the_count_of_safe_to_go_blocks--;
              virtual_walls.update( virtual_position, virtual_direction, RIGHT, UNSAFE );
            }
            else {
              route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
              return GO_RIGHTWARD;
            }
          }
          else {
            route_memorizer.set_the_count_of_facing_unvisited_blocks( position, the_count_of_safe_to_go_blocks - 1 );
            return GO_RIGHTWARD;
          }
        }
        else {
          the_count_of_safe_to_go_blocks--;
          route_memorizer.update_visited_places( virtual_position );
          route_memorizer.update_for_virtual_walls( position, direction, maze_memorizer, virtual_walls );
          virtual_walls.update( position, direction, RIGHT, UNSAFE );
        }
      }
    }
    is_done = 1;
    return DUMMY;
  }
}

normal::normal() {
  is_done = 0;
}

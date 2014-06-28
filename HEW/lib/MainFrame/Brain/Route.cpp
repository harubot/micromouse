class route {
  struct {
    unsigned short int visited :1;
    unsigned short int the_count_of_facing_unvisited_blocks :3;
    unsigned short int sign_to_reverse :6;
    unsigned short int sign_to_solve :6;
    unsigned short int potential;
  } route_info[16][16];
public:
  unsigned short int s;
  unsigned short int t;
  unsigned short int has_potential_map;
  route();
  void reset(); //note: doesn't reset signs to solve
  void reset_the_map_to_solve();
  void put_a_sign_to_solve ( unsigned short int position[], unsigned short int opposite_direction_to_solve );
  unsigned short int get_the_direction_to_solve( unsigned short int position[], unsigned short int current_direction );
  void put_a_sign_to_reverse ( unsigned short int position[], unsigned short int opposite_direction_to_reverse );
  void remove_a_sign_to_reverse ( unsigned short int position[] );
  unsigned short int get_the_direction_to_reverse( unsigned short int position[], unsigned short int current_direction );
  void update_visited_places ( unsigned short int position[] );
  unsigned short int visited ( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_check );
  unsigned short int signed_crossroad( unsigned int short position[] );
  void set_the_count_of_facing_unvisited_blocks ( unsigned short int position[], unsigned short int the_count_of_facing_unvisited_blocks );
  void decrement_the_count_of_facing_unvisited_blocks ( unsigned short int position[], unsigned short int direction,
                                                        unsigned short int direction_to_decrement );
  void decrement_the_count_of_facing_unvisited_blocks ( unsigned short int position[] );
  unsigned short int get_potential( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_get );
  unsigned short int get_potential( unsigned short int position[] );
  void set_potential( unsigned short int position[], unsigned short int potential );
  void update_status( unsigned short int position[], unsigned short int &direction, unsigned short int action );
  void solve_the_maze( wall &maze_memorizer );
  void solve_the_maze( wall &maze_memorizer,
                       unsigned short int starting_position[], unsigned short int starting_direction,
                       unsigned short int position[], unsigned short int direction );
  unsigned short int virtual_foolish_solution( wall &maze_memorizer,  unsigned short int position[], unsigned short int &direction,
                                 unsigned short int aimed_position[] );
  void update_for_virtual_walls( unsigned short int position[], unsigned short int direction, wall &maze_memorizer, wall &virtual_walls );
};

route::route() {
  reset();
  has_potential_map = 0;
  s = 3;
  t = 9;
}

void route::reset() {
  for ( unsigned short int i = 0; i < 16; i++ ) {
    for ( unsigned short int j = 0; j < 16; j++ ) {
      route_info[i][j].visited = 0;
      route_info[i][j].the_count_of_facing_unvisited_blocks = 0;
      route_info[i][j].potential = 0;
      route_info[i][j].sign_to_reverse = DUMMY;
    }
  }
}

void route::reset_the_map_to_solve() {
  for ( unsigned short int i = 0; i < 16; i++ ) {
    for ( unsigned short int j = 0; j < 16; j++ ) {
      route_info[i][j].sign_to_solve = DUMMY;
    }
  }
}

void route::put_a_sign_to_solve ( unsigned short int position[], unsigned short int opposite_direction_to_solve ) {
  if ( opposite_direction_to_solve == UP ) {
    route_info[position[0]][position[1]].sign_to_solve = DOWN;
  }
  else if ( opposite_direction_to_solve == RIGHT ) {
    route_info[position[0]][position[1]].sign_to_solve = LEFT;
  }
  else if ( opposite_direction_to_solve == LEFT ) {
    route_info[position[0]][position[1]].sign_to_solve = RIGHT;
  }
  else if ( opposite_direction_to_solve == DOWN ) {
    route_info[position[0]][position[1]].sign_to_solve = UP;
  }
}

unsigned short int route::get_the_direction_to_solve( unsigned short int position[], unsigned short int current_direction ) {
  unsigned short int sign_to_solve = route_info[position[0]][position[1]].sign_to_solve;
  if ( sign_to_solve == UP ) {
    if ( current_direction == UP ) {
      return GO_FORWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_BACKWARD;
    }
  }
  else if ( sign_to_solve == RIGHT ) {
    if ( current_direction == UP ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_FORWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_LEFTWARD;
    }
  }
  else if ( sign_to_solve == LEFT ) {
    if ( current_direction == UP ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_FORWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_RIGHTWARD;
    }
  }
  else if ( sign_to_solve == DOWN ) {
    if ( current_direction == UP ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_FORWARD;
    }
  }
  return DUMMY;
}

void route::put_a_sign_to_reverse ( unsigned short int position[], unsigned short int opposite_direction_to_reverse ) {
  if ( opposite_direction_to_reverse == UP ) {
    route_info[position[0]][position[1]].sign_to_reverse = DOWN;
  }
  else if ( opposite_direction_to_reverse == RIGHT ) {
    route_info[position[0]][position[1]].sign_to_reverse = LEFT;
  }
  else if ( opposite_direction_to_reverse == LEFT ) {
    route_info[position[0]][position[1]].sign_to_reverse = RIGHT;
  }
  else if ( opposite_direction_to_reverse == DOWN ) {
    route_info[position[0]][position[1]].sign_to_reverse = UP;
  }
}

void route::remove_a_sign_to_reverse ( unsigned short int position[] ) {
  route_info[position[0]][position[1]].sign_to_reverse = DUMMY;
}

unsigned short int route::get_the_direction_to_reverse( unsigned short int position[], unsigned short int current_direction ) {
  unsigned short int sign_to_reverse = route_info[position[0]][position[1]].sign_to_reverse;
  if ( sign_to_reverse == UP ) {
    if ( current_direction == UP ) {
      return GO_FORWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_BACKWARD;
    }
  }
  else if ( sign_to_reverse == RIGHT ) {
    if ( current_direction == UP ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_FORWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_LEFTWARD;
    }
  }
  else if ( sign_to_reverse == LEFT ) {
    if ( current_direction == UP ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_FORWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_RIGHTWARD;
    }
  }
  else if ( sign_to_reverse == DOWN ) {
    if ( current_direction == UP ) {
      return GO_BACKWARD;
    }
    else if ( current_direction == RIGHT ) {
      return GO_RIGHTWARD;
    }
    else if ( current_direction == LEFT ) {
      return GO_LEFTWARD;
    }
    else if ( current_direction == DOWN ) {
      return GO_FORWARD;
    }
  }
  return DUMMY;
}

void route::update_visited_places ( unsigned short int position[] ) {
  route_info[position[0]][position[1]].visited = 1;
}

unsigned short int route::visited ( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_check ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_check == LEFT ) {
      if ( route_info[i][j-1].visited ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_check == UP ) {
      if ( route_info[i-1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == RIGHT ) {
      if ( route_info[i][j+1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == DOWN ) {
      if ( route_info[i+1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_check == LEFT ) {
      if ( route_info[i][j+1].visited ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_check == UP ) {
      if ( route_info[i+1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == RIGHT ) {
      if ( route_info[i][j-1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == DOWN ) {
      if ( route_info[i-1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_check == LEFT ) {
      if ( route_info[i-1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_check == UP ) {
      if ( route_info[i][j+1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == RIGHT ) {
      if ( route_info[i+1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == DOWN ) {
      if ( route_info[i][j-1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_check == LEFT ) {
      if ( route_info[i+1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_check == UP ) {
      if ( route_info[i][j-1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == RIGHT ) {
      if ( route_info[i-1][j].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
    else if ( direction_to_check == DOWN ) {
      if ( route_info[i][j+1].visited ) {
        return 1;
      }
      else {
        return 0;
      }      
    }
  }
  return DUMMY;
}

unsigned short int route::signed_crossroad( unsigned short int position[] ) {
  if ( route_info[position[0]][position[1]].the_count_of_facing_unvisited_blocks ) {
    return 1;
  }
  else {
    return 0;
  }
}

void route::set_the_count_of_facing_unvisited_blocks ( unsigned short int position[], unsigned short int the_count_of_facing_unvisited_blocks ) {
  route_info[position[0]][position[1]].the_count_of_facing_unvisited_blocks = the_count_of_facing_unvisited_blocks;
}

void route::decrement_the_count_of_facing_unvisited_blocks ( unsigned short int position[], unsigned short int direction,
                                                             unsigned short int direction_to_decrement ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_decrement == LEFT ) {
      route_info[i][j-1].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == UP ) {
      route_info[i-1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == RIGHT ) {
      route_info[i][j+1].the_count_of_facing_unvisited_blocks -= 1;
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_decrement == LEFT ) {
      route_info[i][j+1].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == UP ) {
      route_info[i+1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == RIGHT ) {
      route_info[i][j-1].the_count_of_facing_unvisited_blocks -= 1;
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_decrement == LEFT ) {
      route_info[i-1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == UP ) {
      route_info[i][j+1].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == RIGHT ) {
      route_info[i+1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_decrement == LEFT ) {
      route_info[i+1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == UP ) {
      route_info[i][j-1].the_count_of_facing_unvisited_blocks -= 1;
    }
    else if ( direction_to_decrement == RIGHT ) {
      route_info[i-1][j].the_count_of_facing_unvisited_blocks -= 1;
    }
  }
}

void route::decrement_the_count_of_facing_unvisited_blocks ( unsigned short int position[] ) {
  route_info[position[0]][position[1]].the_count_of_facing_unvisited_blocks -= 1;
}

unsigned short int route::get_potential( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_get ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_get == LEFT ) {
      return route_info[i][j-1].potential;
    }
    else if ( direction_to_get == UP ) {
      return route_info[i-1][j].potential;
    }
    else if ( direction_to_get == RIGHT ) {
      return route_info[i][j+1].potential;
    }
    else if ( direction_to_get == DOWN ) {
      return route_info[i+1][j].potential;
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_get == LEFT ) {
      return route_info[i][j+1].potential;
    }
    else if ( direction_to_get == UP ) {
      return route_info[i+1][j].potential;
    }
    else if ( direction_to_get == RIGHT ) {
      return route_info[i][j-1].potential;
    }
    else if ( direction_to_get == DOWN ) {
      return route_info[i-1][j].potential;
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_get == LEFT ) {
      return route_info[i-1][j].potential;
    }
    else if ( direction_to_get == UP ) {
      return route_info[i][j+1].potential;
    }
    else if ( direction_to_get == RIGHT ) {
      return route_info[i+1][j].potential;
    }
    else if ( direction_to_get == DOWN ) {
      return route_info[i][j-1].potential;
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_get == LEFT ) {
      return route_info[i+1][j].potential;
    }
    else if ( direction_to_get == UP ) {
      return route_info[i][j-1].potential;
    }
    else if ( direction_to_get == RIGHT ) {
      return route_info[i-1][j].potential;
    }
    else if ( direction_to_get == DOWN ) {
      return route_info[i][j+1].potential;
    }
  }
  return DUMMY;
}

unsigned short int route::get_potential( unsigned short int position[] ) {
  return route_info[position[0]][position[1]].potential;
}

void route::set_potential( unsigned short int position[], unsigned short int potential ) {
  route_info[position[0]][position[1]].potential = potential;
}

void route::update_status( unsigned short int position[], unsigned short int &direction, unsigned short int action ) {
  if ( action == GO_FORWARD ) {
    if ( direction == UP ) {
      position[0] -= 1;
    }
    else if ( direction == DOWN ) {
      position[0] += 1;
    }
    else if ( direction == RIGHT ) {
      position[1] += 1;
    }
    else if ( direction == LEFT ) {
      position[1] -= 1;
    }
  }
  else if ( action == GO_BACKWARD ) {
    if ( direction == UP ) {
      position[0] += 1;
      direction = DOWN;
    }
    else if ( direction == DOWN ) {
      position[0] -= 1;
      direction = UP;
    }
    else if ( direction == RIGHT ) {
      position[1] -= 1;
      direction = LEFT;
    }
    else if ( direction == LEFT ) {
      position[1] += 1;
      direction = RIGHT;
    }
  }
  else if ( action == GO_RIGHTWARD ) {
    if ( direction == UP ) {
      position[1] += 1;
      direction = RIGHT;
    }
    else if ( direction == DOWN ) {
      position[1] -= 1;
      direction = LEFT;
    }
    else if ( direction == RIGHT ) {
      position[0] += 1;
      direction = DOWN;
    }
    else if ( direction == LEFT ) {
      position[0] -= 1;
      direction = UP;
    }
  }
  else if ( action == GO_LEFTWARD ) {
    if ( direction == UP ) {
      position[1] -= 1;
      direction = LEFT;
    }
    else if ( direction == DOWN ) {
      position[1] += 1;
      direction = RIGHT;
    }
    else if ( direction == RIGHT ) {
      position[0] -= 1;
      direction = UP;
    }
    else if ( direction == LEFT ) {
      position[0] += 1;
      direction = DOWN;
    }
  }
  else if ( action == TURN ) {
    if ( direction == UP ) {
      direction = DOWN;
    }
    else if ( direction == DOWN ) {
      direction = UP;
    }
    else if ( direction == RIGHT ) {
      direction = LEFT;
    }
    else if ( direction == LEFT ) {
      direction = RIGHT;
    }    
  }
  else if ( action == TURN_L_90 ) {
    if ( direction == UP ) {
      direction = LEFT;
    }
    else if ( direction == DOWN ) {
      direction = RIGHT;
    }
    else if ( direction == RIGHT ) {
      direction = UP;
    }
    else if ( direction == LEFT ) {
      direction = DOWN;
    }    
  }
  else if ( action == TURN_R_90 ) {
    if ( direction == UP ) {
      direction = RIGHT;
    }
    else if ( direction == DOWN ) {
      direction = LEFT;
    }
    else if ( direction == RIGHT ) {
      direction = DOWN;
    }
    else if ( direction == LEFT ) {
      direction = UP;
    }
  }
}

void route::solve_the_maze( wall &maze_memorizer ) {
  unsigned short int potentials[4];
  unsigned short int position[2];
  unsigned short int potential;
  unsigned short int direction;
  unsigned short int next_action;
  position[0] = GOAL_I_MIN;
  position[1] = GOAL_J_MIN;
  potentials[0] = get_potential( position );
  position[0] = GOAL_I_MIN;
  position[1] = GOAL_J_MAX;
  potentials[1] = get_potential( position );
  position[0] = GOAL_I_MAX;
  position[1] = GOAL_J_MIN;
  potentials[2] = get_potential( position );
  position[0] = GOAL_I_MAX;
  position[1] = GOAL_J_MAX;
  potentials[3] = get_potential( position );
  potential = potentials[0];
  position[0] = GOAL_I_MIN;
  position[1] = GOAL_J_MIN;
  direction = UP;
  if ( potentials[1] < potential ) {
    potential = potentials[1];
    position[0] = GOAL_I_MIN;
    position[1] = GOAL_J_MAX;
    direction = RIGHT;
  }
  if ( potentials[2] < potential ) {
    potential = potentials[2];
    position[0] = GOAL_I_MAX;
    position[1] = GOAL_J_MIN;
    direction = LEFT;
  }
  if ( potentials[3] < potential ) {
    potential = potentials[3];
    position[0] = GOAL_I_MAX;
    position[1] = GOAL_J_MAX;
    direction = DOWN;
  }
  while( !( position[0] == 15 && position[1] == 0 ) ) {
    if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
      potentials[0] = get_potential( position, direction, RIGHT );
    }
    else {
      potentials[0] = 1000;
    }
    if ( maze_memorizer.safe_to_go( position, direction, UP ) ) {
      potentials[1] = get_potential( position, direction, UP );
    }
    else {
      potentials[1] = 1000;
    }
    if ( maze_memorizer.safe_to_go( position, direction, LEFT ) ) {
      potentials[2] = get_potential( position, direction, LEFT );
    }
    else {
      potentials[2] = 1000;
    }
    potential = potentials[0];
    next_action = GO_RIGHTWARD;
    if ( potentials[1] < potential ) {
      potential = potentials[1];
      next_action = GO_FORWARD;
    }
    if ( potentials[2] < potential ) {
      potential = potentials[2];
      next_action = GO_LEFTWARD;
    }
    update_status( position, direction, next_action );
    put_a_sign_to_solve( position, direction );
  }
}

void route::solve_the_maze( wall &maze_memorizer,
                            unsigned short int aimed_position[], unsigned short int aimed_direction,
                            unsigned short int position[], unsigned short int direction ) {
  unsigned short int next_action;
  unsigned short int potential;
  unsigned short int potentials[3];
  update_status( position, direction, GO_BACKWARD );
  put_a_sign_to_solve( position, direction );
  while( !( position[0] == aimed_position[0] && position[1] == aimed_position[1] ) ) {
    if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) ) {
      potentials[0] = get_potential( position, direction, RIGHT );
    }
    else {
      potentials[0] = 1000;
    }
    if ( maze_memorizer.safe_to_go( position, direction, UP ) ) {
      potentials[1] = get_potential( position, direction, UP );
    }
    else {
      potentials[1] = 1000;
    }
    if ( maze_memorizer.safe_to_go( position, direction, LEFT ) ) {
      potentials[2] = get_potential( position, direction, LEFT );
    }
    else {
      potentials[2] = 1000;
    }
    potential = potentials[0];
    next_action = GO_RIGHTWARD;
    if ( potentials[1] < potential ) {
      potential = potentials[1];
      next_action = GO_FORWARD;
    }
    if ( potentials[2] < potential ) {
      potential = potentials[2];
      next_action = GO_LEFTWARD;
    }
    update_status( position, direction, next_action );
    put_a_sign_to_solve( position, direction );
  }
}

unsigned short int route::virtual_foolish_solution( wall &maze_memorizer,  unsigned short int position[], unsigned short int &direction,
                                      unsigned short int aimed_position[] ) {
  short int cnt_to_determine_whether_the_route_includes_the_goal;
  unsigned short int previous_position[2];
  previous_position[0] = position[0];
  previous_position[1] = position[1];
  cnt_to_determine_whether_the_route_includes_the_goal = 0;
  while( position[0] != aimed_position[0] || position[1] != aimed_position[1] ) {
    if ( !maze_memorizer.unsafe_to_go( position, direction, LEFT ) ) {
      update_status( position, direction, GO_LEFTWARD );
    }
    else if ( !maze_memorizer.unsafe_to_go( position, direction, UP ) ) {
      update_status( position, direction, GO_FORWARD );
    }
    else if ( !maze_memorizer.unsafe_to_go( position, direction, RIGHT ) ) {
      update_status( position, direction, GO_RIGHTWARD );
    }
    else {
      update_status( position, direction, GO_BACKWARD );
    }
    if ( previous_position[0] > GOAL_I_MIN && previous_position[1] == GOAL_J_MIN && position[1] == GOAL_J_MAX ) {
      cnt_to_determine_whether_the_route_includes_the_goal++;
    }
    else if ( previous_position[0] > GOAL_I_MIN && previous_position[1] == GOAL_J_MAX && position[1] == GOAL_J_MIN ) {
      cnt_to_determine_whether_the_route_includes_the_goal--;
    }
    previous_position[0] = position[0];
    previous_position[1] = position[1];
  }
  if ( cnt_to_determine_whether_the_route_includes_the_goal == 0 ) {
    return 1;
  }
  else {
    return 0;
  }
}

void route::update_for_virtual_walls( unsigned short int position[], unsigned short int direction, wall &maze_memorizer, wall &virtual_walls ) {
  if ( maze_memorizer.safe_to_go( position, direction, LEFT ) &&
       virtual_walls.safe_to_go( position, direction, LEFT ) && visited( position, direction, LEFT ) ) {
    virtual_walls.update( position, direction, LEFT, UNSAFE );
    decrement_the_count_of_facing_unvisited_blocks( position, direction, LEFT );
  }
  if ( maze_memorizer.safe_to_go( position, direction, UP ) &&
       virtual_walls.safe_to_go( position, direction, UP ) && visited( position, direction, UP ) ) {
    virtual_walls.update( position, direction, UP, UNSAFE );
    decrement_the_count_of_facing_unvisited_blocks( position, direction, UP );
  }
  if ( maze_memorizer.safe_to_go( position, direction, RIGHT ) &&
       virtual_walls.safe_to_go( position, direction, RIGHT ) && visited( position, direction, RIGHT ) ) {
    virtual_walls.update( position, direction, RIGHT, UNSAFE );
    decrement_the_count_of_facing_unvisited_blocks( position, direction, RIGHT );
  }
}

class wall {
  struct {
    unsigned char up    :2;
    unsigned char down  :2;
    unsigned char right :2;
    unsigned char left  :2;
  } wall_info[16][16];
  void side_update( unsigned short int position[], unsigned short int direction_to_update );
public:
  wall( unsigned short int init_type );
  void update ( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_update, unsigned short int content );
  unsigned short int safe_to_go ( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_go );
  unsigned short int unsafe_to_go ( unsigned short int position[], unsigned short int direction, unsigned short int direction_to_go );
  unsigned short int the_count_of_safe_to_go_blocks ( unsigned short int position[], unsigned short int direction );
  unsigned short int the_count_of_unsafe_to_go_blocks ( unsigned short int position[], unsigned short int direction );
  unsigned short int need_to_scan ( unsigned short int position[] );
};

wall::wall( unsigned short int init_type ) {
  if ( init_type == MAZE ) {
    for ( unsigned short int i = 0; i < 16; i++ ) {
      for ( unsigned short int j = 0; j < 16; j++ ) {
        wall_info[i][j].up    = UNKNOWN;
        wall_info[i][j].down  = UNKNOWN;
        wall_info[i][j].right = UNKNOWN;
        wall_info[i][j].left  = UNKNOWN;
      }
    }
    wall_info[15][0].up    = SAFE;
    wall_info[14][0].down  = SAFE;
    wall_info[15][0].right = UNSAFE;
    wall_info[15][1].left  = UNSAFE;
    for ( unsigned short int i = 0; i < 16; i++ ) {
      wall_info[i][0].left   = UNSAFE;
      wall_info[i][15].right = UNSAFE;
      wall_info[0][i].up     = UNSAFE;
      wall_info[15][i].down  = UNSAFE;
    }
  }
  else if ( init_type == VIRTUAL ) {
    for ( unsigned short int i = 0; i < 16; i++ ) {
      for ( unsigned short int j = 0; j < 16; j++ ) {
        wall_info[i][j].up    = SAFE;
        wall_info[i][j].down  = SAFE;
        wall_info[i][j].right = SAFE;
        wall_info[i][j].left  = SAFE;
      }
    }
  }
}



void wall::side_update( unsigned short int position[], unsigned short int direction_to_update ) {
  unsigned short int i, j;
  unsigned short int the_count_of_safe_walls = 0;
  i = position[0];
  j = position[1];
  if ( direction_to_update == UPPER_LEFT ) {
    if ( wall_info[i][j].left == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i][j-1].up == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i-1][j-1].right == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i-1][j].down == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( the_count_of_safe_walls == 3 ) {
      if ( wall_info[i][j].left == UNKNOWN ) {
        wall_info[i][j].left = UNSAFE;
        wall_info[i][j-1].right = UNSAFE;
      }
      else if ( wall_info[i][j-1].up == UNKNOWN ) {
        wall_info[i][j-1].up = UNSAFE;
        wall_info[i-1][j-1].down = UNSAFE;
      }
      else if ( wall_info[i-1][j-1].right == UNKNOWN ) {
        wall_info[i-1][j-1].right = UNSAFE;
        wall_info[i-1][j].left = UNSAFE;
      }
      else if ( wall_info[i-1][j].down == UNKNOWN ) {
        wall_info[i-1][j].down = UNSAFE;
        wall_info[i][j].up = UNSAFE;
      }    
    }
  }
  else if ( direction_to_update == UPPER_RIGHT ) {
    if ( wall_info[i][j].right == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i][j+1].up == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i-1][j+1].left == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i-1][j].down == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( the_count_of_safe_walls == 3 ) {
      if ( wall_info[i][j].right == UNKNOWN ) {
        wall_info[i][j].right = UNSAFE;
        wall_info[i][j+1].left = UNSAFE;
      }
      else if ( wall_info[i][j+1].up == UNKNOWN ) {
        wall_info[i][j+1].up = UNSAFE;
        wall_info[i-1][j+1].down = UNSAFE;
      }
      else if ( wall_info[i-1][j+1].left == UNKNOWN ) {
        wall_info[i-1][j+1].left = UNSAFE;
        wall_info[i-1][j].right = UNSAFE;
      }
      else if ( wall_info[i-1][j].down == UNKNOWN ) {
        wall_info[i-1][j].down = UNSAFE;
        wall_info[i][j].up = UNSAFE;
      }    
    }
  }
  else if ( direction_to_update == LOWER_RIGHT ) {
    if ( wall_info[i][j].right == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i][j+1].down == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i+1][j+1].left == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i+1][j].up == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( the_count_of_safe_walls == 3 ) {
      if ( wall_info[i][j].right == UNKNOWN ) {
        wall_info[i][j].right = UNSAFE;
        wall_info[i][j+1].left = UNSAFE;
      }
      else if ( wall_info[i][j+1].down == UNKNOWN ) {
        wall_info[i][j+1].down = UNSAFE;
        wall_info[i+1][j+1].up = UNSAFE;
      }
      else if ( wall_info[i+1][j+1].left == UNKNOWN ) {
        wall_info[i+1][j+1].left = UNSAFE;
        wall_info[i+1][j].right = UNSAFE;
      }
      else if ( wall_info[i+1][j].up == UNKNOWN ) {
        wall_info[i+1][j].up = UNSAFE;
        wall_info[i][j].down = UNSAFE;
      }    
    }
  }
  else if ( direction_to_update == LOWER_LEFT ) {
    if ( wall_info[i][j].left == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i][j-1].down == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i+1][j-1].right == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( wall_info[i+1][j].up == SAFE ) {
      the_count_of_safe_walls++;
    }
    if ( the_count_of_safe_walls == 3 ) {
      if ( wall_info[i][j].left == UNKNOWN ) {
        wall_info[i][j].left = UNSAFE;
        wall_info[i][j-1].right = UNSAFE;
      }
      else if ( wall_info[i][j-1].down == UNKNOWN ) {
        wall_info[i][j-1].down = UNSAFE;
        wall_info[i+1][j-1].up = UNSAFE;
      }
      else if ( wall_info[i+1][j-1].right == UNKNOWN ) {
        wall_info[i+1][j-1].right = UNSAFE;
        wall_info[i+1][j].left = UNSAFE;
      }
      else if ( wall_info[i+1][j].up == UNKNOWN ) {
        wall_info[i+1][j].up = UNSAFE;
        wall_info[i][j].down = UNSAFE;
      }    
    }
  }
}

void wall::update ( unsigned short int position[], unsigned short int direction,
                    unsigned short int direction_to_update, unsigned short int content ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_update == FRONT ) {
      wall_info[i][j].up = content;
      if ( i != 0 ) {
        wall_info[i-1][j].down = content;
      }
      if ( i > 0 && j > 0 ) {
        side_update( position, UPPER_LEFT );
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
    }
    else if ( direction_to_update == RIGHT ) {
      wall_info[i][j].right = content;
      if ( j != 15 ) {
        wall_info[i][j+1].left = content;
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
    }
    else if ( direction_to_update == LEFT ) {
      wall_info[i][j].left = content;
      if ( j != 0 ) {
        wall_info[i][j-1].right = content;
      }
      if ( i > 0 && j > 0 ) {
        side_update( position, UPPER_LEFT );
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_update == FRONT ) {
      wall_info[i][j].down = content;
      if ( i != 15 ) {
        wall_info[i+1][j].up = content;
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
    }
    else if ( direction_to_update == RIGHT ) {
      wall_info[i][j].left = content;
      if ( j != 0 ) {
        wall_info[i][j-1].right = content;
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
      if ( i > 0 && j > 0 ) {
        side_update( position, UPPER_LEFT );
      }
    }
    else if ( direction_to_update == LEFT ) {
      wall_info[i][j].right = content;
      if ( j != 15 ) {
        wall_info[i][j+1].left = content;
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_update == FRONT ) {
      wall_info[i][j].right = content;
      if ( j != 15 ) {
        wall_info[i][j+1].left = content;
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
    }
    else if ( direction_to_update == RIGHT ) {
      wall_info[i][j].down = content;
      if ( i != 15 ) {
        wall_info[i+1][j].up = content;
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
    }
    else if ( direction_to_update == LEFT ) {
      wall_info[i][j].up = content;
      if ( i != 0 ) {
        wall_info[i-1][j].down = content;
      }
      if ( i > 0 && j > 0 ) {
        side_update( position, UPPER_LEFT );
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_update == FRONT ) {
      wall_info[i][j].left = content;
      if ( j != 0 ) {
        wall_info[i][j-1].right = content;
      }
      if ( i > 0 && j > 0 ) {
        side_update( position, UPPER_LEFT );
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
    }
    else if ( direction_to_update == RIGHT ) {
      wall_info[i][j].up = content;
      if ( i != 0 ) {
        wall_info[i-1][j].down = content;
      }
      if ( i > 0 && j > 0) {
        side_update( position, UPPER_LEFT );
      }
      if ( i > 0 && j < 15 ) {
        side_update( position, UPPER_RIGHT );
      }
    }
    else if ( direction_to_update == LEFT ) {
      wall_info[i][j].down = content;
      if ( i != 15 ) {
        wall_info[i+1][j].up = content;
      }
      if ( i < 15 && j < 15 ) {
        side_update( position, LOWER_RIGHT );
      }
      if ( i < 15 && j > 0 ) {
        side_update( position, LOWER_LEFT );
      }
    }
  }
}

unsigned short int wall::safe_to_go ( unsigned short int position[],
                                      unsigned short int direction, unsigned short int direction_to_go ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].left == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].up == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].right == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].down == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].right == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].down == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].left == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].up == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].up == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].right == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].down == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].left == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].down == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].left == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].up == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].right == SAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return DUMMY;
}

unsigned short int wall::unsafe_to_go ( unsigned short int position[],
                                        unsigned short int direction, unsigned short int direction_to_go ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].left == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].up == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].right == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].down == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == DOWN ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].right == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].down == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].left == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].up == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == RIGHT ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].up == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].right == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].down == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].left == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  else if ( direction == LEFT ) {
    if ( direction_to_go == LEFT ) {
      if ( wall_info[i][j].down == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == UP ) {
      if ( wall_info[i][j].left == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == RIGHT ) {
      if ( wall_info[i][j].up == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if ( direction_to_go == DOWN ) {
      if ( wall_info[i][j].right == UNSAFE ) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return DUMMY;
}

unsigned short int wall::the_count_of_safe_to_go_blocks ( unsigned short int position[], unsigned short int direction ) {
  unsigned short int the_count_of_safe_to_go_blocks = 0;
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( direction == UP ) {
    if ( wall_info[i][j].up == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].right == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].left == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
  }
  else if ( direction == DOWN ) {
    if ( wall_info[i][j].down == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].right == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].left == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
  }
  else if ( direction == RIGHT ) {
    if ( wall_info[i][j].up == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].down == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].right == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
  }
  else if ( direction == LEFT ) {
    if ( wall_info[i][j].up == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].down == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
    if ( wall_info[i][j].left == SAFE ) {
      the_count_of_safe_to_go_blocks += 1;
    }
  }
  return the_count_of_safe_to_go_blocks;
}

unsigned short int wall::the_count_of_unsafe_to_go_blocks ( unsigned short int position[], unsigned short int direction ) {
  return 3 - the_count_of_safe_to_go_blocks( position, direction );
}

unsigned short int wall::need_to_scan ( unsigned short int position[] ) {
  unsigned short int i, j;
  i = position[0];
  j = position[1];
  if ( wall_info[i][j].up == UNKNOWN || wall_info[i][j].down == UNKNOWN ||
       wall_info[i][j].right == UNKNOWN || wall_info[i][j].left == UNKNOWN ) {
    return 1;
  }
  else {
    return 0;
  }
}

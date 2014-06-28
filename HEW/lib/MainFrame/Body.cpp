#include "Body/ADC.cpp"
#include "Body/Motor.cpp"

class body {
  unsigned short int previous_wall_info_r;
  unsigned short int previous_wall_info_l;
  unsigned short int previous_wall_info_ur;
  unsigned short int previous_wall_info_ul;
  void update_aimed_status( unsigned short int update_type );
  unsigned short int get_direction();
  void go_straight_on();
  void turn( unsigned short int turn_type );
  void idle();
  int get_distance_to_go();
  void adj_theta();
  void adj_xy();
public:
  adc scanner;
  motor driver;
  body();
  void init_status();
  void execute( int actions[] );
};

body::body() {
  // Iφ=50MHz, Bφ=25MHz, Pφ=25MHz, MPφ=25MHz
  CPG.FRQCR.WORD = 0x16DB;
  //PinFunctions (p.476)
  //PA
  PFC.PAIORL.WORD = 0xFC27; //1111 1100 0010 0111
  PFC.PACRL4.WORD = 0x0000; //0000 0000 0000 0000
  PFC.PACRL3.WORD = 0x0011; //0000 0000 0001 0001
  PFC.PACRL2.WORD = 0x1101; //0001 0001 0000 0001
  PFC.PACRL1.WORD = 0x1000; //0001 0000 0000 0000
  //PB
  PFC.PBIORH.WORD = 0x0001; //0000 0000 0000 0001
  PFC.PBIORL.WORD = 0x002E; //0000 0000 0010 1110
  PFC.PBCRL1.WORD = 0x0000; //0000 0000 0000 0000
  PFC.PBCRL2.WORD = 0x0000; //0000 0000 0000 0000
  PFC.PBCRH1.WORD = 0x0000; //0000 0000 0000 0000
  //PE
  PFC.PEIORL.WORD = 0xFFFF; //1111 1111 1111 1111
  PFC.PECRL4.WORD = 0x0000; //0000 0000 0000 0000
  PFC.PECRL3.WORD = 0x0101; //0000 0001 0000 0001
  PFC.PECRL2.WORD = 0x0000; //0000 0000 0000 0000
  PFC.PECRL1.WORD = 0x0000; //0000 0000 0000 0000
  init_status();
}

void body::execute(  int actions[] ) {
  unsigned short int current_action = actions[0];
  short int i = 1;
  unsigned short int next_action;
  while( current_action != SCAN && current_action != IDLE ) {
    next_action = actions[i];
    if ( current_action == GO_BACKWARD ) {
      turn( TURN );
    }
    else if ( current_action == GO_RIGHTWARD ) {
      turn( TURN_R_90 );
    }
    else if ( current_action == GO_LEFTWARD ) {
      turn( TURN_L_90 );
    }
    if ( get_distance_to_go() < 45000 ) { //90000 / 2 = 45000
      update_aimed_status( GO_FORWARD_HALF );
    }
    else {
      update_aimed_status( UP );
    }
    if ( next_action != GO_FORWARD ) {
      go_straight_on();
      adj_theta();
      update_aimed_status( GO_FORWARD_HALF );
      go_straight_on();
    }
    current_action = next_action;
    i++;
  }
  if ( current_action == SCAN ) {
    scanner.update_wall_info();
  }
  else if ( current_action == IDLE ) {
    turn( TURN );
    idle();
  }
}

void body::adj_xy() {
  extern unsigned int x;
  extern unsigned int y;
  extern unsigned int dist_ul;
  extern unsigned int dist_ur;
  extern unsigned int dist_l;
  extern unsigned int dist_r;
  unsigned int on2off = 30000; //[μm]
  unsigned int off2on = 55000;
  unsigned int on2off_u = 75000;
  unsigned short int current_direction = get_direction();
  int distance_to_go = get_distance_to_go();
  unsigned int ideal_distance_to_go = distance_to_go - (distance_to_go % 180000);
  unsigned short int flag = 0;
  scanner.update_wall_info();
  //壁切れ補正（横センサ）
  if ( previous_wall_info_r && !scanner.wall.right ) {
    ideal_distance_to_go += on2off;
    flag = 1;
  }
  else if ( !previous_wall_info_r && scanner.wall.right ) {
    ideal_distance_to_go += off2on;
    flag = 1;
  }
  else if ( previous_wall_info_l && !scanner.wall.left ) {
    ideal_distance_to_go += on2off;
    flag = 1;
  }
  else if ( !previous_wall_info_l && scanner.wall.left ) {
    ideal_distance_to_go += off2on;
    flag = 1;
  }
  //壁切れ補正（斜めセンサ）
  if ( scanner.wall.right && previous_wall_info_r &&
       scanner.wall.left && previous_wall_info_l &&
       (!scanner.wall.upper_right || !scanner.wall.upper_left) ) {
    ideal_distance_to_go += on2off_u;
    flag = 1;
  }
  if ( flag &&
       distance_to_go > 20000 &&
       distance_to_go < 80000 ) {
    if ( current_direction == UP ) {
      y = aimed_y - ideal_distance_to_go;
    }
    else if ( current_direction == DOWN ) {
      y = aimed_y + ideal_distance_to_go;
    }
    else if ( current_direction == RIGHT ) {
      x = aimed_x - ideal_distance_to_go;
    }
    else if ( current_direction == LEFT ) {
      x = aimed_x + ideal_distance_to_go;
    }
  }
  //姿勢制御時はコースの中心にいると仮定することによる補正
  if ( (dist_r < 100000 && dist_ur < 220000) ||
       (dist_l < 100000 && dist_ul < 220000) ) {
    if ( current_direction == UP || current_direction == DOWN ) {
      x = aimed_x;
    }
    else if ( current_direction == RIGHT || current_direction == LEFT ) {
      y = aimed_y;
    }
  }
  previous_wall_info_r = scanner.wall.right;
  previous_wall_info_l = scanner.wall.left;
  previous_wall_info_ur = scanner.wall.upper_right;
  previous_wall_info_ul = scanner.wall.upper_left;
}

void body::go_straight_on() {
  extern short int v;
  extern short int a;
  extern short int u_s_up_r;
  extern short int u_s_up_l;
  extern short int u_s_down_r;
  extern short int u_s_down_l;
  extern short int u_s_right_r;
  extern short int u_s_right_l;
  extern short int u_s_left_r;
  extern short int u_s_left_l;
  extern short int aimed_v;
  extern double ddtheta;
  extern double aimed_dtheta;
  extern double aimed_theta;
  extern double theta;
  extern double dtheta;
  extern unsigned int dist_ul;
  extern unsigned int dist_ur;
  extern unsigned int dist_l;
  extern unsigned int dist_r;
  int distance_to_go;
//   short int a_max = 500;//[mm/s/s]
//   short int v_max = 500;//[mm/s]
  short int v_f = 400;
  unsigned short int direction = get_direction();
  ddtheta = 0;
  aimed_dtheta = 0;
  aimed_v = 0;
  scanner.update_wall_info();
  previous_wall_info_r = scanner.wall.right;
  previous_wall_info_l = scanner.wall.left;
  previous_wall_info_ur = scanner.wall.upper_right;
  previous_wall_info_ul = scanner.wall.upper_left;
  do {
    adj_xy();
    distance_to_go = get_distance_to_go();
//     if ( distance_to_go > 225000 ) { //180000 + 90000 / 2 = 225000
//       if ( a_max * ((distance_to_go - 225000) / 1000) < (v - v_f) * (v + v_f) / 2 ) {
// 	a = -a_max;
//       }
//       else if ( v > v_max ) {
// 	a = 0;
// 	aimed_v = v_max;
//       }
//       else {
// 	a = a_max;
// 	if ( abs(dtheta) < 1 &&
// 	     dist_r < 100000 && dist_ur < 220000 &&
// 	     dist_l < 100000 && dist_ul < 220000 ) {
// 	  theta = aimed_theta;
// 	}
//       }
//     }
//     else {
      a = 0;
      aimed_v = v_f;
//    }
    if ( direction == UP ) {
      driver.drive( RIGHT, u_s_up_r );
      driver.drive( LEFT, u_s_up_l );
    }
    else if ( direction == DOWN ) {
      driver.drive( RIGHT, u_s_down_r );
      driver.drive( LEFT, u_s_down_l );
    }
    else if ( direction == RIGHT ) {
      driver.drive( RIGHT, u_s_right_r );
      driver.drive( LEFT, u_s_right_l );
    }
    else if ( direction == LEFT ) {
      driver.drive( RIGHT, u_s_left_r );
      driver.drive( LEFT, u_s_left_l );
    }
  } while( distance_to_go > 0 );
}

void body::turn( unsigned short int turn_type ) {
  extern double ddtheta;
  extern double aimed_dtheta;
  extern double aimed_theta;
  extern double theta;
  extern short int u_t_r;
  extern short int u_t_l;
  double ddtheta_max = 10;//[rad/s/s]
  double dtheta_max = 10;//[rad/s]
  double dtheta_f = 0;
  double angle_to_turn;
  update_aimed_status( turn_type );
  aimed_dtheta = 0;
  do {
    if ( turn_type == TURN || turn_type == TURN_L_90 ) {
      angle_to_turn = aimed_theta - theta;
      if ( ddtheta_max * angle_to_turn < (dtheta - dtheta_f) * (dtheta + dtheta_f) / 2 ) {
	ddtheta = -ddtheta_max;
      }
      else if ( dtheta > dtheta_max ) {
	ddtheta = 0;
	aimed_dtheta = dtheta_max;
      }
      else {
	ddtheta = ddtheta_max;
      }
    }
    else if ( turn_type == TURN_R_90 ) {
      angle_to_turn = theta - aimed_theta;
      if ( ddtheta_max * angle_to_turn < (-dtheta - dtheta_f) * (-dtheta + dtheta_f) / 2 ) {
	ddtheta = ddtheta_max;
      }
      else if ( -dtheta > dtheta_max ) {
	ddtheta = 0;
	aimed_dtheta = -dtheta_max;
      }
      else {
	ddtheta = -ddtheta_max;
      }
    }
    driver.drive( RIGHT, u_t_r );
    driver.drive( LEFT, u_t_l );
  } while( angle_to_turn > 0 );
}

void body::idle() {
  extern unsigned short int initializing_as_solve_mode;
  extern unsigned int dist_rf;
  extern unsigned int dist_lf;
  extern unsigned int dist_r;
  extern unsigned int dist_l;
  extern unsigned int dist_ur;
  extern unsigned int dist_ul;
  driver.brake();
  driver.stop();
  while ( dist_rf > 120000 || dist_lf > 120000 ||
	  dist_r > 120000 || dist_l > 120000 ||
	  dist_ur > 120000 || dist_ul > 120000 );
  initializing_as_solve_mode = 1;
  scanner.led( UPPER_LEFT, 0 );
  scanner.led( LEFT, 0 );
  scanner.led( LEFT_FRONT, 0 );
  scanner.led( RIGHT_FRONT, 0 );
  scanner.led( RIGHT, 0 );
  scanner.led( UPPER_RIGHT, 0 );
  wait(5000000);
  for( int i = 0; i < 10; i++ ) {
    scanner.led( UPPER_LEFT, 0 );
    scanner.led( LEFT, 0 );
    scanner.led( LEFT_FRONT, 1 );
    scanner.led( RIGHT_FRONT, 1 );
    scanner.led( RIGHT, 0 );
    scanner.led( UPPER_RIGHT, 0 );
    wait(50000);
    scanner.led( UPPER_LEFT, 0 );
    scanner.led( LEFT, 1 );
    scanner.led( LEFT_FRONT, 0 );
    scanner.led( RIGHT_FRONT, 0 );
    scanner.led( RIGHT, 1 );
    scanner.led( UPPER_RIGHT, 0 );
    wait(50000);
    scanner.led( UPPER_LEFT, 1 );
    scanner.led( LEFT, 0 );
    scanner.led( LEFT_FRONT, 0 );
    scanner.led( RIGHT_FRONT, 0 );
    scanner.led( RIGHT, 0 );
    scanner.led( UPPER_RIGHT, 1 );
    wait(50000);
  }
  initializing_as_solve_mode = 0;
  init_status();
}

int body::get_distance_to_go() {
  extern unsigned int x;
  extern unsigned int y;
  unsigned short int current_direction = get_direction();
  if ( current_direction == UP ) {
    return aimed_y - y;
  }
  else if ( current_direction == DOWN ) {
    return y - aimed_y;
  }
  else if ( current_direction == RIGHT ) {
    return aimed_x - x;
  }
  else if ( current_direction == LEFT ) {
    return x - aimed_x;
  }
  return DUMMY;
}

void body::init_status() {
  extern unsigned int x;
  extern unsigned int y;
  extern double theta;
  extern unsigned int aimed_x;
  extern unsigned int aimed_y;
  extern short int aimed_v;
  extern short int a;
  extern double aimed_theta;
  extern double aimed_dtheta;
  extern double ddtheta;
  extern unsigned int sample_dist_u_for_go_straight_on;
  extern unsigned int dist_ul;
  wait(1000000);
  sample_dist_u_for_go_straight_on = dist_ul;
  x = 90000;
  y = 90000;
  theta = 1.571;
  aimed_x = 90000;
  aimed_y = 90000;
  aimed_v = 0;
  aimed_theta = 1.571;
  aimed_dtheta = 0;
  a = 0;
  ddtheta = 0;
}

void body::update_aimed_status( unsigned short int update_type ) {
  extern unsigned int aimed_x;
  extern unsigned int aimed_y;
  extern double aimed_theta;
  unsigned short int current_direction = get_direction();
  if ( update_type == UP ) {
    if ( current_direction == UP ) {
      aimed_y += 180000;
    }
    else if ( current_direction == RIGHT ) {
      aimed_x += 180000;
    }
    else if ( current_direction == LEFT ) {
      aimed_x -= 180000;
    }
    else if ( current_direction == DOWN ) {
      aimed_y -= 180000;
    }
  }
  else if ( update_type == GO_FORWARD_HALF ) {
    if ( current_direction == UP ) {
      aimed_y += 90000;
    }
    else if ( current_direction == RIGHT ) {
      aimed_x += 90000;
    }
    else if ( current_direction == LEFT ) {
      aimed_x -= 90000;
    }
    else if ( current_direction == DOWN ) {
      aimed_y -= 90000;
    }
  }
  else if ( update_type == TURN ) {
    aimed_theta += 3.142;
  }
  else if ( update_type == TURN_R_90 ) {
    aimed_theta -= 1.571;
  }
  else if ( update_type == TURN_L_90 ) {
    aimed_theta += 1.571;
  }
}

unsigned short int body::get_direction() {
  extern double theta;
  short int deg = rad2deg( theta ); //-180 < deg < 180
  if ( deg < -135 ) {
    return LEFT;
  }
  else if ( -135 <= deg && deg < -45 ) {
    return DOWN;
  }
  else if ( -45 <= deg && deg < 45 ) {
    return RIGHT;
  }
  else if ( 45 <= deg && deg < 135 ) {
    return UP;
  }
  else if ( 135 <= deg ) {
    return LEFT;
  }
  return DUMMY;
}

void body::adj_theta() {
  extern double theta;
  extern double aimed_theta;
  extern unsigned int dist_rf;
  extern unsigned int dist_lf;
  extern double ddtheta;
  extern double aimed_dtheta;
  extern short int u_t_r;
  extern short int u_t_l;
  ddtheta = 0;
  aimed_dtheta = 0;
  if ( dist_rf < 180000 && dist_lf < 180000 ) {
    while( abs((int)dist_rf - (int)dist_lf) > 1000 ) {
      if ( dist_rf > dist_lf ) {
	aimed_dtheta = 3;
      }
      else {
	aimed_dtheta = -3;
      }
      driver.drive( RIGHT, u_t_r );
      driver.drive( LEFT, u_t_l );
    }
    theta = aimed_theta;
  }
}

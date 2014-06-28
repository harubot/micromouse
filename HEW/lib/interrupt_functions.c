#include "math.h"
#include "iodefine.h"
#include "MainFrame/definitions.h"
#include "sub_functions.c"
#define PROPORTIONALITY_FACTOR 43.7 //[mm/(pulse*s)]  (pi * 21[mm] / (256 (分解能) * 3 (ギア比1:3)) * 10**3 / 2[ms]) + 補正
#define TREAD 72.5 //[mm]

/* INTERRUPT FUNCTIONS */
void int_mtu2_4_tgia4() { //200μsごとに実行（割り込みレベル13）
  extern short int v_r;
  extern short int v_l;
  extern short int aimed_v;
  extern short int a;
  extern double theta;
  extern double aimed_theta;
  extern double dtheta;
  extern double aimed_dtheta;
  extern double ddtheta;
  extern unsigned int dist_ul;
  extern unsigned int dist_ur;
  extern unsigned int dist_l;
  extern unsigned int dist_r;
  extern unsigned int sample_dist_u_for_go_straight_on;
  extern short int u_s_up_r;
  extern short int u_s_up_l;
  extern short int u_s_down_r;
  extern short int u_s_down_l;
  extern short int u_s_right_r;
  extern short int u_s_right_l;
  extern short int u_s_left_r;
  extern short int u_s_left_l;
  extern unsigned int aimed_x;
  extern unsigned int aimed_y;
  extern unsigned int x;
  extern unsigned int y;
  extern short int u_t_r;
  extern short int u_t_l;
  extern double previous_theta;
  extern short int v;
  short int u_x_r;
  short int u_x_l;
  short int u_y_r;
  short int u_y_l;
  short int u_v_r;
  short int u_v_l;
  short int u_theta_r;
  short int u_theta_l;
  short int u_dtheta_r;
  short int u_dtheta_l;
  short int u_dist_r;
  short int u_dist_l;
  short int P = 4;
  short int D = 1;
  short int AMP_v = 1;
  short int AMP_theta = 2000;
  short int AMP_dtheta = 30;
  double AMP_y = 0;
  double AMP_x = 0;
/*    double AMP_y = 0.0168; */
/*    double AMP_x = 0.0168; */
  double AMP_dist_u1 = 0.004;
  double AMP_dist_u2 = 0.0077;
  double AMP_dist_u3 = 0.004;
  double AMP_dist_side = 0.006;
  short int G_s = 3;
  short int G_t = 10;
  u_y_r = P * ((aimed_y - y) * AMP_y);
  u_y_l = -u_y_r;
  u_x_r = P * ((aimed_x - x) * AMP_x);
  u_x_l = -u_x_r;
  u_v_r = ((P * (aimed_v - v_r)) + (D * a)) * AMP_v;
  u_v_l = ((P * (aimed_v - v_l)) + (D * a)) * AMP_v;
  u_theta_r = ((P * (aimed_theta - theta) + (D * aimed_dtheta))) * AMP_theta;
  u_theta_l = -u_theta_r;
  previous_theta = theta;
  u_dtheta_r = ((P * (aimed_dtheta - (v_r  * 2 / TREAD))) + (D * ddtheta)) * AMP_dtheta;
  u_dtheta_l = ((P * ((-v_l * 2 / TREAD) - aimed_dtheta)) - (D * ddtheta)) * AMP_dtheta;
  if ( dist_r < 100000 && dist_ur < 220000 &&
       dist_l < 100000 && dist_ul < 220000 ) {
    u_dist_r = P * ( ((int)dist_ul - (int)dist_ur) * AMP_dist_u1 );
    u_dist_l = -u_dist_r;
  }
  else if ( dist_r < 100000 && dist_ur < 220000 ) {
    u_dist_r = P * ( ((int)sample_dist_u_for_go_straight_on - (int)dist_ur) * AMP_dist_u2 );
    u_dist_l = -u_dist_r;
  }
  else if ( dist_l < 100000 && dist_ul < 220000 ) {
    u_dist_r = P * ( ((int)dist_ul - (int)sample_dist_u_for_go_straight_on) * AMP_dist_u2 );
    u_dist_l = -u_dist_r;
  }
  else if ( dist_r < 100000 && dist_l < 100000 ) {
    u_dist_r = P * ( ((int)dist_l - (int)dist_r) * AMP_dist_side );
    u_dist_l = -u_dist_r;
  }
  else if ( dist_ur < 220000 && dist_ul < 220000 ) {
    u_dist_r = P * ( ((int)dist_ur - (int)dist_ul) * AMP_dist_u3 );
    u_dist_l = -u_dist_r;
  }
  else {
    u_dist_r = 0;
    u_dist_l = 0;
  }
  u_s_up_r = ( -u_x_r + u_v_r + u_theta_r + u_dist_r ) * G_s;
  u_s_up_l = ( -u_x_l + u_v_l + u_theta_l + u_dist_l ) * G_s;
  u_s_down_r = ( u_x_r + u_v_r + u_theta_r + u_dist_r ) * G_s;
  u_s_down_l = ( u_x_l + u_v_l + u_theta_l + u_dist_l ) * G_s;
  u_s_right_r = ( u_y_r + u_v_r + u_theta_r + u_dist_r ) * G_s;
  u_s_right_l = ( u_y_l + u_v_l + u_theta_l + u_dist_l ) * G_s;
  u_s_left_r = ( -u_y_r + u_v_r + u_theta_r + u_dist_r ) * G_s;
  u_s_left_l = ( -u_y_l + u_v_l + u_theta_l + u_dist_l ) * G_s;
  u_t_r = ( u_dtheta_r ) * G_t;
  u_t_l = ( u_dtheta_l ) * G_t;
  MTU24.TSR.BIT.TGFA = 0;
}

void int_mtu2_0_tgia0() { //2msごとに実行（割り込みレベル14）
  extern unsigned int x;
  extern unsigned int y;
  extern short int v;
  extern short int v_r;
  extern short int v_l;
  extern double theta;
  extern double dtheta;
  extern short int a;
  extern short int aimed_v;
  extern double ddtheta;
  extern double aimed_dtheta;
  extern unsigned short int next_direction_to_scan_r;
  extern unsigned short int next_direction_to_scan_l;
  extern unsigned short int cnt_for_adc;
  extern double previous_theta;
  unsigned short int limitter = 1;
  //座標計算
  v_r = MTU22.TCNT;
  v_r *= PROPORTIONALITY_FACTOR;
  MTU22.TCNT = 0;
  v_l = MTU21.TCNT;
  v_l *= PROPORTIONALITY_FACTOR;
  MTU21.TCNT = 0;
  v = (v_r + v_l) / 2;
  dtheta = ((double)v_r -(double)v_l) / (double)TREAD; //[rad/s]
  previous_theta = theta;
  theta += dtheta * 0.002; //[rad]
  x += ((double)v_r + (double)v_l) * cos( theta ); //[μm]
  y += ((double)v_r + (double)v_l) * sin( theta );
  //目標速度計算
  aimed_v += a / 500;
  aimed_dtheta += ddtheta / 500;
  //A/D変換（周期は"limitter"の値で調整可能）
  if ( cnt_for_adc > limitter ) {
    if ( !adc_is_busy( next_direction_to_scan_r ) ) {
      scan( next_direction_to_scan_r );
    }
    if ( !adc_is_busy( next_direction_to_scan_l ) ) {
      scan( next_direction_to_scan_l );
    }
    cnt_for_adc = 0;
  }
  cnt_for_adc++;
  MTU20.TSR.BIT.TGFA = 0;
}

void int_ad1_adi1() { //RIGHT（割り込みレベル15）
  extern unsigned short int next_direction_to_scan_r;
  extern unsigned int dist_ur;//[μm]
  extern unsigned int dist_r;
  extern unsigned int dist_rf;
  extern unsigned short int adc_rf;
  extern unsigned short int adc_r;
  extern unsigned short int adc_ur;
  led( next_direction_to_scan_r, 0 );
  if ( next_direction_to_scan_r == UPPER_RIGHT ) {
    adc_ur = AD1.ADDR5;
    dist_ur = adc2dist( AD1.ADDR5, UPPER_RIGHT );
    next_direction_to_scan_r = RIGHT;
  }
  else if ( next_direction_to_scan_r == RIGHT ) {
    adc_r = AD1.ADDR6;
    dist_r = adc2dist( AD1.ADDR6, RIGHT );
    next_direction_to_scan_r = RIGHT_FRONT;
  }
  else if ( next_direction_to_scan_r == RIGHT_FRONT ) {
    adc_rf = AD1.ADDR4;
    dist_rf = adc2dist( AD1.ADDR4, RIGHT_FRONT );
    next_direction_to_scan_r = UPPER_RIGHT;
  }
  AD1.ADCSR.BIT.ADF = 0;
}

void int_ad0_adi0() { //LEFT（割り込みレベル15）
  extern unsigned short int next_direction_to_scan_l;
  extern unsigned int dist_ul;//[μm]
  extern unsigned int dist_l;
  extern unsigned int dist_lf;
  extern unsigned short int adc_ul;
  extern unsigned short int adc_l;
  extern unsigned short int adc_lf;
  led( next_direction_to_scan_l, 0 );
  if ( next_direction_to_scan_l == UPPER_LEFT ) {
    adc_ul = AD0.ADDR1;
    dist_ul = adc2dist( AD0.ADDR1, UPPER_LEFT );
    next_direction_to_scan_l = LEFT;
  }
  else if ( next_direction_to_scan_l == LEFT ) {
    adc_l = AD0.ADDR2;
    dist_l = adc2dist( AD0.ADDR2, LEFT );
    next_direction_to_scan_l = LEFT_FRONT;
  }
  else if ( next_direction_to_scan_l == LEFT_FRONT ) {
    adc_lf = AD0.ADDR0;
    dist_lf = adc2dist( AD0.ADDR0, LEFT_FRONT );
    next_direction_to_scan_l = UPPER_LEFT;
  }
  AD0.ADCSR.BIT.ADF = 0;
}

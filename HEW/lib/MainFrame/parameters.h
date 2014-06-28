#include "definitions.h"

/* SHARED VARIABLES WITH INTERRUPT FUNCTIONS */
unsigned int x;//[ﾎｼm]
unsigned int y;
short int v = 0;//[mm/s]
short int v_r = 0;
short int v_l = 0;
double theta; //[rad]
double dtheta = 0; //[rad/s]
short int a; //[mm/s/s]
unsigned int aimed_x;
unsigned int aimed_y;
short int aimed_v;
double aimed_theta;
double aimed_dtheta;
double ddtheta;//[rad/s/s]
unsigned short int initializing_as_solve_mode = 0;
unsigned short int next_direction_to_scan_r = UPPER_RIGHT;
unsigned short int next_direction_to_scan_l = UPPER_LEFT;
unsigned short int cnt_for_adc = 0;
unsigned int dist_ul = 0;//[ﾎｼm]
unsigned int dist_l = 0;
unsigned int dist_lf = 0;
unsigned int dist_ur = 0;
unsigned int dist_r = 0;
unsigned int dist_rf = 0;
unsigned int sample_dist_u_for_go_straight_on;
unsigned short int adc_ul = 0;
unsigned short int adc_l = 0;
unsigned short int adc_lf = 0;
unsigned short int adc_rf = 0;
unsigned short int adc_r = 0;
unsigned short int adc_ur = 0;
short int u_s_up_r = 0;
short int u_s_up_l = 0;
short int u_s_down_r = 0;
short int u_s_down_l = 0;
short int u_s_right_r = 0;
short int u_s_right_l = 0;
short int u_s_left_r = 0;
short int u_s_left_l = 0;
short int u_t_r = 0;
short int u_t_l = 0;
double previous_theta = 0;

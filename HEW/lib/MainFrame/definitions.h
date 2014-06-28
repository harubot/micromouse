/* WALL STATE DEFINITIONS */
#define SAFE    0
#define UNSAFE  1
#define UNKNOWN 2

/* ACTION DEFINITIONS */
#define SCAN		3
#define GO_FORWARD	4
#define GO_BACKWARD	5
#define GO_RIGHTWARD	6
#define GO_LEFTWARD	7
#define GO_UPPER_RIGHT	8
#define GO_UPPER_LEFT	9
#define GO_LOWER_RIGHT	10
#define GO_LOWER_LEFT	11
#define TURN_R_45	12
#define TURN_R_90	13
#define TURN_R_135	14
#define TURN_L_45	15
#define TURN_L_90	16
#define TURN_L_135	17
#define TURN		18
#define IDLE		19
#define GO_FORWARD_HALF 20

/* DIRECTION DEFINITIONS */
#define RIGHT_FRONT 21
#define LEFT_FRONT  22
#define UP          23
#define FRONT       23
#define DOWN        24
#define BACK        24
#define RIGHT       25
#define LEFT        26
#define UPPER_RIGHT 27
#define UPPER_LEFT  28
#define LOWER_RIGHT 29
#define LOWER_LEFT  30

/* MODE DEFINITIONS */
#define SEARCH  31
#define SOLVE   32
#define NORMAL  33
#define REVERSE 34

/* TYPE DEFINITIONS */
#define MAZE    35
#define VIRTUAL 36

/* GOAL AREA DEFINITIONS */
#define GOAL_I_MIN 7
#define GOAL_I_MAX 8
#define GOAL_J_MIN 7
#define GOAL_J_MAX 8
/* #define GOAL_I_MIN 7 */
/* #define GOAL_I_MAX 8 */
/* #define GOAL_J_MIN 3 */
/* #define GOAL_J_MAX 4 */

/* OTHERS */
#define DUMMY 37

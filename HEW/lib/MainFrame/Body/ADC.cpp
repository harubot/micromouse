class adc {
public:
  struct {
    unsigned short int front;
    unsigned short int right;
    unsigned short int left;
    unsigned short int upper_right;
    unsigned short int upper_left;
  } wall;
  adc();
  void update_wall_info();
  void led ( unsigned short int direction, unsigned short int status );
};

adc::adc() {
  STB.CR4.BIT._AD1 = 0; //AD1起動
  STB.CR4.BIT._AD0 = 0; //AD0起動
  AD1.ADCSR.WORD = 0x40C0; //0100 0000 1100 0000
  AD0.ADCSR.WORD = 0x40C0; //0100 0000 1100 0000
  INTC.IPRK.BIT._AD01 = 15;
}

void adc::update_wall_info() {
  extern unsigned int dist_l;
  extern unsigned int dist_lf;
  extern unsigned int dist_rf;
  extern unsigned int dist_r;
  extern unsigned int dist_ur;
  extern unsigned int dist_ul;
  if ( dist_rf < 180000 || dist_lf < 180000 ) {
    wall.front = UNSAFE;
  }
  else {
    wall.front = SAFE;
  }
  if ( dist_r < 150000 ) {
    wall.right = UNSAFE;
  }
  else {
    wall.right = SAFE;
  }
  if ( dist_l < 150000 ) {
    wall.left = UNSAFE;
  }
  else {
    wall.left = SAFE;
  }
  if ( dist_ur < 250000 ) {
    wall.upper_right = UNSAFE;
  }
  else {
    wall.upper_right = SAFE;
  }
  if ( dist_ul < 250000 ) {
    wall.upper_left = UNSAFE;
  }
  else {
    wall.upper_left = SAFE;
  }
}

void adc::led ( unsigned short int direction, unsigned short int status ) {
  if ( direction == RIGHT_FRONT ) {
    PA.DRL.BIT.B13 = status;
  }
  else if ( direction == LEFT_FRONT ) {
    PA.DRL.BIT.B12 = status;
  }
  else if ( direction == UPPER_RIGHT ) {
    PA.DRL.BIT.B15 = status;
  }
  else if ( direction == UPPER_LEFT ) {
    PA.DRL.BIT.B10 = status;
  }
  else if ( direction == RIGHT ) {
    PA.DRL.BIT.B14 = status;
  }
  else if ( direction == LEFT ) {
    PA.DRL.BIT.B11 = status;
  }
}


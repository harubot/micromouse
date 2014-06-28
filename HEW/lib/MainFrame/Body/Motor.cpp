class motor {
public:
  motor(); 
  void drive( unsigned short int motor_id, unsigned short int direction_to_drive, unsigned short int duty );
  void drive( unsigned short int motor_id, short int power ); // -32767 < power < 32767
  void brake();
  void stop();
  unsigned short int encoder_value( unsigned short int encoder_id );
};

motor::motor() {
  STB.CR4.BIT._MTU2 = 0;
  //PWM
  MTU23.TMDR.BIT.MD = 2;
  MTU23.TCR.BIT.CCLR = 1;
  MTU23.TCR.BIT.CKEG = 0;
  MTU23.TCR.BIT.TPSC = 0; //MPφ /1 でカウント(=>25MHz)
  MTU23.TIOR.BIT.IOA = 1;
  MTU23.TIOR.BIT.IOB = 2;
  MTU23.TIOR.BIT.IOC = 1;
  MTU23.TIOR.BIT.IOD = 2;
  stop();
  MTU2.TSTR.BIT.CST3 = 1;
  //Encoder
  MTU21.TMDR.BIT.MD = 4;
  MTU22.TMDR.BIT.MD = 4;
  MTU2.TSTR.BIT.CST1 = 1;
  MTU2.TSTR.BIT.CST2 = 1;
  //Timer-LEVEL15
  INTC.IPRD.BIT._MTU20G = 14;
  MTU20.TCR.BIT.CCLR = 1;
  MTU20.TCR.BIT.CKEG = 0;
  MTU20.TCR.BIT.TPSC = 0; //MPφ /1 でカウント(=>25MHz)
  MTU20.TGRA = 50000;     //25MHz * 2ms = 50000
  MTU20.TIER.BIT.TGIEA = 1;
  MTU2.TSTR.BIT.CST0 = 1;
  //Timer-LEVEL14
  INTC.IPRF.BIT._MTU24G = 13;
  MTU24.TCR.BIT.CCLR = 1;
  MTU24.TCR.BIT.CKEG = 0;
  MTU24.TCR.BIT.TPSC = 0; //MPφ /1 でカウント(=>25MHz)
  MTU24.TGRA = 5000;      //25MHz * 200us = 5000
  MTU24.TIER.BIT.TGIEA = 1;
  MTU2.TSTR.BIT.CST4 = 1;
}

void motor::drive( unsigned short int motor_id, unsigned short int direction_to_drive, unsigned short int duty ) {
  unsigned short int cycle_length = 1250; //単位:clock. 1secに20000サイクルまわす。(25MHz / 20kHz = 1250clock)
  unsigned short int cmp;
  PE.DRL.BIT.B2 = 1;
  PE.DRL.BIT.B3 = 1;
  cmp = cycle_length * duty / 100;
  if ( motor_id == RIGHT ) {
    MTU23.TGRC = cycle_length;
    MTU23.TGRD = cmp;
    if ( direction_to_drive == FRONT ) {
      PE.DRL.BIT.B1 = 1;
    }
    else {
      PE.DRL.BIT.B1 = 0;
    }
  }
  else {
    MTU23.TGRA = cycle_length;
    MTU23.TGRB = cmp;
    if ( direction_to_drive == FRONT ) {
      PE.DRL.BIT.B0 = 1;
    }
    else {
      PE.DRL.BIT.B0 = 0;
    }
  }
}

void motor::drive( unsigned short int motor_id, short int power ) {
  unsigned short int duty = abs( power / 328 ); //-100 <  (power/32767)*100 < 100
  if ( power > 0 ) {
    drive ( motor_id, FRONT, duty );
  }
  else {
    drive ( motor_id, BACK, duty );
  }
}

void motor::brake() {
  PE.DRL.BIT.B2 = 1;
  PE.DRL.BIT.B3 = 0;
}

void motor::stop() {
  drive( RIGHT, FRONT, 0 );
  drive( LEFT, FRONT, 0 );
  PE.DRL.BIT.B2 = 0;
}

unsigned short int motor::encoder_value( unsigned short int encoder_id ) {
  if ( encoder_id == RIGHT ) {
    return MTU22.TCNT;
  }
  else {
    return MTU21.TCNT;
  }
}

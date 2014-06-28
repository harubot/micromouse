/* SUB FUNCTIONS */
void wait( unsigned int i ) {
  volatile unsigned int j;
  for( j = 0; j < i; j++ );
}

void led ( unsigned short int direction, unsigned short int status ) {
  extern unsigned short int initializing_as_solve_mode;
  if ( !initializing_as_solve_mode ) {
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
}

void scan( unsigned short int direction_to_scan ) {
  unsigned short int wait_time = 180;
  AD1.ADCSR.BIT.ADIE = AD0.ADCSR.BIT.ADIE = 1;
  if ( direction_to_scan == RIGHT_FRONT ) {
    AD1.ADCSR.BIT.CH = 0; //0b000
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD1.ADCR.BIT.ADST = 1;
  }
  else if ( direction_to_scan == LEFT_FRONT ) {
    AD0.ADCSR.BIT.CH = 0; //0b000
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD0.ADCR.BIT.ADST = 1;
  }
  else if ( direction_to_scan == RIGHT ) {
    AD1.ADCSR.BIT.CH = 2; //0b010;
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD1.ADCR.BIT.ADST = 1;
  }
  else if ( direction_to_scan == LEFT ) {
    AD0.ADCSR.BIT.CH = 2; //0b010
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD0.ADCR.BIT.ADST = 1;
  }
  else if ( direction_to_scan == UPPER_RIGHT ) {
    AD1.ADCSR.BIT.CH = 1; //0b001
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD1.ADCR.BIT.ADST = 1;
  }
  else if ( direction_to_scan == UPPER_LEFT ) {
    AD0.ADCSR.BIT.CH = 1; //0b001
    led( direction_to_scan, 1 );
    wait( wait_time );
    AD0.ADCR.BIT.ADST = 1;
  }
}

unsigned short int adc_is_busy( unsigned short int direction_to_check ) {
  if ( direction_to_check == RIGHT_FRONT ||
       direction_to_check == RIGHT ||
       direction_to_check == UPPER_RIGHT
  ) {
    if ( AD1.ADCR.BIT.ADST ) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    if ( AD0.ADCR.BIT.ADST ) {
      return 1;
    }
    else {
      return 0;
    }
  }
}

unsigned int adc2dist( unsigned short int adc_result, unsigned short int adc_id ) {
  unsigned short int env_value = 1;
  unsigned int x = adc_result / 100;
  if ( adc_id == UPPER_LEFT ) {
    return env_value * ( 15597835 * pow(x, -0.970) );
  }
  else if ( adc_id == LEFT ) {
    return env_value * ( 11525012 * pow(x, -1.022) );
  }
  else if ( adc_id == LEFT_FRONT ) {
    return env_value * ( 15597835 * pow(x, -0.970) );
  }
  else if ( adc_id == RIGHT_FRONT ) {
    return env_value * ( 13628777 * pow(x, -0.927) );
  }
  else if ( adc_id == RIGHT ) {
    return env_value * ( 26679868 * pow(x, -1.146) );
  }
  else if ( adc_id == UPPER_RIGHT ) {
    return env_value * ( 13628777 * pow(x, -0.927) );
  }
}

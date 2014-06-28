void wait( unsigned int i ) {
  volatile unsigned int j;
  for( j = 0; j < i; j++ );
}

short int rad2deg ( double rad ) { //-180 < deg < 180
  if ( rad > 0 ) {
    while( rad > 3.2 ) {
      rad -= 6.283;
    }
  }
  else {
    while ( rad < -3.2 ) {
      rad += 6.283;
    }
  }
  rad *= 57.3; //180 / pi = 57.30
  return rad;
}

#ifdef __IS_NOT_SIMULATOR_MODE__
unsigned int abs( int num ) {
  if ( num > 0 ) {
    return num;
  }
  else {
    return -num;
  }
}

double abs( double dnum ) {
  if ( dnum > 0 ) {
    return dnum;
  }
  else {
    return -dnum;
  }
}
#endif


#include <stdio.h>
void send_char( char data ) {
  while ( SCI1.SCSSR.BIT.TDRE == 0 ); //未送信データが送られるまで待つ
  SCI1.SCTDR = data;                  //送信データのセット
  SCI1.SCSSR.BIT.TDRE = 0;            //送信フラグのクリア
}

void print( char *str ) {
  while ( *str != '\0' ) { //文字が\0になるまで繰り返す 
    send_char( *str );     //1文字送信
    str++;                 //次の文字に移る
  }
  send_char( ' ' );
  send_char( ' ' );
}

void print( int num ) {
  char str[32]; //32bit
  sprintf( str, "%d", num );
  print( str );
}

void print( unsigned int num ) {
  print( (int)num );
}

void print( double dnum ) {
  char str[64]; //64bit
  sprintf( str, "%lf", dnum );
  print( str );  
}

void nl() {
  send_char( '\r' );
  send_char( '\n' );
}

void init_serial() {
  STB.CR3.BIT._SCI1 = 0;   //SCI_1起動
  SCI1.SCSCR.BYTE = 0x00;  //初期化 (p.409)
  SCI1.SCSMR.BYTE = 0x00;  //調歩同期式モード
  SCI1.SCSDCR.BYTE = 0xF2; //フォーマット設定
  SCI1.SCBRR = 80;         //ボーレート:9600bps, Pφ=25MHzの場合の計算値。
  wait( 100000 );
  wait( 100000 );
  SCI1.SCSCR.BYTE = 0x20; //TE=1
  nl();
  nl();
  nl();
}


#ifndef   __IS_NOT_SIMULATOR_MODE__
#include <iostream>
void warn () {
  using namespace std;
  cout << "something is wrong..." << endl;
}

void warn ( char *str ) {
  using namespace std;
  cout << str << endl;
}

void warn ( unsigned int i ) {
  using namespace std;
  cout << i << endl;
}

void die () {
  using namespace std;
  cout << "died..." << endl;
  abort();
}

void die ( char *str ) {
  using namespace std;
  cout << str << endl;
  abort();
}

void die ( unsigned int i ) {
  using namespace std;
  cout << i << endl;
  abort();
}
#endif

#!/usr/bin/perl
#
# マイクロマウス競技における仮想迷路 (Maze.pm) 上でC++思考ルーチンを走らせるためのPerlスクリプト
#
use strict;
use warnings;
use lib 'lib';
use Inline CPP => 'DATA';
use Inline 'Force';
use Inline (CPP => Config => AUTO_INCLUDE => '#include "HEW/lib/MainFrame.cpp"');
use Getopt::Long;
use Time::HiRes qw(usleep);
use Maze;

my $maze_path;
my $msleep_time = 300;
GetOptions (
    'maze=s'   => \$maze_path,
    'msleep=i' => \$msleep_time,
);
die 'usage: perl maze.pl --maze=maze/foobar.txt [--msleep=num]'
    if not $maze_path;

# C++で実装した迷路解析AIをインスタンス化
my $mouse = mouseInterface->new();

# 自作モジュールによる仮想迷路をAIに解かせる
my $trial_times = 2; # マイクロマウス大会ルールに従う。一回目は全面探索。二回目はタイムアタック
for ( my $i = 0; $i < $trial_times; $i++ ) {
    my $maze = Maze->new( maze_path => $maze_path ); # 仮想迷路インスタンス化
    while ( !$maze->finished and !$maze->failed ) { # 仮想迷路探索が終了(or失敗)するまでループ
        my $next_action = $mouse->next_action(); # AIが取得した迷路情報から次のアクションを考える
        if ( $next_action eq 'scan' ) { # 'scan'(周囲迷路情報取得アクション)の場合、仮想迷路からの情報をAIに与える
            my $wall = $maze->wall;
            $mouse->update( $wall->{front}, $wall->{left}, $wall->{right} );
        }
        else { # それ以外の走行アクションの場合、仮想迷路上のロボットの位置を更新し標準出力に出力
            $maze->update( $next_action );
            system 'clear';
            print "Action: $next_action\n";
            $maze->print;
            usleep ( $msleep_time * 10**3 ); # 出力された現在状況を人間が目で確認するためにしばらくSleep
        }
    }
    last if $maze->failed; # 先程の全面探索ループが成功した場合のみタイムアタックに進める
    $mouse->init_as_solve_mode(); # 迷路情報を保持したままタイムアタック用にAIをリセット
}

__DATA__
__CPP__

/* マイクロマウスAI(C++)のPerlコードへのインタフェースクラス */
class mouseInterface : private mainFrame {
public:
    char *next_action();
    void update( int front, int left, int right );
    void init_as_solve_mode();
};

/* 既知迷路情報から次のアクションを導出 */
char *mouseInterface::next_action() {
    int next_action = thinker.next_action();
    if ( next_action == SCAN ) {
        return "scan";
    }
    else if ( next_action == GO_FORWARD ) {
        return "go_forward";
    }
    else if ( next_action == GO_BACKWARD ) {
        return "go_backward";
    }
    else if ( next_action == GO_RIGHTWARD ) {
        return "go_rightward";
    }
    else if ( next_action == GO_LEFTWARD ) {
        return "go_leftward";
    }
    else if ( next_action == TURN_R_90 ) {
        return "turn_r_90";
    }
    else if ( next_action == TURN_L_90 ) {
        return "turn_l_90";
    }
    else if ( next_action == TURN ) {
        return "turn";
    }
    else if ( next_action == IDLE ) {
        return "idle";
    }
}

/* 'scan'アクションにより得られた迷路情報をメモリに格納 */
void mouseInterface::update( int front, int left, int right ) {
    if ( front ) {
        controller.scanner.wall.front = UNSAFE;
    }
    else {
        controller.scanner.wall.front = SAFE;
    }
    if ( left ) {
        controller.scanner.wall.left = UNSAFE;
    }
    else {
        controller.scanner.wall.left = SAFE;
    }
    if ( right ) {
        controller.scanner.wall.right = UNSAFE;
    }
    else {
        controller.scanner.wall.right = SAFE;
    }
    thinker.update_memorized_maze( controller.scanner );
}

/* 迷路情報を保持したままタイムアタックモードにリセット */
void mouseInterface::init_as_solve_mode () {
    thinker.init_as_solve_mode();
}

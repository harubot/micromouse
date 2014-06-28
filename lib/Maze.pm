#
# マイクロマウス競技の迷路を表現するPerlモジュール
#
package Maze;
use Moose;
use IO::File;

# 迷路情報 (テキスト形式) の参照場所
has 'maze_path' => (
    is        => 'ro',
    isa       => 'Str',
    required  => 1,
);

# 迷路壁情報 (_build_mazeにより格納)
has 'maze' => (
    is         => 'ro',
    isa        => 'ArrayRef',
    required   => 1,
    lazy_build => 1,
);

# 仮想迷路探索を終了したかどうか
has 'finished' => (
    is  => 'rw',
    isa => 'Bool',
);

# 仮想迷路探索に失敗したかどうか
has 'failed' => (
    is  => 'rw',
    isa => 'Bool',
);

# マイクロマウスの位置情報
has 'i' => (
    is      => 'rw',
    isa     => 'Int',
    default => '15',
);
has 'j' => (
    is      => 'rw',
    isa     => 'Int',
    default => '0',
);

# マイクロマウスの方向
has 'angle' => (
    is      => 'rw',
    isa     => 'Int',
    default => '0',
);

# マイクロマウスの通った経路情報
has 'route' => (
    is => 'rw',
    isa => 'ArrayRef',
    default => sub {
        my @route;
        $route[15][0] = 'up';
        return \@route;
    },
);

# テキスト形式迷路情報から壁情報を取得しmazeプロパティに格納
sub _build_maze {
    my $self = shift;
    my $fh = IO::File->new( $self->maze_path, 'r' );
    my @maze;
    while ( <$fh> ) {
        chomp;
        push @maze, [ split( //, $_ ) ];
    }
    my @arranged;
    for( my $i = 0; $i < 16; $i++ ) {
        for( my $j = 0; $j < 16; $j++ ) {
            if( $maze[$i][$j*2+1] eq '_' ) {
                $arranged[$i][$j]->{up} = 1;
            }
            else {
                $arranged[$i][$j]->{up} = 0;
            }
            if( $maze[$i+1][$j*2+1] eq '_' ) {
                $arranged[$i][$j]->{down} = 1;
            }
            else {
                $arranged[$i][$j]->{down} = 0;
            }
            if( $maze[$i+1][$j*2] eq '|' ) {
                $arranged[$i][$j]->{left} = 1;
            }
            else {
                $arranged[$i][$j]->{left} = 0;
            }
            if( $maze[$i+1][$j*2+2] eq '|' ) {
                $arranged[$i][$j]->{right} = 1;
            }
            else {
                $arranged[$i][$j]->{right} = 0;
            }
        }
    }
    return \@arranged;
}

__PACKAGE__->meta->make_immutable;
no Moose;

# マイクロマウスの現在角度を返す
sub direction {
    my $self = shift;
    my $direction = $self->angle % 4;
    if ( $direction == 0 ) {
        return 'up';
    }
    elsif ( $direction == 1 ) {
        return 'left';
    }
    elsif ( $direction == 2 ) {
        return 'down';
    }
    elsif ( $direction == 3 ) {
        return 'right';
    }
}

# マイクロマウスの周囲壁情報を返す
sub wall {
    my $self = shift;
    my ( $i, $j ) = ( $self->i , $self->j );
    my $direction = $self->direction;
    my $maze = $self->maze;
    if ( $direction eq 'up' ) {
        return {
            front => $maze->[$i][$j]->{up},
            left  => $maze->[$i][$j]->{left},
            right => $maze->[$i][$j]->{right},
        };
    }
    elsif ( $direction eq 'down' ) {
        return {
            front => $maze->[$i][$j]->{down},
            left  => $maze->[$i][$j]->{right},
            right => $maze->[$i][$j]->{left},
        };
    }
    elsif ( $direction eq 'right' ) {
        return {
            front => $maze->[$i][$j]->{right},
            left  => $maze->[$i][$j]->{up},
            right => $maze->[$i][$j]->{down},
        };
    }
    elsif ( $direction eq 'left' ) {
        return {
            front => $maze->[$i][$j]->{left},
            left  => $maze->[$i][$j]->{down},
            right => $maze->[$i][$j]->{up},
        };
    }
}

# マイクロマウスの位置情報を更新
sub update {
    my ( $self, $action ) = @_;
    my ( $i, $j ) = ( $self->i, $self->j );
    if ( $action eq 'go_forward' ) {
        if ( $self->wall->{front} ) { # 全面壁の時に前進アクションをしたら探索失敗
            $self->failed(1);
            return;
        }
        my $direction = $self->direction;
        if ( $direction eq 'up' ) {
            $self->i( $i-1 );
        }
        elsif ( $direction eq 'down' ) {
            $self->i( $i+1 );
        }
        elsif ( $direction eq 'left' ) {
            $self->j( $j-1 );
        }
        elsif ( $direction eq 'right' ) {
            $self->j( $j+1 );
        }
    }
    elsif ( $action eq 'go_backward' ) {
        $self->update( 'turn' );
        $self->update( 'go_forward' );
    }
    elsif ( $action eq 'go_leftward' ) {
        $self->update( 'turn_l_90' );
        $self->update( 'go_forward' );
    }
    elsif ( $action eq 'go_rightward' ) {
        $self->update( 'turn_r_90' );
        $self->update( 'go_forward' );
    }
    elsif ( $action eq 'turn_l_90' ) {
        $self->angle($self->angle + 1);
    }
    elsif ( $action eq 'turn_r_90' ) {
        $self->angle( $self->angle - 1 );
    }
    elsif ( $action eq 'turn' ) {
        $self->update( 'turn_l_90' );
        $self->update( 'turn_l_90' );
    }
    elsif ( $action eq 'idle' ) {
        $self->finished(1);
        return;
    }
    $self->route->[ $self->i ][ $self->j ] = $self->direction;
}

# マイクロマウス現在経路情報をターミナルにプリント出力
sub print {
    my $self = shift;
    my ( $i, $j ) = ( $self->i, $self->j );
    my $direction = $self->direction;
    my ( $maze, $route ) = ( $self->maze, $self->route );
    print " ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ \n";
    for( my $i = 0; $i < 16; $i++ ) {
        print '|';
        for( my $j = 0; $j < 16; $j++ ) {
            if ( !defined $route->[$i][$j] ) {
                print '  ';
            }
            elsif( $route->[$i][$j] eq 'up' ) {
                print ' ↑';
            }
            elsif( $route->[$i][$j] eq 'down' ) {
                print ' ↓';
            }
            elsif( $route->[$i][$j] eq 'left' ) {
                print ' ←';
            }
            elsif( $route->[$i][$j] eq 'right' ) {
                print ' →';
            }
            if ( $maze->[$i][$j]->{right} ) {
                print ' |';
            }
            else {
                print '  ';
            }
        }
        print "\n",'|';
        for( my $j = 0; $j < 16; $j++ ) {
            if ( $maze->[$i][$j]->{down} ) {
                print '___';
            }
            else {
                print '   ';
            }
            if ( $maze->[$i][$j]->{right} ) {
                print '|';
            }
            else {
                print ' ';
            }
        }
        print "\n";
    }
}

1;

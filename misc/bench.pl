use 5.020;

use CSGame::Dict;

use Benchmark qw(:all);
my $tmp;
my $start = time;
my $dict = CSGame::Dict->new( 'misc/test.json' );

timethese(1000000,
          {
              'get'          => sub{$dict->get('heroes', 'Pirate_Pirate', 'tier') },
              'get_value'    => sub{$dict->get_value('heroes', 'Pirate_Pirate', 'tier') },
              'get_w_export' => sub{$dict->get('heroes', 'Pirate_Pirate', 'tier')->export() },
          });

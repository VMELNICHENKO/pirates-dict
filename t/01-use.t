use 5.020;
use Test::More;
use Test::Deep;

BEGIN {
   use_ok('Pirates::Dict');
};

my $dict_obj = Pirates::Dict->new('t/var/dict/heroes.json');
isa_ok $dict_obj, 'Pirates::Dict';

$dict_obj->dump();

#cmp_deeply $dict_obj->export, {};

my $child = $dict_obj->get(['testhash', 'obj']);
#my $val   = $dict_obj->get_child_val('ident');

note explain $child;
$child->dump();
$child = $dict_obj->get(['testhash', 'bad_key']);

ok not defined $child;

ok 1;

done_testing();

use 5.020;
use Test::More;
use Test::Deep;

BEGIN {
   use_ok('CSGame::Dict');
};

my $dict_obj = CSGame::Dict->new('t/var/dict/heroes.json');
isa_ok $dict_obj, 'CSGame::Dict';

$dict_obj->dump();

my $child = $dict_obj->get(['testhash', 'obj']);

$child->dump();
$child = $dict_obj->get(['testhash', 'bad_key']);

ok not defined $child;

my $str_child = $dict_obj->get(['ident']);
is $str_child->export(), "test";

my $int_child = $dict_obj->get(['testint']);
is $int_child->export(), 42;

my $float_child = $dict_obj->get(['testfloat']);
is $float_child->export(), 42.42;

my $bool_child = $dict_obj->get(['testbool-1']);
is $bool_child->export(), 0;
$bool_child = $dict_obj->get(['testbool-2']);
is $bool_child->export(), 1;


my $hash = $dict_obj->get(['testhash']);
cmp_deeply $hash->export(),
    {
		arr  => [1, 0],
		obj  => { qqq => 111 },
		test => 1
	};

my $array = $dict_obj->get(['testarr']);
cmp_deeply $array->export(),
    [
        '1',
        '2',
        {
            'asd' => '42',
            'zxc' => '13'
        },
        [1, 2, 3]
    ];

done_testing();

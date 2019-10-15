use 5.020;
use Test::More;
use Test::Deep;

BEGIN {
   use_ok('CSGame::Dict');
};

my $dict_obj = CSGame::Dict->new('t/var/dict/heroes.json');
#subtest init {
    isa_ok $dict_obj, 'CSGame::Dict';

    ok defined $dict_obj->get(['testhash', 'obj']);

    ok not defined $dict_obj->get(['testhash', 'bad_key']);
    ok defined $dict_obj->get(['testhash', 'arr']);
    ok defined $dict_obj->get(['testarr' , 1]);
ok defined $dict_obj->get(['testarr' , 2, 'asd']);
$dict_obj->dump();
note "==============================\n";

#note explain $dict_obj->get(['testarr' , 2, 'asd', 'qqq'])->export();
note "\n\n\n";
    ok not defined $dict_obj->get(['testarr' , 2, 'asd', 'qqq']);
#};

#subtest export {
is $dict_obj->get(['testarr' , 2, 'asd'])->export(), 42;

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
            obj  => { qqq => 111, "undef" => undef },
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
#};

done_testing();

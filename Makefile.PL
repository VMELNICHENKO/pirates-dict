use 5.022;
use Config;
use XS::Install;

write_makefile(
    NAME     => 'CSGame::Dict',
    AUTHOR   => 'Vladimir Melnichenko',
    BIN_DEPS => ['Export::XS', 'XS::Framework', 'CPP::panda::lib'],
    CPLUS    => 17,
    SRC      => ['src', 'external'],
    INC      => '-Isrc -Iexternal',
    CCFLAGS  => '-Wall -Wextra',
);

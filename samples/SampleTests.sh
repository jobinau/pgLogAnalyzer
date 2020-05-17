./pgloganalyzer -f samples/postgresql-2020-05-11_093620.log -p "%m [%p] %q%u@%d"
./pgloganalyzer -f samples/postgresql-2020-05-11_102203.log -p "%m [%p] "
./pgloganalyzer -f samples/postgresql-2020-05-17_164935.log -p "%t [%p]: [%l-1] user=%u,db=%d,app=%a,client=%h "

if ! test -f pvstat10000 
then
    gzip -d pvstat10000.gz
fi
time ../bin/stat_wlog -p beacon_pv2.conf  -f "pvstat10000" -l log.config  -o logana_bench

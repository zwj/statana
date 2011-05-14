#!/usr/bin/awk -f

BEGIN {
    FS=OFS=","
    num = 7
    outdir = "split/"
    if (ARGC > 4)
        usage()

    i = 1
    if (ARGV[i] ~ /^-[0-9]+$/) {
        num = -ARGV[i]
        ARGV[i] = ""
        i++
    }
    # test argv in case reading from stdin instead of file
    if (i in ARGV)
        i++    # skip data file name

    outdir = ARGV[ARGC-1]
    ARGV[ARGC-1] = ""
}

{
    i=2
    key = $2
    while(i < NF)
    {
        i++
        key = (key "," $i)
    }
    j = int(key % num)
    if(j < 0)
        j = -j
    out = (outdir j)
    print $0 > out
}

function usage(e)
{
    e = "usage: split [-num] [file] [outdir]"
    print e > "/dev/stderr"
    exit 1
}

#!/usr/bin/awk -f

BEGIN {
    FS=OFS=","
    outfile = ".stat"
}

{
    if(fn != FILENAME && fn != "")
    {
        for(key in count)
            print count[key],key > out
        delete count
    }
    i=2
    key = $2
    while(i < NF)
    {
        i++
        key = (key "," $i)
    }
    count[key] += $1
    fn = FILENAME
    out = (fn outfile)
}

END {
    for(key in count)
        print count[key],key > out
}

function usage(e)
{
    e = "usage: split file1 file2 ... outdir"
    print e > "/dev/stderr"
    exit 1
}

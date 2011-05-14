#!/bin/sh

BINDIR=/home/logsget/bin
TMPDIR=split
split_files=

merge () {
    mkdir -p $TMPDIR
    $BINDIR/split.awk $1 $TMPDIR/
    for file in `ls $TMPDIR`
    do
        split_files="$split_files $TMPDIR/$file"
    done
    $BINDIR/batch_merge.awk $split_files
    for file in `ls $TMPDIR/*.stat`
    do
        cat $file >> $2
    done
    rm -rf $TMPDIR
}

files=`echo $1 | awk 'BEGIN{FS=","; i=0} {while(i < NF) {i++; file=file " " $i} print file}'`

merge "$files" $2

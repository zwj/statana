#bjam "subsystems/split_merger/build" release
#mv $LOG_HOME/bin/result_splitter $LOG_HOME/bin/result_splitter-1.5
#mv $LOG_HOME/bin/result_merger $LOG_HOME/bin/result_merger-1.5
#bjam release
#mv $LOG_HOME/bin/stat_wlog $LOG_HOME/bin/stat_wlog_1.2.2
clear 
bjam > .prj/out.txt;
cat .prj/out.txt;
#bjam release > bjam.res 2>&1
#cat bjam.res

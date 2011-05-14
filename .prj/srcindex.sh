root=$LOG_HOME
cd $root
rm -r -f cscope.*
find $root/ -name "*.h" > .prj/cscope.files
find $root/ -name "*.hpp" >> .prj/cscope.files
find $root/ -name "*.c" >> .prj/cscope.files
find $root/ -name "*.cpp" >> .prj/cscope.files
#find /usr/local/include/stlport/ -name "*" >> .prj/cscope.files
#find /usr/local/include/stlport/ -name "*" >> .prj/cscope.files
#find /usr/local/include/boost/ -name "*.h*" >> .prj/cscope.files
cd .prj
cscope -b 
/usr/local/bin/exctags -R ../ #/usr/local/include/stlport/ /usr/local/include/boost/

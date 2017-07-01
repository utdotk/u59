# qmake.sh
# 2016/11/22
# qmakeによって.proファイルを作成し、
# llvmのコンパイルオプションを追加してmakeファイルを作成する

name=$(basename `pwd`)
qmake -project
echo >> $name.pro
echo QMAKE_CXXFLAGS += \`llvm-config --cxxflags\` >> $name.pro
echo LIBS += \`llvm-config --ldflags --libs core\` \
     -lpthread -ldl -lncurses -std=c++11 >> $name.pro
qmake

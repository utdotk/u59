# format.sh
# 2016/11/22
# include/, src/内のファイル全てをフォーマットし,
# .formatted/include, .formatted/src内に出力する.

for f in include/* src/*
do
    clang-format-3.7 -style=llvm $f > .formatted/$f
done

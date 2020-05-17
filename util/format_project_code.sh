#! /bin/sh

find src/ -name '*.cpp' -exec clang-format -i {} \;
find src/ -name '*.hpp' -exec clang-format -i {} \;
find src/ -name '*.h' -exec clang-format -i {} \;
find testsrc/ -name '*.cpp' -exec clang-format -i {} \;
find testsrc/ -name '*.hpp' -exec clang-format -i {} \;
find testsrc/ -name '*.h' -exec clang-format -i {} \;

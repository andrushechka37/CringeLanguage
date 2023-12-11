CXXFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
   -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts                             \
   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal           \
   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline                   \
   -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked                     \
   -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           \
   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                         \
   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default               \
   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast                    \
   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing            \
   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation             \
   -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192                  \
   -Wstack-usage=8192 -fsanitize=address -fsanitize=undefined -fPIE -Werror=vla


.PHONY: tree
akinator: tree.o deff_dump.o diff.o recursive_down.o frontend.o
	clang++ tree.o deff_dump.o diff.o recursive_down.o frontend.o $(CXXFLAGS) -o deff && ./deff
deff_dump.o: diff_project/deff_dump.cpp diff_project/deff_dump.h diff_project/tree.h
	clang++ -c diff_project/deff_dump.cpp

tree.o: diff_project/tree.cpp diff_project/tree.h
	clang++ -c diff_project/tree.cpp

diff.o: diff_project/diff.cpp diff_project/diff.h diff_project/tree.h
	clang++ -c diff_project/diff.cpp
recursive_down.o: recursive_down.cpp recursive_down.h
	clang++ -c recursive_down.cpp
frontend.o: frontend.cpp recursive_down.h diff_project/diff.cpp diff_project/diff.h diff_project/tree.h
	clang++ -c frontend.cpp
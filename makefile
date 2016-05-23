c2smt: src/ast.cpp src/parser.cpp src/preprocessor.cpp src/solver_runner.cpp src/z3_runner.cpp src/options_parser.cpp src/c2smt.cpp
	g++ -std=c++11 -o c2smt src/ast.cpp src/parser.cpp src/preprocessor.cpp src/solver_runner.cpp src/z3_runner.cpp src/options_parser.cpp src/c2smt.cpp

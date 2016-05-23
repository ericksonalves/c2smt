# c2smt

c2smt is a tool to translate C code into SMT script.

You can build c2smt by typing make in the root directory (g++ required).

Usage:
* If you want to simply verify a file, use c2smt file.c --with-z3=path/to/z3/bin
* If you want to delete the smt2 file created for verification, you can append the option --delete-file to the previous command.

Tool developed during the Systems and Software Verification course, instructed by Prof. Ph.D. Lucas Cordeiro at the Federal University of Amazonas, UFAM, Brazil.

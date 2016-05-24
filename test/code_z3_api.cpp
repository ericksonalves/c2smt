#include <iostream>
#include <z3++.h>

void q2()
{
    z3::context c;
    z3::solver s(c);

    z3::expr n0 = c.int_const("n0");
    s.add(n0 == 4);
    s.add(n0 > 0);
    z3::expr c0 = c.real_const("c0");
    s.add(c0 == 10 / n0);

    z3::expr n1 = c.int_const("n1");
    s.add(n1 == n0 - 1);
    s.add(n1 > 0);
    z3::expr c1 = c.real_const("c1");
    s.add(c1 == 10 / n1);

    z3::expr n2 = c.int_const("n2");
    s.add(n2 == n1 - 1);
    s.add(n2 > 0);
    z3::expr c2 = c.real_const("c2");
    s.add(c2 == 10 / n2);

    z3::expr n3 = c.int_const("n3");
    s.add(n3 == n2 - 1);
    s.add(n3 > 0);
    z3::expr c3 = c.real_const("c3");
    s.add(c3 == 10 / n3);

    /*
    * Uncomment this code to generate an unsat example
    *
    *
    * z3::expr n4 = c.int_const("n4");
    * s.add(n4 == n3 - 1);
    * s.add(n4 > 0);
    */

    switch (s.check()) {
        case z3::unsat:   std::cout << "given smt-lib code is unsat" << std::endl; break;
        case z3::sat:     std::cout << "given smt-lib code is sat" << std::endl; break;
        case z3::unknown: std::cout << "given smt-lib code is unknown" << std::endl; break;
    }
}

int main() {
    try
    {
        q2();
    }
    catch (z3::exception & ex)
    {
        std::cout << "unexpected error: " << ex << std::endl;
    }
    return 0;
}

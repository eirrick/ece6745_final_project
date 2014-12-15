
#define CATCH_CONFIG_MAIN

#include<catch.hpp>
#include<iostream>
#include"polynomial.h"

using std::cout;
using std::endl;
using std::get;

TEST_CASE("Test Monomial")
{
    Monomial m("32.56x2y3z4");
    REQUIRE(m.get_coefficient()>32.55);
    REQUIRE(m.get_coefficient()<32.57);
    REQUIRE(m.get_degree()==9);
    REQUIRE(m.vars[0].var=='z');
    REQUIRE(m.vars[0].exponent==4);
    REQUIRE(m.vars[1].var=='y');
    REQUIRE(m.vars[1].exponent==3);
    REQUIRE(m.vars[2].var=='x');
    REQUIRE(m.vars[2].exponent==2);
    Monomial neg_m = -m;
    REQUIRE(double_compare(neg_m.get_coefficient(),-32.56)==true);
}

TEST_CASE("Test Monomial 2")
{
    Monomial m("abcwxyz");
    REQUIRE(m.get_coefficient()>0.999);
    REQUIRE(m.get_coefficient()<1.001);
    REQUIRE(m.get_degree()==7);
    REQUIRE(m.vars[0].var=='z');
    REQUIRE(m.vars[0].exponent==1);
    REQUIRE(m.vars[1].var=='y');
    REQUIRE(m.vars[1].exponent==1);
    REQUIRE(m.vars[2].var=='x');
    REQUIRE(m.vars[2].exponent==1);
    REQUIRE(m.vars[3].var=='w');
    REQUIRE(m.vars[3].exponent==1);
    REQUIRE(m.vars[4].var=='c');
    REQUIRE(m.vars[4].exponent==1);
    REQUIRE(m.vars[5].var=='b');
    REQUIRE(m.vars[5].exponent==1);
    REQUIRE(m.vars[6].var=='a');
    REQUIRE(m.vars[6].exponent==1);
    Monomial neg_m = -m;
    REQUIRE(double_compare(neg_m.get_coefficient(),-1)==true);
}

TEST_CASE("Test Monomial 3")
{
    Monomial m("6t78937b");
    REQUIRE(m.get_coefficient()>5.99);
    REQUIRE(m.get_coefficient()<6.01);
    REQUIRE(m.get_degree()==78938);
    REQUIRE(m.vars[0].var=='t');
    REQUIRE(m.vars[0].exponent==78937);
    REQUIRE(m.vars[1].var=='b');
    REQUIRE(m.vars[1].exponent==1);
    Monomial neg_m = -m;
    REQUIRE(double_compare(neg_m.get_coefficient(),-6)==true);
}

TEST_CASE("Test Monomial just number")
{
    Monomial m("33.8");
    REQUIRE(m.get_coefficient()<33.801);
    REQUIRE(m.get_coefficient()>33.799);
    REQUIRE(m.get_degree()==0);
    REQUIRE(m.vars.empty()==true);
    Monomial neg_m = -m;
    REQUIRE(double_compare(neg_m.get_coefficient(),-33.8)==true);
}

TEST_CASE("Test Polynomial 1")
{
    Polynomial p("2x2yz+3xy3-2x3");
    REQUIRE(p.get_leading_term()==Monomial("3xy3"));
    REQUIRE(double_compare(p.get_leading_coefficient(),3)==true);
    REQUIRE(p.get_leading_monom()==Monomial("xy3"));
}

TEST_CASE("Test Polynomial 2")
{
    Polynomial p("3v5e2a8-2x16");
    REQUIRE(p.get_leading_term()==Monomial("-2x16"));
    REQUIRE(double_compare(p.get_leading_coefficient(),-2)==true);
    REQUIRE(p.get_leading_monom()==Monomial("x16"));
}

TEST_CASE("Test simplify()")
{
    Polynomial p("3x+x");
    REQUIRE(p.get_leading_term()==Monomial("4x"));
}

TEST_CASE("Test polynomial subtraction")
{
    Polynomial p("3x+4y-z");
    Polynomial p2("-x+3z");
    Polynomial result = p-p2;
    REQUIRE(result==Polynomial("4x+4y-4z"));
    Polynomial result2 = p2-p;
    REQUIRE(result2==Polynomial("-4x-4y+4z"));
}

TEST_CASE("Test simplify with just numbers")
{
    Polynomial p("3.2+8.678-2.125-3");
    REQUIRE(p==Polynomial("6.753"));
}

TEST_CASE("Test monomial division")
{
    Monomial dividend("3xy2z3");
    Monomial divisor("3xy2z3");
    Monomial quotient = dividend/divisor;
    REQUIRE(quotient==Monomial("1"));
    dividend = Monomial("x2");
    divisor = Monomial("x");
    quotient = dividend/divisor;
    REQUIRE(quotient==Monomial("x"));
    dividend = Monomial("x2");
    divisor = Monomial("x3");
    quotient = dividend/divisor;
    REQUIRE(quotient==Monomial());
    dividend = Monomial("4x3y2z");
    divisor = Monomial("2xyz");
    quotient = dividend/divisor;
    REQUIRE(quotient==Monomial("2x2y"));
    divisor = Monomial("yxa");
    dividend = Monomial("z");
    REQUIRE((dividend/divisor)==Monomial());
    divisor = Monomial("y");
    dividend = Monomial("z");
    REQUIRE((dividend/divisor)==Monomial());
    divisor = Monomial("x2y");
    dividend = Monomial("-x2z");
    REQUIRE((dividend/divisor)==Monomial());
}

TEST_CASE("Test monomial multiplied by floating point number")
{
    Monomial m("22x2y3");
    Monomial result = m*2.2;
    REQUIRE(result==Monomial("48.4x2y3"));
}

TEST_CASE("Test Monomial negation")
{
    Monomial m("-3.14x");
    Monomial result = -m;
    REQUIRE(result==Monomial("3.14x"));
}

TEST_CASE("Test has_same_vars")
{
    Monomial m1("31x2y5a9");
    Monomial m2("-22x2y5a9");
    Monomial m3("31xy5a9");
    REQUIRE(has_same_vars(m1,m2)==true);
    REQUIRE(has_same_vars(m1,m3)==false);
}

TEST_CASE("Test Polynomial addition")
{
    Polynomial p1("3x+4z2y-6ab2");
    Polynomial p2("2x-4z+m-4zy+2ab2");
    REQUIRE((p1+p2)==Polynomial("5x+4z2y-4z+m-4zy-4ab2"));
}

TEST_CASE("Test Polynomial/Monomial addition/multiplication")
{
    Polynomial p("3x2-y2+xyz");
    Monomial m("3b");
    Monomial m2("2y2");
    REQUIRE((p+m)==Polynomial("3x2-y2+xyz+3b"));
    REQUIRE((p+m2)==Polynomial("3x2+y2+xyz"));
    REQUIRE((m*p)==Polynomial("9bx2-3by2+3bxyz"));
    Polynomial p2 = m2*p;
    bool b = p2==Polynomial("6x2y2-2y4+2xy3z");
    REQUIRE(p2==Polynomial("6x2y2-2y4+2xy3z"));
}

TEST_CASE("Multivariate division")
{
    Polynomial f("x3-x2y-x2z+x");
    Polynomial f1("x2y-z");
    Polynomial f2("xy-1");
    vector<Polynomial> v({f1,f2});
    auto result = f/v;
    REQUIRE(get<0>(result)==Polynomial("x3-x2z+x-z"));
    REQUIRE(get<1>(result)[0]==Polynomial("-1"));
}

TEST_CASE("Test lcm")
{
    Monomial m1("31.45x2yz");
    Monomial m2("22a2b2c2xy");
    REQUIRE(lcm(m1,m2)==Monomial("a2b2c2x2yz"));
    m1 = Monomial("3a2");
    m2 = Monomial("z54");
    REQUIRE(lcm(m1,m2)==Monomial("a2z54"));
    m1 = Monomial("m");
    m2 = Monomial("m");
    REQUIRE(lcm(m1,m2)==Monomial("m"));
    m1 = Monomial("m3");
    m2 = Monomial("m2");
    REQUIRE(lcm(m1,m2)==Monomial("m3"));
}



#define CATCH_CONFIG_MAIN

#include<catch.hpp>
#include<iostream>
#include"polynomial.h"

using std::cout;
using std::endl;

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
}

TEST_CASE("Test Polynomial 2")
{
    Polynomial p("3v5e2a8-2x16");
    REQUIRE(p.get_leading_term()==Monomial("-2x16"));
    REQUIRE(double_compare(p.get_leading_coefficient(),-2)==true);
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


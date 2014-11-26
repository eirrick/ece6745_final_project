
/* Class: ECE 6745 Final Project
 * Author: Eric Hair
 * Date: Nov 2014
 */

#include<regex>
#include<iterator>
#include<iostream>
#include<locale>
#include"polynomial.h"

using std::regex;
using std::smatch;
using std::locale;
using std::regex_constants::icase;
using std::cerr;
using std::endl;
using std::sregex_iterator;

// Monomial constructor takes a string representing the monomial
Monomial::Monomial(string monom) :
    coefficient(1)
{
    string coeff_pattern = "-?\\d+\\.?\\d*";
    string var_exp_pattern = "([a-z])(\\d*)";
    regex r_coeff(coeff_pattern);
    regex r_var_exp(var_exp_pattern);
    smatch m;
    string leftover = monom;

    // Strip the coefficient off and put it in member variable
    if (regex_search(monom,m,r_coeff)) {
        coefficient = stod(m[1]);
        leftover = m.suffix();
    }
    sregex_iterator it(leftover.begin(),leftover.end(),r_var_exp);
    sregex_iterator it_end;

    // Iterate over variable/exponents and add them to the vars vector
    while (it!=it_end) {
        string letter = (*it)[2];
        var_exp current;
        current.var = letter[0];
        if (it->size()==2)
            current.exponent = 1;
        else if (it->size()==3)
            current.exponent = stoi((*it)[3]);
        else {
            cerr<<"Error: The monomial was not in the corrent format."<<endl;
            exit(2);
        }
        vars.push_back(current);
        ++it;
    }
    sort(vars.begin(),vars.end(),comp_var_exp);
}

// Polynomial constructor takes a string representing the entire polynomial
Polynomial::Polynomial(string s) :
    num_terms(0)
{
    // Make the letters all lower case by default
    locale loc; // Needed by tolower()
    string poly_pattern = "([+-]?(\\d+\\.?\\d*|[a-z])([a-z]?\\d*)*)+";
    for (unsigned long i=0;i<s.length();++i) {
        s[i] = tolower(s[i],loc);
    }
    regex r_poly(poly_pattern,icase);

    // Verify it is a valid polynomial
    if (regex_match(s,r_poly)!=true) {
        cerr<<"Error: The entry is not a valid polynomial."<<endl;
        exit(1);
    }
    string monom_pattern = "-?(\\d+\\.?\\d*|[a-z])([a-z]?\\d*)*";
    regex r_monom(monom_pattern,icase);
    sregex_iterator it(s.begin(),s.end(),r_monom);
    sregex_iterator it_end;

    // Iterate over individual terms, create Monomial objects, and put them in
    // the terms vector
    while (it!=it_end) {
        ++num_terms;
        terms.push_back(Monomial(it->str()));
        ++it;
    }

    // Ensure DegRevLex ordering
    order_terms();
}

// Operator < used on Monomials so that they can be sorted by the sort()
bool operator<(const Monomial& lhs, const Monomial& rhs)
{
    if (lhs.get_degree()<rhs.get_degree())
        return false;
    else if (lhs.get_degree()>rhs.get_degree())
        return true;
    // If degree is equal: look through the variables and use RevLex as a tie
    // breaker
    else {
        for (unsigned long i=0;i<lhs.vars.size()&&i<rhs.vars.size();++i) {
            if (lhs.vars[i].var<rhs.vars[i].var)
                return false;
            else if (lhs.vars[i].var>rhs.vars[i].var)
                return true;
            else
                continue;
        }
    }
    return false;
}


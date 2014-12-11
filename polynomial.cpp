
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
using std::cerr;
using std::endl;
using std::sregex_iterator;
using std::cout;
using std::make_tuple;

// Monomial constructor takes a string representing the monomial
Monomial::Monomial(string monom) :
    coefficient(1)
{
    string sign = "-";
    string coeff_pattern = "^-?\\d+\\.?\\d*";
    string var_exp_pattern = "([a-z])(\\d*)";
    regex r_coeff(coeff_pattern);
    regex r_var_exp(var_exp_pattern);
    regex r_sign(sign);
    smatch m;
    string leftover = monom;

    // Strip the coefficient off and put it in member variable
    if (regex_search(monom,m,r_coeff)) {
        coefficient = stod(m[0]);
        leftover = m.suffix();
    }
    
    // Check to see if there is a negative sign and coefficient is 1 then
    // coefficient should be -1
    if (double_compare(coefficient,1)&&regex_search(monom,m,r_sign))
        coefficient = -1;

    sregex_iterator it(leftover.begin(),leftover.end(),r_var_exp);
    sregex_iterator it_end;

    // Iterate over variable/exponents and add them to the vars vector
    while (it!=it_end) {
        string letter = (*it)[1];
        var_exp current;
        current.var = letter[0];
        if ((*it)[2]=="")
            current.exponent = 1;
        else {
            current.exponent = stoi((*it)[2]);
        }
        vars.push_back(current);
        ++it;
    }
    sort(vars.begin(),vars.end(),comp_var_exp);
    return;
}

// Polynomial constructor takes a string representing the entire polynomial
Polynomial::Polynomial(string s)
{
    // Make the letters all lower case by default
    locale loc; // Needed by tolower()
    string poly_pattern = "([+-]?(\\d+\\.?\\d*|[a-z])([a-z]?\\d*)*)+";
    for (unsigned long i=0;i<s.length();++i) {
        s[i] = tolower(s[i],loc);
    }
    regex r_poly(poly_pattern);

    // Verify it is a valid polynomial
    if (regex_match(s,r_poly)!=true) {
        cerr<<"Error: The entry is not a valid polynomial."<<endl;
        exit(1);
    }
    string monom_pattern = "-?(\\d+\\.?\\d*|[a-z])([a-z]?\\d*)*";
    regex r_monom(monom_pattern);
    sregex_iterator it(s.begin(),s.end(),r_monom);
    sregex_iterator it_end;

    // Iterate over individual terms, create Monomial objects, and put them in
    // the terms vector
    while (it!=it_end) {
        Monomial m(it->str());
        if (!double_compare(m.coefficient,0))
            terms.push_back(Monomial(it->str()));
        ++it;
    }

    // Ensure DegRevLex ordering
    simplify();
    return;
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
                return true;
            else if (lhs.vars[i].var>rhs.vars[i].var)
                return false;
            // If var is the same, check exponent
            else {
                if (lhs.vars[i].exponent<rhs.vars[i].exponent)
                    return true;
                else if (lhs.vars[i].exponent>rhs.vars[i].exponent)
                    return false;
                // If exponents are the same then check next var
                else
                    continue;
            }
        }
    }
    return false;
}

// Operator == can be used to check equality of Monomials
bool operator==(const Monomial& lhs, const Monomial& rhs)
{
    if (!double_compare(lhs.get_coefficient(),rhs.get_coefficient()))
        return false;
    else {
        if (lhs.vars.size()!=rhs.vars.size())
            return false;
        else {
            for (unsigned int i=0;i<lhs.vars.size();++i) {
                if (lhs.vars[i]!=rhs.vars[i])
                    return false;
            }
            return true;
        }
    }
}


// A function that will combine combinable terms in the Polynomial
void Polynomial::simplify()
{
    // Sort the terms, then move through them comparing vars, if they're the
    // same, then combine by adding or subtracting the coefficient.
    order_terms();
    unsigned int i = 0;
    while (i<terms.size()-1) {
        if (has_same_vars(terms[i],terms[i+1])) {
            terms[i].coefficient += terms[i+1].coefficient;
            terms.erase(terms.begin()+i+1);
        }
        else
            ++i;
    }
    return;
}

// Returns true if the Polynomials contain the same terms, false otherwise
bool operator==(const Polynomial& lhs, const Polynomial& rhs)
{
    if (lhs.terms.size()!=rhs.terms.size())
        return false;
    for (unsigned int i=0;i<lhs.terms.size();++i) {
        if (lhs.terms[i]!=rhs.terms[i])
            return false;
    }
    return true;
}

// Polynomial division
tuple<Polynomial,vector<Polynomial>> operator/(const Polynomial& f,
        const vector<Polynomial>& divisor)
{
    vector<Polynomial> u(divisor.size());
    Polynomial r;
    Polynomial h = f;

    while (h.size()!=0) {
        bool found_one = false;
        for (unsigned long i=0;i<divisor.size();++i) {
            Monomial result = h.get_leading_term()
                /divisor[i].get_leading_term();
            if (result!=Monomial()) {
                u[i] = u[i]+result;
                h = h-result*divisor[i];
                found_one = true;
                break;
            }
        }
        if (!found_one) {
            r = r+h.get_leading_term();
            h = h-h.get_leading_term();
        }
    }
    tuple<Polynomial,vector<Polynomial>> result = make_tuple(r,u);
    return result;
}

// Two Monomials need to be divided to perform the multivariate division
// algorithm, returns an empty Monomial if the division can't be performed
Monomial operator/(const Monomial& dividend, const Monomial& divisor)
{
    if (divisor.vars.size()>dividend.vars.size())
        return Monomial();
    Monomial result;
    unsigned long j = 0;
    for (unsigned long i=0;i<divisor.vars.size();++i) {
        if (divisor.vars[i].var==dividend.vars[j].var) {
            if (dividend.vars[j].exponent==divisor.vars[i].exponent) {
                ++j;
            }
            else if (dividend.vars[j].exponent>divisor.vars[i].exponent) {
                var_exp quotient;
                quotient.var = dividend.vars[j].var;
                quotient.exponent = dividend.vars[j].exponent
                    -divisor.vars[i].exponent;
                result.vars.push_back(quotient);
                ++j;
            }
            else
                return Monomial();
        }
        else if (divisor.vars[i].var>dividend.vars[j].var)
            return Monomial();
        else {
            do {
                var_exp quotient = dividend.vars[j];
                result.vars.push_back(quotient);
                ++j;
                if (j==dividend.vars.size())
                    return Monomial();
            } while (divisor.vars[i].var<dividend.vars[j].var);
        }
    }
    result.coefficient = dividend.get_coefficient()/divisor.get_coefficient();
    return result;
}

// Multiplies a Monomial by a Polynomial
Polynomial operator*(const Monomial& lhs, const Polynomial& rhs)
{
    Polynomial result = rhs;
    for (unsigned long i=0;i<rhs.terms.size();++i) {
        unsigned long k = 0;
        for (unsigned long j=0;j<lhs.vars.size();) {
            if (rhs.terms[i].vars[k].var==lhs.vars[j].var) {
                result.terms[i].vars[k].exponent += lhs.vars[j].exponent;
                ++k;
                ++j;
            }
            else if (rhs.terms[i].vars[k].var<lhs.vars[j].var) {
                var_exp ve = lhs.vars[j];
                vector<var_exp>::iterator it = result.terms[i].vars.begin();
                result.terms[i].vars.insert(it+static_cast<int>(k),ve);
                ++j;
            }
            else {
                do {
                    ++k;
                    if (k==result.terms[i].vars.size()) {
                        var_exp ve = lhs.vars[j];
                        result.terms[i].vars.push_back(ve);
                        ++j;
                        break;
                    }
                } while (result.terms[i].vars[k].var>lhs.vars[j].var);
            }
        }
        result.terms[i] = result.terms[i]*lhs.get_coefficient();
    }
    return result;
}


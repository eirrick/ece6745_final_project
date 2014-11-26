
/* Class: ECE 6745 Final Project
 * Author: Eric Hair
 * Date: Nov 2014
 */

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include<string>
#include<vector>
#include<algorithm>

using std::vector;
using std::string;

// This struct is used just to keep the variable and the exponent of that
// variable together, so they can't get mixed up when placing the multiple
// variables in rev lex order. This makes ordering an entire polynomial much
// easier, because you only have to check the first variables until one is
// found that doesn't exist in both monomials.
struct var_exp {
    char var;
    int exponent;
};

// This function is provided to the sort() from the c++ stdlib. Sort can then
// quickly and efficiently order the variable and exponent pairs of a monomial.
bool comp_var_exp(const var_exp& a, const var_exp& b)
{
    return(a.var>b.var);
}

// The Monomial class contains represents indivdual terms that make up a
// polynomial.
class Monomial {
    private:
        double coefficient;

    public:
        vector<var_exp> vars; // Container for variable exponent pairs
        Monomial(string monom);
        friend bool operator<(const Monomial& lhs, const Monomial& rhs);

        // Returns the total degree of the monomial
        int get_degree() const
        {
            int degree = 0;
            for (unsigned long i=0;i<vars.size();++i) {
                degree += vars[i].exponent;
            }
            return degree;
        }

        // Returns the coefficient of the term
        double get_coefficient()
        {
            return coefficient;
        }
};

bool operator<(const Monomial& lhs, const Monomial& rhs);

// The Polynomial class represents a DegRevLex ordered polynomial.
class Polynomial {
    private:
        int num_terms;
        vector<Monomial> terms; // Contains all of the seperate terms in order

        // Sorts the terms into DegRevLex order by comparing monomials via the
        // overloaded "<" operator.
        void order_terms()
        {
            sort(terms.begin(),terms.end());
        }

    public:
        Polynomial(string s);

        // Returns the leading term
        Monomial get_leading_term()
        {
            return terms[0];
        }

        // Returns only the leading coefficient
        double get_leading_coefficient()
        {
            return get_leading_term().get_coefficient();
        }
};

#endif



/* Class: ECE 6745 Final Project
 * Author: Eric Hair
 * Date: Nov 2014
 */

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include<string>
#include<vector>
#include<algorithm> // sort()
#include<cmath> // abs()
#include<tuple>

using std::vector;
using std::string;
using std::abs;
using std::tuple;

// This struct is used just to keep the variable and the exponent of that
// variable together, so they can't get mixed up when placing the multiple
// variables in rev lex order. This makes ordering an entire polynomial much
// easier, because you only have to check the first variables until one is
// found that doesn't exist in both monomials.
struct var_exp {
    char var;
    int exponent;
};

// Allows comparing var_exp types using == operator
inline bool operator==(const var_exp& a, const var_exp& b)
{
    if (a.var==b.var && a.exponent==b.exponent)
        return true;
    return false;
}

// Allows comparing var_exp types using != operator
inline bool operator!=(const var_exp& a, const var_exp& b)
{
    return !(a==b);
}

// This function is provided to the sort() from the c++ stdlib. Sort can then
// quickly and efficiently order the variable and exponent pairs of a monomial.
inline bool comp_var_exp(const var_exp& a, const var_exp& b)
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

        Monomial() :
            coefficient(0)
        {
            vars = vector<var_exp>();
        }

        Monomial(string monom);
        Monomial(const Monomial& other) : // Copy constructor
            coefficient(other.coefficient),
            vars(other.vars)
        {   }
        friend bool operator==(const Monomial& lhs, const Monomial& rhs);

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
        double get_coefficient() const
        {
            return coefficient;
        }

        friend class Polynomial;
        friend bool has_same_vars(const Monomial& lhs, const Monomial& rhs);
        friend Monomial operator/(const Monomial& dividend,
                const Monomial& divisor);
        friend Monomial operator*(const Monomial& lhs, double coefficient);

        Monomial operator-() const
        {
            Monomial result = *this;
            result.coefficient = -coefficient;
            return result;
        }
};

inline bool has_same_vars(const Monomial& lhs, const Monomial& rhs)
{
    Monomial templ = lhs;
    Monomial tempr = rhs;
    templ.coefficient = 1;
    tempr.coefficient = 1;
    return templ==tempr;
}

bool operator<(const Monomial& lhs, const Monomial& rhs);
bool operator==(const Monomial& lhs, const Monomial& rhs);
inline Monomial operator*(const Monomial& lhs, double coefficient)
{
    Monomial result = lhs;
    result.coefficient *= coefficient;
    return result;
}

// Allows comparing Monomial objects using != operator
inline bool operator!=(const Monomial& lhs, const Monomial& rhs)
{
    return !(lhs==rhs);
}

// The Polynomial class represents a DegRevLex ordered polynomial.
class Polynomial {
    private:
        vector<Monomial> terms; // Contains all of the seperate terms in order

        // Sorts the terms into DegRevLex order by comparing monomials via the
        // overloaded "<" operator.
        void order_terms()
        {
            sort(terms.begin(),terms.end());
        }
        void simplify();

    public:
        Polynomial()
        {
            terms = vector<Monomial>();
        }

        Polynomial(string s);

        // Returns the leading term
        Monomial get_leading_term() const
        {
            return terms[0];
        }

        // Returns only the leading coefficient
        double get_leading_coefficient() const
        {
            return get_leading_term().get_coefficient();
        }

        // Returns the leading term with the coefficient set to 1
        Monomial get_leading_monom() const
        {
            Monomial m = get_leading_term();
            Monomial temp = m;
            temp.coefficient = 1;
            return temp;
        }

        unsigned long size() const
        {
            return terms.size();
        }
        
        friend Polynomial operator+(const Polynomial& lhs,
                const Polynomial& rhs);
        friend Polynomial operator-(const Polynomial& lhs,
                const Polynomial& rhs);
        friend bool operator==(const Polynomial& lhs, const Polynomial& rhs);
        friend Polynomial operator-(const Polynomial& lhs, const Monomial& rhs);
        friend Polynomial operator+(const Polynomial& lhs, const Monomial& rhs);
        friend Polynomial operator*(const Monomial& lhs, const Polynomial& rhs);
};

bool operator==(const Polynomial& lhs, const Polynomial& rhs);

inline bool operator!=(const Polynomial& lhs, const Polynomial& rhs)
{
    return !(lhs==rhs);
}

// Making the + operator usable for adding Polynomials
inline Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs)
{
    Polynomial result = lhs;
    for (unsigned long i=0;i<rhs.terms.size();++i)
        result.terms.push_back(rhs.terms[i]);
    result.simplify();
    return result;
}

inline Polynomial operator+(const Polynomial& lhs, const Monomial& rhs)
{
    Polynomial result = lhs;
    result.terms.push_back(rhs);
    result.simplify();
    return result;
}

// Making the - operator usable for subtracting Polynomials
inline Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs)
{
    // Add the new Polynomial as the negative version of all the terms
    Polynomial result = lhs;
    for (unsigned long i=0;i<rhs.terms.size();++i)
        result.terms.push_back(-rhs.terms[i]);
    result.simplify();
    return result;
}

inline Polynomial operator-(const Polynomial& lhs, const Monomial& rhs)
{
    Polynomial result = lhs;
    result.terms.push_back(-rhs);
    result.simplify();
    return result; 
}

tuple<Polynomial,vector<Polynomial>> operator/(const Polynomial& f,
        const vector<Polynomial>& divisor);

Monomial operator/(const Monomial& dividend, const Monomial& divisor);

// A function used for testing. It compares floating point numbers out to 5
// decimal places or so.
inline bool double_compare(const double a, const double b)
{
    const double epsilon = 1e-5;
    return abs(a-b)<=epsilon*abs(a);
}

Polynomial operator*(const Monomial& lhs, const Polynomial& rhs);

#endif


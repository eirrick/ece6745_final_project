
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include<string>
#include<vector>
#include<algorithm>

using std::vector;
using std::string;

struct var_exp {
    char var;
    int exponent;
};

bool comp_var_exp(var_exp a, var_exp b)
{
    return(a.var>b.var);
}

class Monomial {
    private:
        double coefficient;

    public:
        vector<var_exp> vars;
        Monomial(string monom);
        friend bool operator<(Monomial lhs, Monomial rhs);
        int get_degree()
        {
            int degree = 0;
            for (unsigned long i=0;i<vars.size();++i) {
                degree += vars[i].exponent;
            }
            return degree;
        }
        double get_coefficient()
        {
            return coefficient;
        }
};

bool operator<(Monomial lhs, Monomial rhs);

class Polynomial {
    private:
        int num_terms;
        vector<Monomial> terms;
        void order_terms()
        {
            sort(terms.begin(),terms.end());
        }

    public:
        Polynomial(string s);
        Monomial get_leading_term()
        {
            return terms[0];
        }

        double get_leading_coefficient()
        {
            return get_leading_term().get_coefficient();
        }
};

#endif


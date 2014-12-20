
#include"reduction.h"

using std::find;
using std::get;

vector<Polynomial> getMonomialsFromList(vector<Polynomial> F)
{
    vector<Polynomial> monoms;
    for (unsigned long i=0;i<F.size();++i) {
        for (unsigned long j=0;j<F[i].size();++j) {
            if (find(monoms.begin(),monoms.end(),F[i].terms[j])==monoms.end())
                monoms.push_back(Polynomial(F[i].terms[j]));
        }
    }
    return monoms;
}

bool vectors_contain_same_things(vector<Polynomial> v1, vector<Polynomial> v2)
{
    if (v1.size()!=v2.size())
        return false;
    for (unsigned long i=0;i<v1.size();++i) {
        if (find(v2.begin(),v2.end(),v1[i])==v2.end())
            return false;
    }
    return true;
}

vector<Polynomial> exclude(vector<Polynomial> x, vector<Polynomial> y)
{
    vector<Polynomial> result;
    for (unsigned long i=0;i<x.size();++i) {
        bool doesnt_contain = 1;
        for (unsigned long j=0;i<y.size();++j) {
            if (x[i]==y[j])
                doesnt_contain = 0;
                break;
        }
        if (doesnt_contain)
            result.push_back(x[i]);
    }
    return result;
}

Polynomial isTopReducible(Polynomial f, vector<Polynomial> G)
{
    Polynomial r;
    vector<Polynomial> u;
    for (unsigned long i=0;i<G.size();++i) {
        Polynomial p(G[i].get_leading_monom());
        vector<Polynomial> v({p});
        auto result = f/v;
        r = get<0>(result);
        u = get<1>(result);
        if (r==Polynomial())
            return G[i];
    }
    return Polynomial();
}

vector<Polynomial> initSymbolicPreproc(vector<poly_pair> Ld,
        vector<Polynomial> Fd)
{
    Polynomial a;
    Polynomial b;
    Polynomial m;
    vector<Polynomial> F;
    vector<Polynomial> temp({a,b});
    for (unsigned long i=0;i<Ld.size();++i) {
        temp = mySimplify(Ld[i].p1,Ld[i].p2,F,Fd);
        a = temp[0];
        b = temp[1];
        m = a*b;
        F[i] = m;
    }
    return F;
}

vector<vector<Polynomial>> symbolicPreprocessing(vector<poly_pair> Ld,
        vector<Polynomial> G, vector<Polynomial> Fd)
{
    vector<Polynomial> XL;
    vector<Polynomial> Done;
    Polynomial a;
    Polynomial b;
    Polynomial m;
    Polynomial m_prime;
    vector<Polynomial> F = initSymbolicPreproc(Ld,Fd);
    vector<Monomial> temp; 
    for (unsigned int i=0;i<F.size();++i) {
        temp.push_back(F[i].get_leading_monom());
    }
    sort(temp.begin(),temp.end());
    Done[1] = Polynomial(temp.back());
    XL = getMonomialsFromList(F);
    while (!vectors_contain_same_things(XL,Done)) {
        m = exclude(XL,Done)[0];
        if (find(Done.begin(),Done.end(),m)==Done.end())
            Done.push_back(m);
        m_prime = isTopReducible(m,G);
        if (!(m_prime==Polynomial())) {
            m = (m/m_prime.get_leading_monom())*m_prime;
            if (find(F.begin(),F.end(),m)==Done.end()) {
                F.push_back(m);
            }
            vector<Polynomial> badlist({m});
            vector<Polynomial> monoms = getMonomialsFromList(badlist);
            for (unsigned long i=0;i<monoms.size();++i) {
                if (find(XL.begin(),XL.end(),monoms[i])==XL.end())
                    XL.push_back(monoms[i]);
            }
        }
    }
    return vector<vector<Polynomial>>({F,XL});
}

vector<vector<Polynomial>> red(vector<poly_pair> L, vector<Polynomial> G,
        vector<Polynomial> Fd)
{
    Polynomial* M;
    vector<Polynomial> XL;
    vector<Polynomial> v;
    vector<vector<Polynomial>> temp = symbolicPreprocessing(L,G,Fd);
    vector<Polynomial> F = temp[0];
    XL = temp[1];
    XL = mysort(XL);
    M = getMatrix(F,XL);
    rowred(M);
    Fd = getPolyFromRowEchelonForm(M,F,XL);
    v = removeDuplicateLeadMonom(F,Fd);
    return vector<vector<Polynomial>>({v,Fd});
}



#include"polynomial.h"

struct poly_pair {
    Polynomial p1;
    Polynomial p2;
};

vector<vector<Polynomial>> symbolicPreprocessing(vector<poly_pair> L,
        vector<Polynomial> G, vector<Polynomial> Fd);
vector<Polynomial> mysort(vector<Polynomial> XL);
Polynomial* getMatrix(vector<Polynomial> F, vector<Polynomial> XL);
void rowred(Polynomial* M);
vector<Polynomial> getPolyFromRowEchelonForm(Polynomial* M,
        vector<Polynomial> F, vector<Polynomial> XL);
vector<Polynomial> removeDuplicateLeadMonom(vector<Polynomial> F,
        vector<Polynomial> Fd);
vector<Polynomial> initSymbolicPreproc(vector<poly_pair> Ld,
        vector<Polynomial> Fd);


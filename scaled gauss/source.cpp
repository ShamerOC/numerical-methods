// Krzysztof Wydrzynski

#include "vectalg.h"
#include <iostream>

double* createNorms(const Matrix& m) {
    int size = m.size();
    double* norms = new double[size];

    for (int i = 0; i < size; i++) {
        norms[i] = 0;
        for (int j = 0; j < size; j++) {
            double currentValue = std::abs(m(i, j));
            if (norms[i] < currentValue) norms[i] = currentValue;
        }
    }
    return norms;
}

void swapRows(int* permutations, int row1, int row2) {
    if (row1 == row2) return;
    int temp = permutations[row1];
    permutations[row1] = permutations[row2];
    permutations[row2] = temp;
}

int getMaxNormRowFromColumn(Matrix &m, double* norm, int* p, int row) {
    double max = 0;
    int maxRowNumber = 0;
    for (int i = row; i < m.size(); i++) {
        double current = std::abs(m(p[i], row) / norm[p[i]]);
        if (current > max) {
            max = current;
            maxRowNumber = i;
        }
    }

    return maxRowNumber;
}

void subtractRows(Matrix &m, int row1, int row2, int columnToStart , double mult = 1) {
    int n = m.size();
    Matrix::iterator it1 = m.begin();
    Matrix::iterator it2 = it1;
    it1 += n * row1 + columnToStart;
    it2 += n * row2 + columnToStart;
    for (int i = columnToStart; i < m.size(); i++) {
        *it2 = *it2 - (*it1 * mult);
        it1++;
        it2++;
    }
}

void gaussElimination(Matrix &m, Vector &l, int* p, double* norm) {
    int size = m.size();
    for (int i = 0; i < size - 1; i++) {
        int rowToSwap = getMaxNormRowFromColumn(m, norm, p, i);
        swapRows(p, rowToSwap, i);
        for (int j = i + 1; j < size; j++){
            double mult = m(p[j], i) / m(p[i], i);
            subtractRows(m, p[i], p[j], i, mult);
            l[p[j]] -= l[p[i]] * mult;
        }
    }
}
Vector* addVectors(Vector& a, Vector& b) {
    assert(a.size() == b.size());
    int n = a.size();
    Vector* result = new Vector(n);
    Vector::iterator iterator = (*result).begin();
    for (int i = 0; i < n; i++) {
        *iterator = a[i] + b[i];
        iterator++;
    }
    return result;
}

Vector solveEquations(
        const Matrix & A0,
        const Vector & b,
        double  eps
){

    int size = A0.size();

    double* norm = createNorms(A0);

    Matrix* gauss = new Matrix(A0);
    Vector* l = new Vector(b);

    int permutations[size];

    for (int i = 0; i < size; i++) {
        permutations[i] = i;
    }

    gaussElimination(*gauss, *l, permutations, norm);

    Vector* result = new Vector(size);

    Vector::iterator iterator = result->end() - 1;

    double temp;
    for (int i = size - 1; i >= 0; i--) {
        temp = 0;
        for (int j = i + 1; j < size; j++) {
            temp += (*gauss)(permutations[i], j) * (*result)[j];
        }
        *iterator = ((*l)[permutations[i]] - temp) / (*gauss)(permutations[i], i);
        iterator--;
    }

    Vector r = residual_vector(A0, b, *result);

    while (r.max_norm() >= eps) {
        Vector e = solveEquations(A0, r, eps);
        result = addVectors(*result, e);
        r = residual_vector(A0, b, *result);
    }

    return *result;
}
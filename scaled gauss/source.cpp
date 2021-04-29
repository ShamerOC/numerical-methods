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

void print(Matrix &m) {
    int n = m.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << m(i, j) << " ";
        }
        std::cout << '\n';
    }
}

void print(Vector &v) {
    int n = v.size();
    for (int i = 0; i < n; i++) {
        std::cout << v[i] << " ";
    }
    std::cout << '\n';
}

void resetMatrix(Matrix &m) {
    int size = m.size();
    Matrix::iterator it = m.begin();
    for(int i = 0; i < size * size; i++) {
        *it = 0;
        it++;
    }
}


int getMaxNormRowFromRow(Matrix &m, double* norm, int column, int row) {
    int max = 0;
    int maxRowNumber = 0;
    for (int i = row; i < m.size(); i++) {
        double current = std::abs(m(i, column)) / norm[i];
        if (max < current) {
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

void insertOn(Matrix &m, int row, int column, double value) {
    Matrix::iterator i = m.begin();
    i += row * m.size() + column;
    *i = value;
}

void gaussElimination(Matrix &m, Matrix &l, int* p, double* norm) {
    int size = m.size();
    for (int i = 0; i < size - 1; i++) {
        int rowToSwap = getMaxNormRowFromRow(m, norm, i, p[i]);
        swapRows(p, rowToSwap, i);
        insertOn(l, p[i], i, 1);
        for (int j = i + 1; j < size; j++) {
            double mult = m(p[j], i) / m(p[i], i);

            insertOn(l, p[j], i, mult);
            subtractRows(m, p[i], p[j], 0, mult);
            insertOn(m, p[j], i, 0);
        }
    }
    insertOn(l, p[size - 1], size - 1, 1);
}

Vector* permutate(Vector& v, int* p) {
    int size = v.size();
    Vector* result = new Vector(size);
    Vector::iterator iterator = result->begin();
    for (int i = 0; i < size; i++) {
        *iterator = v[p[i]];
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
    Matrix* l = new Matrix(size);
    resetMatrix(*l);

    Vector* correct = new Vector(b);

    int permutations[size];

    for (int i = 0; i < size; i++) {
        permutations[i] = i;
    }

    std::cout << "vector: \n";

    print(*correct);

    std::cout << "Matrix: \n";
    print(*gauss);

    gaussElimination(*gauss, *l, permutations, norm);

    correct = permutate(*correct, permutations);

    std::cout << '\n';

    std::cout << "After gauss: \n";

    print(*gauss);

    std::cout << "After gauss vector: \n";

    print(*correct);

    std::cout << "permutations: \n";
    for (int i = 0; i < size; i++) {
        std::cout << permutations[i] << " ";
    }
    std::cout << "\n";

    std::cout << "l: \n";

    print(*l);

    Vector* result = new Vector(size);
    Vector* b1 = new Vector(size);

    Vector::iterator iterator = result->end() - 1;
    Vector::iterator iterator1 = b1->begin();





    for (int i = 0; i < size; i++) {
        *iterator1 = 1;
        double subtract = 0;
        for (int j = i - 1; j >= 0; j--) {
            subtract += (*l)(permutations[i], j) * (*b1)[j];
        }
//        std::cout << "iteration: " << i << ": subtract: " << subtract;
        double res = (*correct)[i] - subtract;
        *iterator1 = res / (*l)(permutations[i], i);
//        std::cout << " res: " << res << "/" << (*l)(permutations[i], i) << "=" << *iterator1 << '\n';
        iterator1++;
    }

    std::cout << "b: \n";
    for (int i = 0; i < size; i++) {
        std::cout << (*b1)[i] << " ";
    }

    std::cout << "\n";


    for (int i = size - 1; i >= 0; i--) {
        *iterator = 1;
        double subtract = 0;
        for (int j = size - 1; j > i; j--) {
            subtract += (*gauss)(permutations[i], j) * (*result)[j];
//            std::cout << "subtract: " << (*gauss)(permutations[i], j) << "*" << (*result)[j] << "=" << subtract << '\n';
        }
//        std::cout << "iteration: " << i << ": subtract: " << subtract;
        double res = (*b1)[i] - subtract;
        *iterator = res / (*gauss)(permutations[i], i);
//        std::cout << " res: " << res << "/" << (*gauss)(permutations[i], i) << "=" << *iterator << '\n';
        iterator--;
    }

    std::cout << "result: \n";
    for (int i = 0; i < size; i++) {
        std::cout << (*result)[i] << " ";
    }

    std::cout << "\n";



    return *result;
}



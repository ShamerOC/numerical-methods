//Krzysztof Wydrzynski

#include <iostream>
#include "vectalg.h"

long double hermiteCalc(Vector& X, Vector& hermite, double x) {
    long double result = (long double) *(hermite.end() - 1);

    for (int i = hermite.size() - 2; i >= 0; i--) {
        result *= x - X[i];
        result += hermite[i];
    }
    return result;
}

long int fac(int n) {
    long int res = 1;

    for (int i = 2; i <= n; i++) {
        res *= i;
    }

    return res;
}


Vector interpol(Vector& X, Vector& Y, Vector& Yorig, Vector& Yindex) {
    Vector hermite(Y.size());
    hermite[0] = Y[0];

    for (int i = 1; i < Y.size(); i++) {
        for (int j = 0; j < Y.size() - i; j++) {
            if (X[j] == X[j + i]) {
                Y[j] = Yorig[Yindex[j] + i] / fac(i);
            } else {
                Y[j] = (Y[j + 1] - Y[j]) / (X[j + i] - X[j]);
            }
        }
        hermite[i] = Y[0];
    }

    return hermite;
}

int main() {
    int N, M;

    std::cin >> M >> N;

    Vector X(M);
    Vector Y(M);
    Vector T(N);
    Vector Yfirst(M);
    Vector YfirstIndex(M);

    std::cin >> X >> Y >> T;

    Yfirst[0] = Y[0];
    YfirstIndex[0] = 0;

    for (int i = 1; i < M; i++) {
        if (X[i] == X[i - 1]) {
            Yfirst[i] = Yfirst[i - 1];
            YfirstIndex[i] = YfirstIndex[i - 1];
        } else {
            Yfirst[i] = Y[i];
            YfirstIndex[i] = i;
        }
    }


    Vector hermite = interpol(X, Yfirst, Y, YfirstIndex);

    for (int i = 0; i < hermite.size(); i++) {
        double result = hermite[i];
        printf("%17.17f ", result);
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        long double result = hermiteCalc(X, hermite, T[i]);
        printf("%17.17Lf ", result);
    }

    return 0;
}
//Krzysztof Wydrzynski

#include <cmath>
#include <iostream>
#include <stdio.h>

using namespace std;

typedef void (*FuncPointer)(const double* x, double* y, double* Df);

void printVector(const double* x, unsigned N){
    for(unsigned i=0;i<N;++i)
        printf("%17.17f ", x[i]);
    printf("\n");
}

void stripDf3x2(double* Df, double* Dfn) {
    Dfn[0] = Df[0];
    Dfn[1] = Df[1];
    Dfn[2] = Df[3];
    Dfn[3] = Df[4];
}

void stripDf4x2(double* Df, double* Dfn) {
    Dfn[0] = Df[0];
    Dfn[1] = Df[1];
    Dfn[2] = Df[4];
    Dfn[3] = Df[5];
}

int findCurve(FuncPointer f, double* x, unsigned k, double h) {
    double Df[3 * 3];
    double Dfn[2 * 2];
    double values[3];
    double z[2];

    for (int i = 1; i <= k; i++) {
        x[2] += h;

        int iterator = 0;
//        if(std::isnan(x[0]) || std::isnan(x[1])) return i;
        f(x, values, Df);
        stripDf3x2(Df, Dfn);

        while (std::fabs(values[0]) > 10e-14 && std::fabs(values[1]) > 10e-14) {


            if (Dfn[0] * Dfn[3] - Dfn[1] * Dfn[2] == 0 || Dfn[1] * Dfn[2] - Dfn[0] * Dfn[3] == 0 || iterator >= 1000000) {
                return i;
            }

            z[0] = (Dfn[3] * values[0] - Dfn[1] * values[1]) / (Dfn[1] * Dfn[2] - Dfn[0] * Dfn[3]);
            z[1] = (Dfn[2] * values[0] - Dfn[0] * values[1]) / (Dfn[0] * Dfn[3] - Dfn[1] * Dfn[2]);

            x[0] += z[0];
            x[1] += z[1];

            if(std::isnan(x[0]) || std::isnan(x[1]) || std::isinf(x[0]) || std::isinf(x[1])) return i;

            f(x, values, Df);
            stripDf3x2(Df, Dfn);

            iterator++;
        }
        printVector(x, 3);
    }

    return 0;
}

int findSurface(FuncPointer f, double* x, unsigned k1, unsigned k2, double h1, double h2) {
    double Df[3];
    double Dfn;
    double values[1];
    double z;
    double original[3];
    original[0] = x[0];
    original[1] = x[1];
    original[2] = x[2];

    for(unsigned i = 1; i <= k1; i++) {
        x[1] += h1;
        x[2] = original[2];

        for (unsigned j = 1; j <= k2; j++) {
            x[2] += h2;
            int iterator = 0;
            f(x, values, Df);

            while (std::fabs(values[0]) > 10e-14) {
                Dfn = Df[0];
                if (Dfn == 0 || iterator >= 1000000) return i*k1+j;
                z = -1 * (values[0] / Dfn);
                x[0] += z;
                if(std::isnan(x[0]) || std::isinf(x[1])) return i*k1+j;
                f(x, values, Df);
                iterator++;
            }
            printVector(x, 3);
        }
        if (i != k1)
            std::cout << '\n';
    }

    return 0;
}

int findFixedPoints(FuncPointer f, double* x, unsigned k1, unsigned k2, double h1, double h2) {
    double Df[4 * 2];
    double Dfn[2 * 2];
    double values[2];
    double z[2];
    double original[4];
    original[3] = x[3];

    for(int i = 1; i <= k1; i++) {
        x[2] += h1;
        x[3] = original[3];

        for(int j = 1; j <= k2; j++) {
            x[3] += h2;

            values[0] = 1;
            values[1] = 1;

            int iterator = 0;
            while(std::fabs(values[0]) > 10e-14 || std::fabs(values[1]) > 10e-14) {

                f(x, values, Df);
                stripDf4x2(Df, Dfn);

                values[0] -= x[0];
                values[1] -= x[1];

                Dfn[0] -= 1;
                Dfn[3] -= 1;

                if (Dfn[0] * Dfn[3] - Dfn[1] * Dfn[2] == 0 || Dfn[1] * Dfn[2] - Dfn[0] * Dfn[3] == 0 || iterator >= 100) {
                    return i*k1+j;
                }

                z[0] = (Dfn[3] * values[0] - Dfn[1] * values[1]) / (Dfn[1] * Dfn[2] - Dfn[0] * Dfn[3]);
                z[1] = (Dfn[2] * values[0] - Dfn[0] * values[1]) / (Dfn[0] * Dfn[3] - Dfn[1] * Dfn[2]);

                x[0] += z[0];
                x[1] += z[1];


                iterator++;
            }
            printVector(x, 4);
        }
        std::cout << '\n';
    }

    return 0;
}
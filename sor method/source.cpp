#include <iostream>
#include "vectalg.h"

Vector SOR(Vector** A, const Vector &b, const Vector & x0, int L, int M, int N, double w) {
    Vector x = x0;
    int middle = M / 2;
    for(int iteration = 0; iteration < L; iteration++) {
        int firstCoord = middle;
        for(int i = 0; i < N; i++) {
            double temp = b[i];

            for (int j = std::max(0, i - middle), k = firstCoord; j < std::min(i + middle + 1, N); j++, k--) {
                if (i == j) continue;
                int secondValue = i;
                if (k > middle) secondValue -= k - middle;
                temp -= A[k][0][secondValue] * x[j];
            }
            if (firstCoord < M - 1) firstCoord++;

            x[i] = (1-w) * x[i] + w * temp / A[middle][0][i];
        }
    }
    return x;
}

void print(Vector &v) {
    for (int i = 0; i < v.size(); i++) {
        printf("%.16e ", v[i]);
    }
    printf("\n");
}

int main() {
    int N;
    int M;
    std::cin >> N >> M;

    int middleRibbonIndex = M / 2;

    Vector* vectors[M];

    for (int i = -middleRibbonIndex, j = 0; j < M; i++, j++) {
        vectors[j] = new Vector(N - std::abs(i));
        for (int k = 0; k < N - std::abs(i); k++) {
            double temp;
            std::cin >> temp;
            (*vectors[j])[k] = temp;
        }
    }

    Vector* x = new Vector(N);
    Vector* startVector = new Vector(N);
    double w;
    int L;

    std::cin >> *x;
    std::cin >> *startVector;
    std::cin >> w >> L;


    Vector result = SOR(vectors, *x, *startVector, L, M, N, w);

    print(result);

    return 0;
}
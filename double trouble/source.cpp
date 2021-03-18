//Krzysztof Wydrzynski
#include <iostream>
#include <cmath>
#include <cstdio>

class Series {
public:
    Series(double x0, double x1) {
        this->B = (7 * x1 / 48.0) - (x0 / 48.0);
        this->A = (49 * x0 / 48.0) - (7.0 * x1 / 48.0);
    }

    double get(int n) const {
        double a = this->A * pow(1.0/7.0, n);
        double b = this->B * pow(7.0, n);
        return a + b;
    }

private:
    double A;
    double B;
};

int main() {
    int z;
    std::cin >> z;

    for (int i = 0; i < z; i++) {
        double x0;
        double x1;
        int n;
        std::cin >> x0 >> x1;
        std::cin >> n;
        auto* series = new Series(x0, x1);
        for (int j = 0; j < n; j++) {
            int k;
            std::cin >> k;
            printf("%.15e\n", series->get(k));
        }
        delete series;
        std::cout << '\n';
    }
}
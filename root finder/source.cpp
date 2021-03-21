//Krzysztof Wydrzynski

#include <iostream>
#include <cmath>

class FunctionManager {
public:
    double findZero(       double a,
                           double b,
                           int M,
                           double eps,
                           double delta) {
        this->section = new Section(a, b, f(a), f(b));
        this->usedFunctions += 2;
        this->M = M;

        if (sgn(section->fa) != sgn(section->fb)) bisection(0.3);

        secant(eps);

        a = this->section->a;
        double fa = this->section->fa;
        b = this->section->b;
        double fb = this->section->fb;

        if (fa < fb) return a;
        return b;
    }

    FunctionManager(double (*f)(double)) {
        this->f = *f;
    }

private:
    struct Section {
        double a;
        double b;
        double fa;
        double fb;
        Section(double a, double b, double fa, double fb) {
            this->a = a;
            this->b = b;
            this->fa = fa;
            this->fb = fb;
        }
    };

    Section* section;
    double (*f)(double);
    int M;
    int usedFunctions = 0;

    int sgn(double a) {
        return a > 0 ? 1 : 0;
    }

    void bisection(double epsilon) {
        double a = section->a;
        double b = section->b;
        double fa = section->fa;
        double fb = section->fb;
        double c;
        double fc;

        if (sgn(fa) != sgn(fb)) {
            double e = b - a;
            while (std::abs(a - b) > epsilon && usedFunctions < M) {
                e = e / 2;
                c = a + e;
                fc = f(c);
                this->usedFunctions++;
                if (sgn(fa) == sgn(fc)) {
                    a = c;
                    fa = fc;
                } else {
                    b = c;
                    fb = fc;
                }

            }
        }

        if (std::abs(fa) > std::abs(fb)) {
            this->section->a = a;
            this->section->b = b;
            this->section->fa = fa;
            this->section->fb = fb;
        } else {
            this->section->a = b;
            this->section->b = a;
            this->section->fa = fb;
            this->section->fb = fa;
        }
    }

    void secant(double epsilon) {
        double a = section->a;
        double b = section->b;
        double fa = section->fa;
        double fb = section->fb;

        while (usedFunctions < M && std::abs(fa) > epsilon) {
            double s = b - (b - a) * fb / (fb - fa);
            if (std::isnan(s)) break;
            double fs = f(s);

            a = b;
            fa = fb;
            b = s;
            fb = fs;
            usedFunctions++;
        }
        this->section->a = a;
        this->section->b = b;
        this->section->fa = fa;
        this->section->fb = fb;
    }

};


double findZero(
        double (*f)(double),
        double a,
        double b,
        int M,
        double eps,
        double delta
        ) {

    FunctionManager* functionManager = new FunctionManager(f);
    return functionManager->findZero(a, b, M, eps, delta);
}

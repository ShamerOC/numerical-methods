//Krzysztof Wydrzynski

#include <iostream>

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
        std::cout << "-------------------original----------------------\n";
        std::cout << "a: " << a << ", b: " << b << '\n';
        std::cout << "f(a): " << f(a) << ", f(b): " << f(b) << '\n';
        bisection(0.3);
        a = this->section->a;
        b = this->section->b;
        std::cout << "-------------------after bisection----------------------\n";
        std::cout << "a: " << a << ", b: " << b << '\n';
        std::cout << "f(a): " << f(a) << ", f(b): " << f(b) << '\n';
        secant(eps);
        a = this->section->a;
        b = this->section->b;
        std::cout << "-------------------after secant----------------------\n";
        std::cout << "a: " << a << ", b: " << b << '\n';
        std::cout << "f(a): " << f(a) << ", f(b): " << f(b) << '\n';

        return a + (b - a) / 2;
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
        double c = a;
        double fc = fa;

        if (sgn(fa) != sgn(fb)) {
            while (std::abs(fa) > epsilon && std::abs(fb) > epsilon && std::abs(fc) > epsilon) {
                c = a + (b - a) / 2;
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
        } else {
            std::cout << "Wartosci na koncach przedzialow maja takie same znaki\n";
        }

        this->section->a = a;
        this->section->b = b;
    }

    void secant(double epsilon) {
        double a = section->a;
        double b = section->b;
        double fa = section->fa;
        double fb = section->fb;

        while (usedFunctions < M && std::abs(fa) > epsilon) {
            if (std::abs(fa) > std::abs(fb)) {
                double temp;
                temp = a;
                a = b;
                b = temp;
                temp = fa;
                fa = fb;
                fb = temp;
            }

            double s = (b - a) / (fb - fa);
            b = a;
            fb = fa;
            a = a - (fa * s);
            fa = f(a);
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

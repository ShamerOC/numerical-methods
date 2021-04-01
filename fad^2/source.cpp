//Krzysztof Wydrzynski

#include <cmath>
#include "funkcja.h"
#include <iostream>

class Fad {
public:

    void printAndCalculate (double x1, double y1) {
        const Operations* x = new Operations(x1, 1, 0, 0, 0, 0);
        const Operations* y = new Operations(y1, 0, 1, 0, 0, 0);


        Fad::Operations result = funkcja(*x, *y);


        printf("%.15f ", result.f);
        printf("%.15f ", result.fx);
        printf("%.15f ", result.fy);
        printf("%.15f ", result.fxx);
        printf("%.15f ", result.fxy);
        printf("%.15f\n", result.fyy);
    }

    class Operations {
    public:
        double f;
        double fx;
        double fy;
        double fxy;
        double fxx;
        double fyy;

        void setf (double c) {
            this->f = c;
        }

        void setfx (double c) {
            this->fx = c;
        }

        void setfy (double c) {
            this->fy = c;
        }

        void setfxy (double c) {
            this->fxy = c;
        }

        void setfxx (double c) {
            this->fxx = c;
        }

        void setfyy (double c) {
            this->fyy = c;
        }

        Operations(double f, double fx, double fy, double fxy, double fxx, double fyy) {
            this->f = f;
            this->fx = fx;
            this->fy = fy;
            this->fxy = fxy;
            this->fxx = fxx;
            this->fyy = fyy;
        }

        Operations() {
            this->f = 0;
            this->fx = 0;
            this->fy = 0;
            this->fxy = 0;
            this->fxx = 0;
            this->fyy = 0;
        }

        Operations (const Operations &op) {
            this->f = op.f;
            this->fx = op.fx;
            this->fy = op.fy;
            this->fxy = op.fxy;
            this->fxx = op.fxx;
            this->fyy = op.fyy;
        }

        Operations operator+ (const Operations &c) const {
            const Operations* op = new const Operations(f + c.f, fx + c.fx, fy + c.fy, fxy + c.fxy, fxx + c.fxx, fyy + c.fyy);

            return *op;
        }

        Operations operator+ (const double c) const {
            const Operations* op = new const Operations(f + c, fx, fy, fxy, fxx, fyy);

            return *op;
        }


        Operations operator- (const Operations &c) const {
            const Operations* op = new const Operations(f - c.f, fx - c.fx, fy - c.fy, fxy - c.fxy, fxx - c.fxx, fyy - c.fyy);

            return *op;
        }

        Operations operator- (const double c) const {
            const Operations* op = new const Operations(f - c, fx, fy, fxy, fxx, fyy);

            return *op;
        }

        Operations operator* (const Operations &c) const {
            const Operations* op = new const Operations(f * c.f, fx * c.f + f * c.fx, fy * c.f + f * c.fy, fx * c.fy + fy * c.fx + fxy * c.f + f * c.fxy, fxx * c.f + 2 * fx * c.fx + f * c.fxx, fyy * c.f + 2 * fy * c.fy + f * c.fyy);

            return *op;
        }

        Operations operator/ (const Operations &c) const {
            const Operations* op = new const Operations(f / c.f, (fx - (f / c.f * c.fx)) / c.f, (fy - (f / c.f * c.fy)) / c.f, (2 * f * c.fy * c.fx + c.f * c.f * fxy) / (c.f * c.f * c.f) - (c.fy * fx + fy * c.fx + f * c.fxy) / (c.f * c.f), (2*f * c.fx * c.fx + c.f * c.f * fxx ) / (c.f * c.f * c.f) - (2 * fx * c.fx + f * c.fxx) / (c.f * c.f), (2*f * c.fy * c.fy + c.f * c.f * fyy) / (c.f * c.f * c.f) - (2 * fy * c.fy + f * c.fyy) / (c.f * c.f));

            return *op;
        }

        Operations operator/ (const double c) const {
            const Operations* op = new const Operations(f / c, fx / c, fy / c, fxy / c, fxx / c, fyy / c );

            return *op;
        }

        Operations operator-() const {
            const Operations* op = new const Operations(-f, -1 * fx, -1 * fy, -1 * fxy, -1 * fxx, -1 * fyy);

            return *op;
        }

        Operations operator=(const Operations &c) const {
            if (this != &c) {
                const Operations* op = new const Operations(c.f, c.fx, c.fy, c.fxy, c.fxx, c.fyy);
                return *op;
            }

            return *this;
        }

        };

};

Fad::Operations operator* (const double c, const Fad::Operations &b) {
    const Fad::Operations* op = new const Fad::Operations(b.f * c, b.fx * c, b.fy * c, b.fxy * c , c * b.fxx, c * b.fyy);

    return *op;
}

Fad::Operations operator- (const double c, const Fad::Operations &b) {
    const Fad::Operations* op = new const Fad::Operations(c - b.f, -b.fx, -b.fy, -b.fxy, -b.fxx, -b.fyy);

    return *op;
}

Fad::Operations sin (const Fad::Operations &c) {
    const Fad::Operations* op = new const Fad::Operations(std::sin(c.f), c.fx * std::cos(c.f), c.fy * std::cos(c.f), c.fxy * std::cos(c.f) - c.fy * c.fx * sin(c.f), c.fxx * std::cos(c.f) - c.fx * c.fx * std::sin(c.f), c.fyy * std::cos(c.f) - c.fy * c.fy * std::sin(c.f));

    return *op;
}

Fad::Operations cos (const Fad::Operations &c) {
    const Fad::Operations* op = new const Fad::Operations(std::cos(c.f), -1 * c.fx * std::sin(c.f), -1 * c.fy * std::sin(c.f), c.fy * c.fx * -std::cos(c.f) - c.fxy * sin(c.f), c.fx * c.fx * -cos(c.f) - (c.fxx * sin(c.f)), c.fy * c.fy * -cos(c.f) - (c.fyy * sin(c.f)));

    return *op;
}

Fad::Operations exp (const Fad::Operations &c) {
    const Fad::Operations* op = new const Fad::Operations(std::exp(c.f), c.fx * std::exp(c.f), c.fy * std::exp(c.f), std::exp(c.f) * (c.fy * c.fx + c.fxy), exp(c.f) * (c.fxx + c.fx * c.fx), exp(c.f) * (c.fyy + c.fy * c.fy));

    return *op;
}


int main() {

    int numberOfSets;

    std::cin >> numberOfSets;
    Fad* fad = new Fad();

    for (int i = 0; i < numberOfSets; i++) {
        double x;
        double y;

        std::cin >> x >> y;
        fad->printAndCalculate(x, y);
    }

    delete fad;

    return 0;
}
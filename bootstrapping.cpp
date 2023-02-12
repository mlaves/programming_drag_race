#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#include "Eigen/Dense"
#include "Eigen/QR"

using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::Vector2f;
using Eigen::VectorXi;

std::default_random_engine generator;

float stddev(VectorXf x)
{
    return std::sqrt((x.array() - x.mean()).square().sum() / (x.size()));
}

VectorXf model(VectorXf x, Vector2f p)
{
    return p(0) + p(1)*x.array();
}

Vector2f fit(VectorXf x, VectorXf y)
{
    VectorXf::Ones(x.size());
    MatrixXf A(x.size(), 2);

    // A = [1, x]
    A << VectorXf::Ones(x.size()), x;

    // solve
    Vector2f p_pred = A.colPivHouseholderQr().solve(y);

    return p_pred;
}

VectorXf normal_noise(uint samples)
{
    VectorXf values(VectorXf::Zero(samples));

    // fill vector r with normal random variates
    std::normal_distribution<float> distribution(0.0, 1.0);
    auto normal = [&] (float) {return distribution(generator);};
    VectorXf rand = VectorXf::NullaryExpr(samples, normal);

    return rand;
}

VectorXf generate_data(VectorXf x, Vector2f p_true)
{
    VectorXf y(VectorXf(model(x, p_true)));
    VectorXf rand(normal_noise(x.size()));

    return y + 0.1*rand;
}

MatrixXf bootstrap(VectorXf x, VectorXf y, uint num_iters = 1000)
{
    MatrixXf p_boot = MatrixXf::Zero(2, num_iters);

    std::uniform_int_distribution distribution(0, 9);
    auto uniform = [&] () {return distribution(generator);};
    std::vector<int> ind(x.size());

    for (uint i = 0; i < num_iters; i++)
    {
        std::generate(std::begin(ind), std::end(ind), uniform);

        VectorXf x_boot(x(ind));
        VectorXf y_boot(y(ind));

        p_boot.col(i) << fit(x_boot, y_boot);
    }

    return p_boot;
}

int main(int argc, char *argv[])
{
    uint samples = 100;
    uint num_iters = 1000;

    if (argc == 3)
    {
        samples = std::stof(argv[1]);
        num_iters = std::stof(argv[2]);
    }

    VectorXf x(VectorXf::LinSpaced(samples, 0, 10-0.1));
    Vector2f p_true(0.0, 1.0);
    VectorXf data(generate_data(x, p_true));

    auto start = std::chrono::steady_clock::now();
    MatrixXf p_boot = bootstrap(x, data, num_iters);
    auto end = std::chrono::steady_clock::now();
    std::cout << "runtime = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000.0f
              << " ms" << std::endl;

    std::cout << "p[0] mean: " << p_boot.row(0).mean() << std::endl;
    std::cout << "p[1] mean: " << p_boot.row(1).mean() << std::endl;

    std::cout << "p[0] std:  " << stddev(p_boot.row(0)) << std::endl;
    std::cout << "p[1] std:  " << stddev(p_boot.row(1)) << std::endl;

    return 0;
}

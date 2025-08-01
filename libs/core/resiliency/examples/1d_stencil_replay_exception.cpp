//  Copyright (c) 2019 National Technology & Engineering Solutions of Sandia,
//                     LLC (NTESS).
//  Copyright (c) 2019 Adrian Serio
//  Copyright (c) 2019 Nikunj Gupta
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/chrono.hpp>
#include <hpx/init.hpp>
#include <hpx/modules/resiliency.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

int const subdomain_width = 3;
int const subdomains = 3;
int const iterations = 3;
double const cfl = 0.5;
double const checksum_tol = 1.0e-10;

double const pi = std::acos(-1.0);

// Percent of Errors
double const chaos_factor = 5;

class chk_vector
{
public:
    chk_vector()
      : chk_(0.0)
    {
    }
    chk_vector(std::vector<double> data)
      : chk_(std::accumulate(begin(data), end(data), 0.0))
      , data_(std::move(data))
    {
    }
    double chk() const
    {
        return chk_;
    }
    friend std::vector<double>::const_iterator begin(const chk_vector& v)
    {
        return begin(v.data_);
    }
    friend std::vector<double>::const_iterator end(const chk_vector& v)
    {
        return end(v.data_);
    }

private:
    double chk_;
    std::vector<double> data_;
};

using subdomain_future = hpx::shared_future<chk_vector>;

double initial_condition(double x)
{
    return std::sin(2.0 * pi * x);
}

subdomain_future init(int subdomain_index)
{
    std::vector<double> data(subdomain_width + 1);
    for (int k = 0; k < subdomain_width + 1; ++k)
    {
        data[k] =
            initial_condition((0.0 + subdomain_width * subdomain_index + k) /
                (subdomain_width * subdomains));
    }
    return hpx::make_ready_future(chk_vector(data));
}

double stencil(double left, double center, double right)
{
    const double cc = cfl * cfl;
    // Randomly return an incorrect result.
    if (std::rand() % 100 < chaos_factor)
    {
        return 0.4 * (cfl + cc) * left + (1.0 - cc) * center +
            0.5 * (-cfl + cc) * right;
    }
    // Correct Calculation
    else
        return 0.5 * (cfl + cc) * left + (1.0 - cc) * center +
            0.5 * (-cfl + cc) * right;
}

double left_flux(double left, double center)
{
    const double cc = cfl * cfl;
    return (1.0 - 0.5 * cfl - 0.5 * cc) * left + 0.5 * (-cfl + cc) * center;
}

double right_flux(double center, double right)
{
    const double cc = cfl * cfl;
    return 0.5 * (cfl + cc) * center + (1.0 + 0.5 * cfl - 0.5 * cc) * right;
}

struct validate_exception : std::exception
{
};

chk_vector update(subdomain_future left_input, subdomain_future center_input,
    subdomain_future right_input)
{
    const auto left = left_input.get();
    const auto center = center_input.get();
    const auto right = right_input.get();
    std::vector<double> workspace(3 * subdomain_width + 1);
    std::copy(begin(left), end(left) - 1, &workspace[0]);
    std::copy(begin(center), end(center) - 1, &workspace[subdomain_width]);
    std::copy(begin(right), end(right),
        // NOLINTNEXTLINE(bugprone-misplaced-widening-cast)
        &workspace[static_cast<std::size_t>(2 * subdomain_width)]);
    double checksum = left.chk() - *begin(center) + center.chk() -
        *begin(right) + right.chk();
    for (int t = 0; t < subdomain_width; ++t)
    {
        checksum -= left_flux(workspace[0], workspace[1]);
        checksum -= right_flux(workspace[3 * subdomain_width - 1 - 2 * t],
            workspace[3 * subdomain_width - 2 * t]);
        for (int k = 0; k < 3 * subdomain_width - 1 - 2 * t; ++k)
        {
            workspace[k] =
                stencil(workspace[k], workspace[k + 1], workspace[k + 2]);
        }
    }
    workspace.resize(subdomain_width + 1);
    chk_vector result(std::move(workspace));
    if (std::abs(result.chk() - checksum) > checksum_tol)
    {
        std::cout << "Failed Validation -- Attempting Replay!" << std::endl;
        throw validate_exception();
    }
    return result;
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    std::size_t n = vm["n-value"].as<std::size_t>();

    {
        // Initialize a high resolution timer
        hpx::chrono::high_resolution_timer t;

        std::srand(1);
        std::vector<subdomain_future> input(subdomains);
        for (int j = 0; j < subdomains; ++j)
        {
            input[j] = init(j);
        }
        for (int i = 0; i < iterations; ++i)
        {
            std::vector<subdomain_future> next_input(subdomains);
            for (int j = 0; j < subdomains; ++j)
            {
                next_input[j] = hpx::resiliency::experimental::dataflow_replay(
                    n, update, input[(j - 1 + subdomains) % subdomains],
                    input[j], input[(j + 1) % subdomains]);
            }
            std::swap(input, next_input);
        }
        hpx::wait_all(input);
        try
        {
            for (auto& f : input)
            {
                const auto v = f.get();
                for (auto it = begin(v); it != end(v) - 1; ++it)
                {
                    std::cout << *it << " ";
                }
            }
            std::cout << std::endl;
        }
        catch (std::exception const& e)
        {
            std::cout << e.what() << std::endl;
        }

        double elapsed = t.elapsed();
        hpx::util::format_to(std::cout, "Time elapsed == {1}\n", elapsed);
    }

    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    using hpx::program_options::options_description;
    using hpx::program_options::value;

    // Configure application specific options
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("n-value",
        value<std::size_t>()->default_value(10),
        "Maximum number of repeat launches for a function f");

    // Initialize and run HPX
    hpx::local::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}

//  Copyright (c) 2011-2013 Thomas Heller
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/chrono.hpp>
#include <hpx/future.hpp>

#include "jacobi.hpp"

#include <cmath>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace jacobi_smp {

    void jacobi_kernel_wrap(range const& y_range, std::size_t n,
        std::vector<double>& dst, std::vector<double> const& src)
    {
        for (std::size_t y = y_range.begin(); y < y_range.end(); ++y)
        {
            double* dst_ptr = &dst[y * n];
            const double* src_ptr = &src[y * n];
            jacobi_kernel(dst_ptr, src_ptr, n);
        }
    }

    void jacobi(std::size_t n, std::size_t iterations, std::size_t block_size,
        std::string const& output_filename)
    {
        typedef std::vector<double> vector;

        std::shared_ptr<vector> grid_new(new vector(n * n, 1));
        std::shared_ptr<vector> grid_old(new vector(n * n, 1));

        typedef std::vector<hpx::shared_future<void>> deps_vector;

        std::size_t n_block =
            static_cast<std::size_t>(std::ceil(double(n) / double(block_size)));

        std::shared_ptr<deps_vector> deps_new(
            new deps_vector(n_block, hpx::make_ready_future()));
        std::shared_ptr<deps_vector> deps_old(
            new deps_vector(n_block, hpx::make_ready_future()));

        hpx::chrono::high_resolution_timer t;
        for (std::size_t i = 0; i < iterations; ++i)
        {
            for (std::size_t y = 1, j = 0; y < n - 1; y += block_size, ++j)
            {
                std::size_t y_end = (std::min) (y + block_size, n - 1);
                std::vector<hpx::shared_future<void>> trigger;
                trigger.reserve(3);
                trigger.push_back((*deps_old)[j]);
                if (j > 0)
                    trigger.push_back((*deps_old)[j - 1]);
                if (j + 1 < n_block)
                    trigger.push_back((*deps_old)[j + 1]);

                /*
                 * FIXME: dataflow seems to have some raceconditions
                 * left
                (*deps_new)[j]
                    = hpx::dataflow(
                        hpx::bind(
                            jacobi_kernel_wrap
                          , range(y, y_end)
                          , n
                          , std::ref(*grid_new)
                          , std::cref(*grid_old)
                        )
                      , trigger
                    );
                */

                (*deps_new)[j] =
                    hpx::when_all(std::move(trigger))
                        .then(hpx::launch::async,
                            hpx::bind(jacobi_kernel_wrap, range(y, y_end), n,
                                std::ref(*grid_new), std::cref(*grid_old)));
            }

            std::swap(grid_new, grid_old);
            std::swap(deps_new, deps_old);
        }
        hpx::wait_all(*deps_new);
        hpx::wait_all(*deps_old);

        report_timing(n, iterations, t.elapsed());
        output_grid(output_filename, *grid_old, n);
    }
}    // namespace jacobi_smp

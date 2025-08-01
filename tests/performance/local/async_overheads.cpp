//  Copyright (c) 2007-2025 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/chrono.hpp>
#include <hpx/future.hpp>
#include <hpx/init.hpp>
#include <hpx/modules/testing.hpp>

#include "worker_timed.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
std::size_t num_level_tasks = 16;
std::size_t spread = 2;
std::uint64_t delay_ns = 0;

void test_func()
{
    worker_timed(delay_ns);
}

///////////////////////////////////////////////////////////////////////////////
hpx::future<void> spawn_level(std::size_t num_tasks)
{
    std::vector<hpx::future<void>> tasks;
    tasks.reserve(num_tasks);

    // spawn sub-levels
    if (num_tasks > num_level_tasks && spread > 1)
    {
        std::size_t spawn_hierarchically = num_tasks - num_level_tasks;
        std::size_t num_sub_tasks = 0;
        if (spawn_hierarchically < num_level_tasks)
            num_sub_tasks = spawn_hierarchically;
        else
            num_sub_tasks = spawn_hierarchically / spread;

        for (std::size_t i = 0; i != spread && spawn_hierarchically != 0; ++i)
        {
            std::size_t sub_spawn =
                (std::min) (spawn_hierarchically, num_sub_tasks);
            spawn_hierarchically -= sub_spawn;
            num_tasks -= sub_spawn;

            // force unwrapping
            hpx::future<void> f = hpx::async(&spawn_level, sub_spawn);
            tasks.push_back(std::move(f));
        }
    }

    // then spawn required number of tasks on this level
    for (std::size_t i = 0; i != num_tasks; ++i)
        tasks.push_back(hpx::async(&test_func));

    return hpx::when_all(tasks);
}

///////////////////////////////////////////////////////////////////////////////
int hpx_main(hpx::program_options::variables_map& vm)
{
    std::size_t num_tasks = 128;
    if (vm.count("tasks"))
        num_tasks = vm["tasks"].as<std::size_t>();

    double seqential_time_per_task = 0;

    {
        std::vector<hpx::future<void>> tasks;
        tasks.reserve(num_tasks);

        std::uint64_t start = hpx::chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i != num_tasks; ++i)
            tasks.push_back(hpx::async(&test_func));

        hpx::wait_all(tasks);

        std::uint64_t end = hpx::chrono::high_resolution_clock::now();

        seqential_time_per_task = static_cast<double>(end - start) / 1e9 /
            static_cast<double>(num_tasks);
        std::cout << "Elapsed sequential time: "
                  << static_cast<double>(end - start) / 1e9 << " [s], ("
                  << seqential_time_per_task << " [s])" << std::endl;
        hpx::util::print_cdash_timing(
            "AsyncSequential", seqential_time_per_task);
    }

    double hierarchical_time_per_task = 0;

    {
        std::uint64_t start = hpx::chrono::high_resolution_clock::now();

        hpx::future<void> f = hpx::async(&spawn_level, num_tasks);
        hpx::wait_all(f);

        std::uint64_t end = hpx::chrono::high_resolution_clock::now();

        hierarchical_time_per_task = static_cast<double>(end - start) / 1e9 /
            static_cast<double>(num_tasks);
        std::cout << "Elapsed hierarchical time: "
                  << static_cast<double>(end - start) / 1e9 << " [s], ("
                  << hierarchical_time_per_task << " [s])" << std::endl;
        hpx::util::print_cdash_timing(
            "AsyncHierarchical", hierarchical_time_per_task);
    }

    std::cout << "Ratio (speedup): "
              << seqential_time_per_task / hierarchical_time_per_task
              << std::endl;

    hpx::util::print_cdash_timing(
        "AsyncSpeedup", seqential_time_per_task / hierarchical_time_per_task);

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    using namespace hpx::program_options;
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    // clang-format off
    desc_commandline.add_options()
        ("tasks,t", value<std::size_t>(),
         "number of tasks to spawn sequentially (default: 128)")
        ("sub-tasks,s", value<std::size_t>(&num_level_tasks)->default_value(16),
         "number of tasks spawned per sub-spawn (default: 16)")
        ("spread,p", value<std::size_t>(&spread)->default_value(2),
         "number of sub-spawns per level (default: 2)")
        ("delay,d", value<std::uint64_t>(&delay_ns)->default_value(0),
        "time spent in the delay loop [ns]");
    // clang-format on

    // Initialize and run HPX
    hpx::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    return hpx::init(argc, argv, init_args);
}

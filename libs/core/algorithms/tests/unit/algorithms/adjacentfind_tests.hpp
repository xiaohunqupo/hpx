//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2024 Tobias Wukovitsch
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/init.hpp>
#include <hpx/modules/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(2, 101);
std::uniform_int_distribution<> dist(2, 10005);

template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    // fill vector with random values about 1
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 1;
    c[random_pos + 1] = 1;

    iterator index = hpx::adjacent_find(
        policy, iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index =
        std::begin(c) + static_cast<std::ptrdiff_t>(random_pos);

    HPX_TEST(index == iterator(test_index));
}

#if defined(HPX_HAVE_STDEXEC)
template <typename LnPolicy, typename ExPolicy, typename IteratorTag>
void test_adjacent_find_sender(
    LnPolicy ln_policy, ExPolicy&& ex_policy, IteratorTag)
{
    static_assert(hpx::is_async_execution_policy_v<ExPolicy>,
        "hpx::is_async_execution_policy_v<ExPolicy>");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values about 1
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 1;
    c[random_pos + 1] = 1;

    namespace ex = hpx::execution::experimental;
    namespace tt = hpx::this_thread::experimental;
    using scheduler_t = ex::thread_pool_policy_scheduler<LnPolicy>;

    auto exec = ex::explicit_scheduler_executor(scheduler_t(ln_policy));

    {
        auto snd_result = tt::sync_wait(
            ex::just(iterator(std::begin(c)), iterator(std::end(c))) |
            hpx::adjacent_find(ex_policy.on(exec)));

        iterator index = hpx::get<0>(*snd_result);
        base_iterator test_index =
            std::begin(c) + static_cast<std::ptrdiff_t>(random_pos);

        HPX_TEST(index == iterator(test_index));
    }

    {
        // edge case: empty range

        auto snd_result = tt::sync_wait(
            ex::just(iterator(std::begin(c)), iterator(std::begin(c))) |
            hpx::adjacent_find(ex_policy.on(exec)));
        auto result = hpx::get<0>(*snd_result);

        HPX_TEST(iterator(std::begin(c)) == result);
    }
}
#endif

template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    // fill vector with random values above 1
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 1;
    c[random_pos + 1] = 1;

    hpx::future<iterator> f =
        hpx::adjacent_find(p, iterator(std::begin(c)), iterator(std::end(c)));
    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index =
        std::begin(c) + static_cast<std::ptrdiff_t>(random_pos);

    HPX_TEST(f.get() == iterator(test_index));
}

////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find_exception(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);

    bool caught_exception = false;
    try
    {
        hpx::adjacent_find(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)));
        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find_exception_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);

    bool caught_exception = false;
    bool returned_from_algorithm = false;

    try
    {
        hpx::future<decorated_iterator> f = hpx::adjacent_find(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)));

        returned_from_algorithm = true;

        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
    HPX_TEST(returned_from_algorithm);
}

////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);

    bool caught_bad_alloc = false;
    try
    {
        hpx::adjacent_find(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)));
        HPX_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename ExPolicy, typename IteratorTag>
void test_adjacent_find_bad_alloc_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;

    try
    {
        hpx::future<decorated_iterator> f = hpx::adjacent_find(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)));

        returned_from_algorithm = true;

        f.get();

        HPX_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
    HPX_TEST(returned_from_algorithm);
}

//  Copyright (c) 2014 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/modules/testing.hpp>

#include <cstddef>
#include <iterator>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);

struct set_42
{
    template <typename T>
    void operator()(T& val)
    {
        val = T(42);
    }
};

struct throw_always
{
    template <typename T>
    void operator()(T)
    {
        throw std::runtime_error("test");
    }
};

struct throw_bad_alloc
{
    template <typename T>
    void operator()(T)
    {
        throw std::bad_alloc();
    }
};

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_each(ExPolicy&& policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    hpx::for_each(std::forward<ExPolicy>(policy), iterator(std::begin(c)),
        iterator(std::end(c)), set_42());

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c), std::end(c), [&count](int v) -> void {
        HPX_TEST_EQ(v, int(42));
        ++count;
    });
    HPX_TEST_EQ(count, c.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_for_each_async(ExPolicy&& p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    hpx::future<void> f = hpx::for_each(std::forward<ExPolicy>(p),
        iterator(std::begin(c)), iterator(std::end(c)), set_42());
    f.get();

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c), std::end(c), [&count](int v) -> void {
        HPX_TEST_EQ(v, int(42));
        ++count;
    });
    HPX_TEST_EQ(count, c.size());
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_each_exception(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        hpx::for_each(policy, iterator(std::begin(c)), iterator(std::end(c)),
            throw_always());

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
void test_for_each_exception_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        hpx::future<iterator> f = hpx::for_each(
            p, iterator(std::begin(c)), iterator(std::end(c)), throw_always());
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

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_each_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        hpx::for_each(policy, iterator(std::begin(c)), iterator(std::end(c)),
            throw_bad_alloc());

        HPX_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_for_each_bad_alloc_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        hpx::future<iterator> f = hpx::for_each(p, iterator(std::begin(c)),
            iterator(std::end(c)), throw_bad_alloc());
        returned_from_algorithm = true;
        f.get();

        HPX_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
    HPX_TEST(returned_from_algorithm);
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_each_n(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    iterator result =
        hpx::for_each_n(policy, iterator(std::begin(c)), c.size(), set_42());
    iterator end = iterator(std::end(c));
    HPX_TEST(result == end);

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c), std::end(c), [&count](int v) -> void {
        HPX_TEST_EQ(v, int(42));
        ++count;
    });
    HPX_TEST_EQ(count, c.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_for_each_n_async(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    hpx::future<iterator> f =
        hpx::for_each_n(p, iterator(std::begin(c)), c.size(), set_42());
    HPX_TEST_EQ(f.get(), iterator(std::end(c)));

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c), std::end(c), [&count](int v) -> void {
        HPX_TEST_EQ(v, int(42));
        ++count;
    });
    HPX_TEST_EQ(count, c.size());
}

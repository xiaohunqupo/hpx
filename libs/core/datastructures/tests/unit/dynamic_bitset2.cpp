//  Copyright (c) 2022-2025 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0 Distributed under the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// This code was adapted from boost dynamic_bitset
//
// Copyright (c) 2001 Jeremy Siek
// Copyright (c) 2003-2006 Gennaro Prota
// Copyright (c) 2014 Ahmed Charles
// Copyright (c) 2018 Evgeny Shulgin

#include <hpx/config.hpp>
#include <hpx/datastructures/detail/dynamic_bitset.hpp>

#include <cstddef>
#include <string>

#include "bitset_test.hpp"

template <typename Block>
void run_test_cases()
{
    typedef hpx::detail::dynamic_bitset<Block> bitset_type;
    typedef bitset_test<bitset_type> Tests;
    int const bits_per_block = bitset_type::bits_per_block;

    std::string long_string = get_long_string();

    //=====================================================================
    // Test operator&=
    {
        hpx::detail::dynamic_bitset<Block> lhs, rhs;
        Tests::and_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("1")),
            rhs(std::string("0"));
        Tests::and_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string.size(), 0),
            rhs(long_string);
        Tests::and_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string.size(), 1),
            rhs(long_string);
        Tests::and_assignment(lhs, rhs);
    }
    //=====================================================================
    // Test operator |=
    {
        hpx::detail::dynamic_bitset<Block> lhs, rhs;
        Tests::or_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("1")),
            rhs(std::string("0"));
        Tests::or_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string.size(), 0),
            rhs(long_string);
        Tests::or_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string.size(), 1),
            rhs(long_string);
        Tests::or_assignment(lhs, rhs);
    }
    //=====================================================================
    // Test operator^=
    {
        hpx::detail::dynamic_bitset<Block> lhs, rhs;
        Tests::xor_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("1")),
            rhs(std::string("0"));
        Tests::xor_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("0")),
            rhs(std::string("1"));
        Tests::xor_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string), rhs(long_string);
        Tests::xor_assignment(lhs, rhs);
    }
    //=====================================================================
    // Test operator-=
    {
        hpx::detail::dynamic_bitset<Block> lhs, rhs;
        Tests::sub_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("1")),
            rhs(std::string("0"));
        Tests::sub_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(std::string("0")),
            rhs(std::string("1"));
        Tests::sub_assignment(lhs, rhs);
    }
    {
        hpx::detail::dynamic_bitset<Block> lhs(long_string), rhs(long_string);
        Tests::sub_assignment(lhs, rhs);
    }
    //=====================================================================
    // Test operator<<=
    {    // case pos == 0
        std::size_t pos = 0;
        {
            hpx::detail::dynamic_bitset<Block> b;
            Tests::shift_left_assignment(b, pos);
        }
        {
            hpx::detail::dynamic_bitset<Block> b(std::string("1010"));
            Tests::shift_left_assignment(b, pos);
        }
        {
            hpx::detail::dynamic_bitset<Block> b(long_string);
            Tests::shift_left_assignment(b, pos);
        }
    }
    {
        // test with both multiple and
        // non multiple of bits_per_block
        int const how_many = 10;
        for (std::size_t i = 1; i <= how_many; ++i)
        {
            std::size_t multiple = i * bits_per_block;
            std::size_t non_multiple = multiple - 1;
            hpx::detail::dynamic_bitset<Block> b(long_string);

            Tests::shift_left_assignment(b, multiple);
            Tests::shift_left_assignment(b, non_multiple);
        }
    }
    {    // case pos == size()/2
        std::size_t pos = long_string.size() / 2;
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::shift_left_assignment(b, pos);
    }
    {    // case pos >= n
        std::size_t pos = long_string.size();
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::shift_left_assignment(b, pos);
    }
    //=====================================================================
    // Test operator>>=
    {    // case pos == 0
        std::size_t pos = 0;
        {
            hpx::detail::dynamic_bitset<Block> b;
            Tests::shift_right_assignment(b, pos);
        }
        {
            hpx::detail::dynamic_bitset<Block> b(std::string("1010"));
            Tests::shift_right_assignment(b, pos);
        }
        {
            hpx::detail::dynamic_bitset<Block> b(long_string);
            Tests::shift_right_assignment(b, pos);
        }
    }
    {
        // test with both multiple and
        // non multiple of bits_per_block
        int const how_many = 10;
        for (std::size_t i = 1; i <= how_many; ++i)
        {
            std::size_t multiple = i * bits_per_block;
            std::size_t non_multiple = multiple - 1;
            hpx::detail::dynamic_bitset<Block> b(long_string);

            Tests::shift_right_assignment(b, multiple);
            Tests::shift_right_assignment(b, non_multiple);
        }
    }
    {    // case pos == size()/2
        std::size_t pos = long_string.size() / 2;
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::shift_right_assignment(b, pos);
    }
    {    // case pos >= n
        std::size_t pos = long_string.size();
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::shift_right_assignment(b, pos);
    }
    //=====================================================================
    // test b.set()
    {
        hpx::detail::dynamic_bitset<Block> b;
        Tests::set_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::set_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_all(b);
    }
    //=====================================================================
    // Test b.set(pos)
    {    // case pos >= b.size()
        hpx::detail::dynamic_bitset<Block> b;
        Tests::set_one(b, 0, true);
        Tests::set_one(b, 0, false);
    }
    {    // case pos < b.size()
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::set_one(b, 0, true);
        Tests::set_one(b, 0, false);
    }
    {    // case pos == b.size() / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_one(b, long_string.size() / 2, true);
        Tests::set_one(b, long_string.size() / 2, false);
    }
    //=====================================================================
    // Test b.set(pos, len)
    {    // case size is 1
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::set_segment(b, 0, 1, true);
        Tests::set_segment(b, 0, 1, false);
    }
    {    // case fill the whole set
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_segment(b, 0, b.size(), true);
        Tests::set_segment(b, 0, b.size(), false);
    }
    {    // case pos = size / 4, len = size / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_segment(b, b.size() / 4, b.size() / 2, true);
        Tests::set_segment(b, b.size() / 4, b.size() / 2, false);
    }
    {    // case pos = block_size / 2, len = size - block_size
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_segment(b,
            hpx::detail::dynamic_bitset<Block>::bits_per_block / 2,
            b.size() - hpx::detail::dynamic_bitset<Block>::bits_per_block,
            true);
        Tests::set_segment(b,
            hpx::detail::dynamic_bitset<Block>::bits_per_block / 2,
            b.size() - hpx::detail::dynamic_bitset<Block>::bits_per_block,
            false);
    }
    {    // case pos = 1, len = size - 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_segment(b, 1, b.size() - 2, true);
        Tests::set_segment(b, 1, b.size() - 2, false);
    }
    {    // case pos = 3, len = 7
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::set_segment(b, 3, 7, true);
        Tests::set_segment(b, 3, 7, false);
    }
    //=====================================================================
    // Test b.reset()
    {
        hpx::detail::dynamic_bitset<Block> b;
        Tests::reset_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::reset_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_all(b);
    }
    //=====================================================================
    // Test b.reset(pos)
    {    // case pos >= b.size()
        hpx::detail::dynamic_bitset<Block> b;
        Tests::reset_one(b, 0);
    }
    {    // case pos < b.size()
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::reset_one(b, 0);
    }
    {    // case pos == b.size() / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_one(b, long_string.size() / 2);
    }
    //=====================================================================
    // Test b.reset(pos, len)
    {    // case size is 1
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::reset_segment(b, 0, 1);
    }
    {    // case fill the whole set
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_segment(b, 0, b.size());
    }
    {    // case pos = size / 4, len = size / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_segment(b, b.size() / 4, b.size() / 2);
    }
    {    // case pos = block_size / 2, len = size - block_size
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_segment(b,
            hpx::detail::dynamic_bitset<Block>::bits_per_block / 2,
            b.size() - hpx::detail::dynamic_bitset<Block>::bits_per_block);
    }
    {    // case pos = 1, len = size - 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_segment(b, 1, b.size() - 2);
    }
    {    // case pos = 3, len = 7
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::reset_segment(b, 3, 7);
    }
    //=====================================================================
    // Test ~b
    {
        hpx::detail::dynamic_bitset<Block> b;
        Tests::operator_flip(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(std::string("1"));
        Tests::operator_flip(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::operator_flip(b);
    }
    //=====================================================================
    // Test b.flip()
    {
        hpx::detail::dynamic_bitset<Block> b;
        Tests::flip_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(std::string("1"));
        Tests::flip_all(b);
    }
    {
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_all(b);
    }
    //=====================================================================
    // Test b.flip(pos)
    {    // case pos >= b.size()
        hpx::detail::dynamic_bitset<Block> b;
        Tests::flip_one(b, 0);
    }
    {    // case pos < b.size()
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::flip_one(b, 0);
    }
    {    // case pos == b.size() / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_one(b, long_string.size() / 2);
    }
    //=====================================================================
    // Test b.flip(pos, len)
    {    // case size is 1
        hpx::detail::dynamic_bitset<Block> b(std::string("0"));
        Tests::flip_segment(b, 0, 1);
    }
    {    // case fill the whole set
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_segment(b, 0, b.size());
    }
    {    // case pos = size / 4, len = size / 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_segment(b, b.size() / 4, b.size() / 2);
    }
    {    // case pos = block_size / 2, len = size - block_size
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_segment(b,
            hpx::detail::dynamic_bitset<Block>::bits_per_block / 2,
            b.size() - hpx::detail::dynamic_bitset<Block>::bits_per_block);
    }
    {    // case pos = 1, len = size - 2
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_segment(b, 1, b.size() - 2);
    }
    {    // case pos = 3, len = 7
        hpx::detail::dynamic_bitset<Block> b(long_string);
        Tests::flip_segment(b, 3, 7);
    }
}

int main()
{
    run_test_cases<unsigned char>();
    run_test_cases<unsigned short>();
    run_test_cases<unsigned int>();
    run_test_cases<unsigned long>();
    run_test_cases<unsigned long long>();

    return hpx::util::report_errors();
}

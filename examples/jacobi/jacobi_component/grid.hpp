
//  Copyright (c) 2012 Thomas Heller
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <hpx/config.hpp>
#if !defined(HPX_COMPUTE_DEVICE_CODE)
#include "row.hpp"

#include <cstddef>
#include <vector>

namespace jacobi {
    struct row;

    struct HPX_COMPONENT_EXPORT grid
    {
        grid() {}
        grid(std::size_t nx, std::size_t ny, double value);

        typedef std::vector<row> rows_type;

        rows_type rows;

        template <typename Archive>
        void serialize(Archive& ar, unsigned)
        {
            ar & rows;
        }
    };
}    // namespace jacobi

#endif

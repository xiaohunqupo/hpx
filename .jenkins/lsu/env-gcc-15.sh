# Copyright (c) 2025 Alireza Kheirkhahan
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

module purge
module load cmake
module load gcc/15
module load boost/1.88.0-${build_type,,}
module load hwloc
module load openmpi

export HPXRUN_RUNWRAPPER=srun
export CXX_STD="23"
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH

configure_extra_options+=" -DHPX_WITH_CXX_STANDARD=${CXX_STD}"
configure_extra_options+=" -DHPX_WITH_MALLOC=system"
configure_extra_options+=" -DHPX_WITH_FETCH_ASIO=ON"
configure_extra_options+=" -DHPX_WITH_COMPILER_WARNINGS=ON"
configure_extra_options+=" -DHPX_WITH_COMPILER_WARNINGS_AS_ERRORS=ON"
configure_extra_options+=" -DHPX_WITH_PARCELPORT_MPI=ON"
configure_extra_options+=" -DHPX_WITH_PARCELPORT_LCI=ON"
configure_extra_options+=" -DHPX_WITH_FETCH_LCI=ON"
configure_extra_options+=" -DCMAKE_C_COMPILER=gcc"
configure_extra_options+=" -DCMAKE_C_FLAGS=-fPIC"
configure_extra_options+=" -DHPX_WITH_DATAPAR_BACKEND=EVE"
configure_extra_options+=" -DHPX_WITH_FETCH_EVE=ON"
configure_extra_options+=" -DHPX_WITH_EVE_TAG=main"

# The pwrapi library still needs to be set up properly on rostam
# configure_extra_options+=" -DHPX_WITH_POWER_COUNTER=ON"

# enable additional handshaking in MPI parcelport
configure_extra_options+=" -DHPX_WITH_TESTS_COMMAND_LINE=--hpx:ini=hpx.parcel.mpi.ack_handshake!=1"

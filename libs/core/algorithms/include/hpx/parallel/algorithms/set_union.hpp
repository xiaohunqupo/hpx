//  Copyright (c) 2007-2023 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/set_union.hpp
/// \page hpx::set_union
/// \headerfile hpx/algorithm.hpp

#pragma once

#if defined(DOXYGEN)
namespace hpx {
    // clang-format off

    /// Constructs a sorted range beginning at dest consisting of all elements
    /// present in one or both sorted ranges [first1, last1) and
    /// [first2, last2). This algorithm expects both input ranges to be sorted
    /// with the given binary predicate \a pred. Executed according to the policy.
    ///
    /// \note   Complexity: At most 2*(N1 + N2 - 1) comparisons, where \a N1 is
    ///         the length of the first sequence and \a N2 is the length of the
    ///         second sequence.
    ///
    /// If some element is found \a m times in [first1, last1) and \a n times
    /// in [first2, last2), then all \a m elements will be copied from
    /// [first1, last1) to dest, preserving order, and then exactly
    /// std::max(n-m, 0) elements will be copied from [first2, last2) to dest,
    /// also preserving order.
    ///
    /// The resulting range cannot overlap with either of the input ranges.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it applies user-provided function objects.
    /// \tparam FwdIter1    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam FwdIter3    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator or output iterator and sequential
    ///                     execution.
    /// \tparam Op          The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a set_union requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the sequence of elements
    ///                     of the first range the algorithm will be applied to.
    /// \param last1        Refers to the end of the sequence of elements of
    ///                     the first range the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the sequence of elements
    ///                     of the second range the algorithm will be applied to.
    /// \param last2        Refers to the end of the sequence of elements of
    ///                     the second range the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param op           The binary predicate which returns true if the
    ///                     elements should be treated as equal. The signature
    ///                     of the predicate function should be equivalent to
    ///                     the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type1 must be such
    ///                     that objects of type \a FwdIter can
    ///                     be dereferenced and then implicitly converted to
    ///                     \a Type1
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with a sequential execution policy object execute in sequential
    /// order in the calling thread (\a sequenced_policy) or in a
    /// single new thread spawned from the current thread
    /// (for \a sequenced_task_policy).
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a set_union algorithm returns a \a hpx::future<FwdIter3>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter3 otherwise.
    ///           The \a set_union algorithm returns the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename FwdIter3, typename Pred = hpx::parallel::detail::less>
    hpx::parallel::util::detail::algorithm_result_t<ExPolicy, FwdIter3>
    set_union(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, FwdIter2 last2, FwdIter3 dest, Pred&& op = Pred());

    /// Constructs a sorted range beginning at dest consisting of all elements
    /// present in one or both sorted ranges [first1, last1) and
    /// [first2, last2). This algorithm expects both input ranges to be sorted
    /// with the given binary predicate \a pred. Executed according to the policy.
    ///
    /// \note   Complexity: At most 2*(N1 + N2 - 1) comparisons, where \a N1 is
    ///         the length of the first sequence and \a N2 is the length of the
    ///         second sequence.
    ///
    /// If some element is found \a m times in [first1, last1) and \a n times
    /// in [first2, last2), then all \a m elements will be copied from
    /// [first1, last1) to dest, preserving order, and then exactly
    /// std::max(n-m, 0) elements will be copied from [first2, last2) to dest,
    /// also preserving order.
    ///
    /// The resulting range cannot overlap with either of the input ranges.
    ///
    /// \tparam FwdIter1    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam FwdIter3    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator or output iterator and sequential
    ///                     execution.
    /// \tparam Op          The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a set_union requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
    ///
    /// \param first1       Refers to the beginning of the sequence of elements
    ///                     of the first range the algorithm will be applied to.
    /// \param last1        Refers to the end of the sequence of elements of
    ///                     the first range the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the sequence of elements
    ///                     of the second range the algorithm will be applied to.
    /// \param last2        Refers to the end of the sequence of elements of
    ///                     the second range the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param op           The binary predicate which returns true if the
    ///                     elements should be treated as equal. The signature
    ///                     of the predicate function should be equivalent to
    ///                     the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type1 must be such
    ///                     that objects of type \a FwdIter can
    ///                     be dereferenced and then implicitly converted to
    ///                     \a Type1
    ///
    /// \returns  The \a set_union algorithm returns a \a FwdIter3.
    ///           The \a set_union algorithm returns the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename FwdIter1, typename FwdIter2, typename FwdIter3,
        typename Pred = hpx::parallel::detail::less>
    FwdIter3 set_union(FwdIter1 first1, FwdIter1 last1, FwdIter2 first2,
        FwdIter2 last2, FwdIter3 dest, Pred&& op = Pred());

    // clang-format on
}    // namespace hpx

#else    // DOXYGEN

#include <hpx/config.hpp>
#include <hpx/concepts/concepts.hpp>
#include <hpx/execution/algorithms/detail/predicates.hpp>
#include <hpx/executors/execution_policy.hpp>
#include <hpx/functional/invoke.hpp>
#include <hpx/iterator_support/traits/is_iterator.hpp>
#include <hpx/parallel/algorithms/copy.hpp>
#include <hpx/parallel/algorithms/detail/dispatch.hpp>
#include <hpx/parallel/algorithms/detail/set_operation.hpp>
#include <hpx/parallel/util/detail/algorithm_result.hpp>
#include <hpx/parallel/util/detail/sender_util.hpp>
#include <hpx/parallel/util/result_types.hpp>
#include <hpx/type_support/identity.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace hpx::parallel {
    ///////////////////////////////////////////////////////////////////////////
    // set_union
    namespace detail {

        template <typename Iter1, typename Sent1, typename Iter2,
            typename Sent2, typename Iter3, typename Comp, typename Proj1,
            typename Proj2>
        constexpr util::in_in_out_result<Iter1, Iter2, Iter3>
        sequential_set_union(Iter1 first1, Sent1 last1, Iter2 first2,
            Sent2 last2, Iter3 dest, Comp&& comp, Proj1&& proj1, Proj2&& proj2)
        {
            for (; first1 != last1; ++dest)
            {
                if (first2 == last2)
                {
                    auto result = util::copy(first1, last1, dest);
                    return {result.in, first2, result.out};
                }

                auto&& value1 = HPX_INVOKE(proj1, *first1);
                auto&& value2 = HPX_INVOKE(proj2, *first2);

                if (HPX_INVOKE(comp, value2, value1))
                {
                    *dest = *first2++;
                }
                else
                {
                    *dest = *first1;
                    if (!HPX_INVOKE(comp, value1, value2))
                    {
                        ++first2;
                    }
                    ++first1;
                }
            }

            auto result = util::copy(first2, last2, dest);
            return {first1, result.in, result.out};
        }

        ///////////////////////////////////////////////////////////////////////
        template <typename Result>
        struct set_union : public algorithm<set_union<Result>, Result>
        {
            constexpr set_union() noexcept
              : algorithm<set_union, Result>("set_union")
            {
            }

            template <typename ExPolicy, typename Iter1, typename Sent1,
                typename Iter2, typename Sent2, typename Iter3, typename F,
                typename Proj1, typename Proj2>
            static constexpr util::in_in_out_result<Iter1, Iter2, Iter3>
            sequential(ExPolicy, Iter1 first1, Sent1 last1, Iter2 first2,
                Sent2 last2, Iter3 dest, F&& f, Proj1&& proj1, Proj2&& proj2)
            {
                return sequential_set_union(first1, last1, first2, last2, dest,
                    HPX_FORWARD(F, f), HPX_FORWARD(Proj1, proj1),
                    HPX_FORWARD(Proj2, proj2));
            }

            template <typename ExPolicy, typename Iter1, typename Sent1,
                typename Iter2, typename Sent2, typename Iter3, typename F,
                typename Proj1, typename Proj2>
            static util::detail::algorithm_result_t<ExPolicy,
                util::in_in_out_result<Iter1, Iter2, Iter3>>
            parallel(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
                Sent2 last2, Iter3 dest, F&& f, Proj1&& proj1, Proj2&& proj2)
            {
                using difference_type1 =
                    typename std::iterator_traits<Iter1>::difference_type;
                using difference_type2 =
                    typename std::iterator_traits<Iter2>::difference_type;

                using result_type = util::in_in_out_result<Iter1, Iter2, Iter3>;

                if (first1 == last1)
                {
                    // clang-format off
                    return util::detail::convert_to_result(
                        detail::copy<util::in_out_result<Iter2, Iter3>>().call(
                            HPX_FORWARD(ExPolicy, policy), first2, last2, dest),
                        [first1](util::in_out_result<Iter2, Iter3> const& p)
                            -> result_type { return {first1, p.in, p.out}; });
                    // clang-format on
                }

                if (first2 == last2)
                {
                    // clang-format off
                    return util::detail::convert_to_result(
                        detail::copy<util::in_out_result<Iter1, Iter3>>().call(
                            HPX_FORWARD(ExPolicy, policy), first1, last1, dest),
                        [first2](util::in_out_result<Iter1, Iter3> const& p)
                            -> result_type { return {p.in, first2, p.out}; });
                    // clang-format on
                }

                using buffer_type = typename set_operations_buffer<Iter3>::type;
                using func_type = std::decay_t<F>;

                // calculate approximate destination index
                auto f1 = [](difference_type1 idx1,
                              difference_type2 idx2) -> difference_type1 {
                    return idx1 + idx2;
                };

                // perform required set operation for one chunk
                auto f2 = [proj1, proj2](Iter1 part_first1, Sent1 part_last1,
                              Iter2 part_first2, Sent2 part_last2,
                              buffer_type* d, func_type const& f) {
                    return sequential_set_union(part_first1, part_last1,
                        part_first2, part_last2, d, f, proj1, proj2);
                };

                return set_operation(HPX_FORWARD(ExPolicy, policy), first1,
                    last1, first2, last2, dest, HPX_FORWARD(F, f),
                    HPX_FORWARD(Proj1, proj1), HPX_FORWARD(Proj2, proj2),
                    HPX_MOVE(f1), HPX_MOVE(f2));
            }
        };
    }    // namespace detail
}    // namespace hpx::parallel

namespace hpx {

    ///////////////////////////////////////////////////////////////////////////
    // CPO for hpx::set_union
    inline constexpr struct set_union_t final
      : hpx::detail::tag_parallel_algorithm<set_union_t>
    {
    private:
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename FwdIter3, typename Pred = hpx::parallel::detail::less>
        // clang-format off
            requires (
                hpx::is_execution_policy_v<ExPolicy> &&
                hpx::traits::is_iterator_v<FwdIter1> &&
                hpx::traits::is_iterator_v<FwdIter2> &&
                hpx::traits::is_iterator_v<FwdIter3> &&
                hpx::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                >
            )
        // clang-format on
        friend hpx::parallel::util::detail::algorithm_result_t<ExPolicy,
            FwdIter3>
        tag_fallback_invoke(set_union_t, ExPolicy&& policy, FwdIter1 first1,
            FwdIter1 last1, FwdIter2 first2, FwdIter2 last2, FwdIter3 dest,
            Pred op = Pred())
        {
            static_assert(hpx::traits::is_forward_iterator_v<FwdIter1>,
                "Requires at least forward iterator.");
            static_assert(hpx::traits::is_forward_iterator_v<FwdIter2>,
                "Requires at least forward iterator.");
            static_assert(hpx::traits::is_forward_iterator_v<FwdIter3> ||
                    (hpx::is_sequenced_execution_policy_v<ExPolicy> &&
                        hpx::traits::is_output_iterator_v<FwdIter3>),
                "Requires at least forward iterator or sequential execution.");

            using is_seq = std::integral_constant<bool,
                hpx::is_sequenced_execution_policy_v<ExPolicy> ||
                    !hpx::traits::is_random_access_iterator_v<FwdIter1> ||
                    !hpx::traits::is_random_access_iterator_v<FwdIter2>>;

            using result_type = hpx::parallel::util::in_in_out_result<FwdIter1,
                FwdIter2, FwdIter3>;

            return hpx::parallel::util::get_third_element(
                hpx::parallel::detail::set_union<result_type>().call2(
                    HPX_FORWARD(ExPolicy, policy), is_seq(), first1, last1,
                    first2, last2, dest, HPX_MOVE(op), hpx::identity_v,
                    hpx::identity_v));
        }

        template <typename FwdIter1, typename FwdIter2, typename FwdIter3,
            typename Pred = hpx::parallel::detail::less>
        // clang-format off
            requires (
                hpx::traits::is_iterator_v<FwdIter1> &&
                hpx::traits::is_iterator_v<FwdIter2> &&
                hpx::traits::is_iterator_v<FwdIter3> &&
                hpx::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                >
            )
        // clang-format on
        friend FwdIter3 tag_fallback_invoke(set_union_t, FwdIter1 first1,
            FwdIter1 last1, FwdIter2 first2, FwdIter2 last2, FwdIter3 dest,
            Pred op = Pred())
        {
            static_assert(hpx::traits::is_input_iterator_v<FwdIter1>,
                "Requires at least input iterator.");
            static_assert(hpx::traits::is_input_iterator_v<FwdIter2>,
                "Requires at least input iterator.");
            static_assert(hpx::traits::is_output_iterator_v<FwdIter3>,
                "Requires at least output iterator.");

            using result_type = hpx::parallel::util::in_in_out_result<FwdIter1,
                FwdIter2, FwdIter3>;

            return hpx::parallel::util::get_third_element(
                hpx::parallel::detail::set_union<result_type>().call(
                    hpx::execution::seq, first1, last1, first2, last2, dest,
                    HPX_MOVE(op), hpx::identity_v, hpx::identity_v));
        }
    } set_union{};
}    // namespace hpx

#endif    // DOXYGEN

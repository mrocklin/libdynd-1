//
// Copyright (C) 2011-14 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__LIFT_REDUCTION_CKERNEL_DEFERRED_HPP_
#define _DYND__LIFT_REDUCTION_CKERNEL_DEFERRED_HPP_

#include <dynd/config.hpp>
#include <dynd/array.hpp>
#include <dynd/kernels/ckernel_deferred.hpp>

namespace dynd {

/**
 * Lifts the provided ckernel, broadcasting it as necessary to execute
 * across the additional dimensions in the ``lifted_types`` array.
 *
 * \param out_ckd  The output ckernel_deferred which is filled.
 * \param elwise_reduction  The ckernel_deferred to be lifted. This must
 *                          be a unary operation, which modifies the output
 *                          in place.
 * \param lifted_arr_type  The type the input should be lifted to.
 * \param dst_initialization  Either a NULL nd::array, or a ckernel_deferred
 *                            which initializes an accumulator value from an
 *                            input value. If it is NULL, either the value in
 *                            `reduction_identity` is used, or a copy operation
 *                            is used if that is NULL.
 * \param keepdims  If true, the output type should keep reduced dimensions as
 *                  size one, otherwise they are eliminated.
 * \param reduction_ndim  The number of dimensions being lifted. This should
 *                        be equal to the number of dimensions added in
 *                        `lifted_types` over what is in `elwise_reduction`.
 * \param reduction_dimflags  An array of length `reduction_ndim`, indicating
 *                            for each dimension whether it is being reduced.
 * \param associative  Indicate whether the operation the reduction is derived
 *                     from is associative.
 * \param commutative  Indicate whether the operation the reduction is derived
 *                     from is commutative.
 * \param right_associative  If true, the reduction associates to the right instead of
 *                           the left. This is relevant if 'associative' and/or 'commutative'
 *                           are false, and in that case forces the reduction to happen
 *                           from right to left instead of left to right.
 * \param reduction_identity  If not a NULL nd::array, this is the identity
 *                            value for the accumulator.
 */
void lift_reduction_ckernel_deferred(ckernel_deferred *out_ckd,
                const nd::array& elwise_reduction,
                const ndt::type& lifted_arr_type,
                const nd::array& dst_initialization,
                bool keepdims,
                intptr_t reduction_ndim,
                const bool *reduction_dimflags,
                bool associative,
                bool commutative,
                bool right_associative,
                const nd::array& reduction_identity);

} // namespace dynd

#endif // _DYND__LIFT_REDUCTION_CKERNEL_DEFERRED_HPP_

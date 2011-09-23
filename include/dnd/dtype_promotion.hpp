//
// Copyright (C) 2011 Mark Wiebe (mwwiebe@gmail.com)
// All rights reserved.
//
// This is unreleased proprietary software.
//
#ifndef _DND__DTYPE_PROMOTION_HPP_
#define _DND__DTYPE_PROMOTION_HPP_

#include <dnd/dtype.hpp>

namespace dnd {

/**
 * Given two dtypes, this function produces the dtype with which
 * to do arithmetic calculations for both (i.e. float + int -> float)
 *
 * For the built-in types, this is generally following the
 * rules for C/C++, with a unit test validating the results.
 */
dtype promote_dtypes_arithmetic(const dtype& dt0, const dtype& dt1);

} // namespace dnd

#endif // _DND__DTYPE_PROMOTION_HPP_

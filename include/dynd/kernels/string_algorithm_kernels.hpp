//
// Copyright (C) 2011-13 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

// Implement a number of string algorithms. Note that the initial 

#ifndef _DYND__STRING_ALGORITHM_KERNELS_HPP_
#define _DYND__STRING_ALGORITHM_KERNELS_HPP_

#include <dynd/kernels/kernel_instance.hpp>
#include <dynd/kernels/hierarchical_kernels.hpp>
#include <dynd/typed_data_assign.hpp>
#include <dynd/string_encodings.hpp>

namespace dynd { namespace kernels {

/**
 * String concatenation kernel.
 *
 * (string, string, ...) -> string
 */
struct string_concatenation_kernel {
    typedef string_concatenation_kernel extra_type;

    kernel_data_prefix m_base;
    // The number of input operands
    size_t m_nop;
    // The kernel borrows this reference from the dst metadata
    memory_block_data *m_dst_blockref;

    kernel_data_prefix& base() {
        return m_base;
    }

    /**
     * Initializes the kernel data.
     *
     * \param nop  This must be >= 2.
     * \param dst_metadata  Must be the destination for a "string" type (utf-8 string type).
     * \param src_metadata  Must be the two sources for "string" types.
     */
    void init(size_t nop, const char *dst_metadata, const char **src_metadata);

    static void single(char *dst, const char * const *src,
                kernel_data_prefix *extra);
    static void strided(char *dst, intptr_t dst_stride,
                const char * const *src, const intptr_t *src_stride,
                size_t count, kernel_data_prefix *extra);
};

/**
 * String find kernel, which searches the whole string.
 *
 * (string, string) -> intp
 */
struct string_find_kernel {
    typedef string_find_kernel extra_type;

    kernel_data_prefix m_base;
    // The string type being searched through
    const base_string_type *m_str_type;
    const char *m_str_metadata;
    // The substring type being searched for
    const base_string_type *m_sub_type;
    const char *m_sub_metadata;

    kernel_data_prefix& base() {
        return m_base;
    }

    /**
     * Initializes the kernel data.
     *
     * \param src_tp        The array of two src types.
     * \param src_metadata  The array of two src metadata.
     */
    void init(const ndt::type* src_tp, const char **src_metadata);

    static void destruct(kernel_data_prefix *extra);

    static void single(char *dst, const char * const *src,
                kernel_data_prefix *extra);
    static void strided(char *dst, intptr_t dst_stride,
                const char * const *src, const intptr_t *src_stride,
                size_t count, kernel_data_prefix *extra);
};


}} // namespace dynd::kernels

#endif // _DYND__STRING_ALGORITHM_KERNELS_HPP_

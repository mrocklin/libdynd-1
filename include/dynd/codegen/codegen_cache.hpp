//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__CODEGEN_CACHE_HPP_
#define _DYND__CODEGEN_CACHE_HPP_

#include <map>
#include <iostream>
#include <string>

#include <dynd/dtype.hpp>
#include <dynd/kernels/unary_kernel_instance.hpp>
#include <dynd/codegen/calling_conventions.hpp>

namespace dynd {

/**
 * This class owns an executable_memory_block, and provides a caching
 * interface to kernel adapters that require codegen.
 */
class codegen_cache {
    /** The memory block all the generated code goes into */
    memory_block_ptr m_exec_memblock;
    /** A mapping from unary kernel adapter unique id to the generated kernel adapter */
    std::map<uint64_t, unary_operation_t *> m_cached_unary_kernel_adapters;
    /** A mapping from binary kernel adapter unique id to the generated kernel adapter */
    std::map<uint64_t, binary_operation_t> m_cached_binary_kernel_adapters;
public:
    codegen_cache();

    /**
     * Returns the executable memory block that
     * this codegen cache generates into.
     */
    const memory_block_ptr& get_exec_memblock()
    {
        return m_exec_memblock;
    }

    /**
     * Generates the requested unary function adapter, and returns a
     * specialized unary kernel for it. Reuses the low level generated
     * adapter functions when it can.
     */
    void codegen_unary_function_adapter(const dtype& restype,
                    const dtype& arg0type, calling_convention_t callconv,
                    void *function_pointer,
                    memory_block_data *function_pointer_owner,
                    unary_specialization_kernel_instance& out_kernel);

    /**
     * Generates the requested binary function adapter, and returns a
     * binary kernel for it. Reuses the low level generated
     * adapter functions when it can.
     */
    void codegen_binary_function_adapter(const dtype& restype,
                    const dtype& arg0type, const dtype& arg1type,
                    calling_convention_t callconv,
                    void *function_pointer,
                    memory_block_data *function_pointer_owner,
                    kernel_instance<binary_operation_t>& out_kernel);

    void codegen_left_associative_binary_reduce_function_adapter(
                    const dtype& reduce_type,calling_convention_t callconv,
                    void *function_pointer,
                    memory_block_data *function_pointer_owner,
                    kernel_instance<unary_operation_t>& out_kernel);

    void codegen_right_associative_binary_reduce_function_adapter(
                    const dtype& reduce_type,calling_convention_t callconv,
                    void *function_pointer,
                    memory_block_data *function_pointer_owner,
                    kernel_instance<unary_operation_t>& out_kernel);

    void debug_dump(std::ostream& o, const std::string& indent = "") const;
};

} // namespace dynd

#endif // _DYND__CODEGEN_CACHE_HPP_
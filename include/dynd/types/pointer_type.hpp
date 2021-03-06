//
// Copyright (C) 2011-14 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

/**
 * The pointer type contains C/C++ raw pointers
 * pointing at data in other memory_blocks, using
 * blockrefs to manage the memory.
 *
 * This type operates in a "gather/scatter" fashion,
 * exposing itself as an expression type whose expression
 * copies the data to/from the pointer targets.
 */

#ifndef _DYND__POINTER_TYPE_HPP_
#define _DYND__POINTER_TYPE_HPP_

#include <dynd/type.hpp>
#include <dynd/types/void_pointer_type.hpp>

namespace dynd {

struct pointer_type_metadata {
    /**
     * A reference to the memory block which contains the data.
     */
    memory_block_data *blockref;
    /* Each pointed-to destination is offset by this amount */
    intptr_t offset;
};

class pointer_type : public base_expression_type {
    ndt::type m_target_tp;
    static ndt::type m_void_pointer_type;

public:
    pointer_type(const ndt::type& target_tp);

    virtual ~pointer_type();

    const ndt::type& get_value_type() const {
        return m_target_tp.value_type();
    }
    const ndt::type& get_operand_type() const {
        if (m_target_tp.get_type_id() == pointer_type_id) {
            return m_target_tp;
        } else {
            return m_void_pointer_type;
        }
    }

    const ndt::type& get_target_type() const {
        return m_target_tp;
    }

    void print_data(std::ostream& o, const char *metadata, const char *data) const;

    void print_type(std::ostream& o) const;

    inline bool is_type_subarray(const ndt::type& subarray_tp) const {
        // Uniform dimensions can share one implementation
        return (!subarray_tp.is_builtin() && (*this) == (*subarray_tp.extended())) ||
                        m_target_tp.is_type_subarray(subarray_tp);
    }

    bool is_expression() const;
    bool is_unique_data_owner(const char *metadata) const;
    void transform_child_types(type_transform_fn_t transform_fn, void *extra,
                    ndt::type& out_transformed_tp, bool& out_was_transformed) const;
    ndt::type get_canonical_type() const;

    ndt::type apply_linear_index(intptr_t nindices, const irange *indices,
                size_t current_i, const ndt::type& root_tp, bool leading_dimension) const;
    intptr_t apply_linear_index(intptr_t nindices, const irange *indices, const char *metadata,
                    const ndt::type& result_tp, char *out_metadata,
                    memory_block_data *embedded_reference,
                    size_t current_i, const ndt::type& root_tp,
                    bool leading_dimension, char **inout_data,
                    memory_block_data **inout_dataref) const;
    ndt::type at_single(intptr_t i0, const char **inout_metadata, const char **inout_data) const;

    ndt::type get_type_at_dimension(char **inout_metadata, intptr_t i, intptr_t total_ndim = 0) const;

        void get_shape(intptr_t ndim, intptr_t i, intptr_t *out_shape, const char *metadata, const char *data) const;

    axis_order_classification_t classify_axis_order(const char *metadata) const;

    bool is_lossless_assignment(const ndt::type& dst_tp, const ndt::type& src_tp) const;

    bool operator==(const base_type& rhs) const;

    ndt::type with_replaced_storage_type(const ndt::type& replacement_type) const;

    void metadata_default_construct(char *metadata, intptr_t ndim, const intptr_t* shape) const;
    void metadata_copy_construct(char *dst_metadata, const char *src_metadata, memory_block_data *embedded_reference) const;
    void metadata_reset_buffers(char *metadata) const;
    void metadata_finalize_buffers(char *metadata) const;
    void metadata_destruct(char *metadata) const;
    void metadata_debug_print(const char *metadata, std::ostream& o, const std::string& indent) const;

    void get_dynamic_type_properties(
                    const std::pair<std::string, gfunc::callable> **out_properties,
                    size_t *out_count) const;
};

namespace ndt {
    inline ndt::type make_pointer(const ndt::type& target_tp) {
        if (target_tp.get_type_id() != void_type_id) {
            return ndt::type(new pointer_type(target_tp), false);
        } else {
            return ndt::type(new void_pointer_type(), false);
        }
    }

    template<typename Tnative>
    ndt::type make_pointer() {
        return make_pointer(ndt::make_type<Tnative>());
    }
} // namespace ndt

} // namespace dynd

#endif // _DYND__POINTER_TYPE_HPP_

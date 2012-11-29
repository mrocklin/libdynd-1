//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//
// The bytes dtype uses memory_block references to store
// arbitrarily sized bytes.
//
#ifndef _DYND__BYTES_DTYPE_HPP_
#define _DYND__BYTES_DTYPE_HPP_

#include <dynd/dtype.hpp>
#include <dynd/dtype_assign.hpp>

namespace dynd {

struct bytes_dtype_metadata {
    /**
     * A reference to the memory block which contains the byte's data.
     * NOTE: This is identical to string_dtype_metadata, by design. Maybe
     *       both should become a typedef to a common class?
     */
    memory_block_data *blockref;
};

class bytes_dtype : public extended_dtype {
    size_t m_alignment;

public:
    bytes_dtype(size_t alignment);

    type_id_t get_type_id() const {
        return bytes_type_id;
    }
    dtype_kind_t get_kind() const {
        return bytes_kind;
    }
    // Expose the storage traits here
    size_t get_alignment() const {
        return sizeof(const char *);
    }
    size_t get_element_size() const {
        return 2 * sizeof(const char *);
    }

    /** Alignment of the bytes data being pointed to. */
    size_t get_data_alignment() const {
        return m_alignment;
    }

    void print_element(std::ostream& o, const char *data, const char *metadata) const;

    void print_dtype(std::ostream& o) const;

    // This is about the native storage, buffering code needs to check whether
    // the value_dtype is an object type separately.
    dtype_memory_management_t get_memory_management() const {
        return blockref_memory_management;
    }

    dtype apply_linear_index(int nindices, const irange *indices, int current_i, const dtype& root_dt) const;
    intptr_t apply_linear_index(int nindices, const irange *indices, char *data, const char *metadata,
                    const dtype& result_dtype, char *out_metadata, int current_i, const dtype& root_dt) const;

    dtype get_canonical_dtype() const;

    void get_shape(int i, std::vector<intptr_t>& out_shape) const;

    bool is_lossless_assignment(const dtype& dst_dt, const dtype& src_dt) const;

    void get_single_compare_kernel(single_compare_kernel_instance& out_kernel) const;

    void get_dtype_assignment_kernel(const dtype& dst_dt, const dtype& src_dt,
                    assign_error_mode errmode,
                    unary_specialization_kernel_instance& out_kernel) const;

    bool operator==(const extended_dtype& rhs) const;

    void prepare_kernel_auxdata(const char *metadata, AuxDataBase *auxdata) const;

    size_t get_metadata_size() const;
    void metadata_default_construct(char *metadata, int ndim, const intptr_t* shape) const;
    void metadata_copy_construct(char *dst_metadata, const char *src_metadata, memory_block_data *embedded_reference) const;
    void metadata_destruct(char *metadata) const;
    void metadata_debug_print(const char *metadata, std::ostream& o, const std::string& indent) const;
};

inline dtype make_bytes_dtype(size_t alignment) {
    return dtype(new bytes_dtype(alignment));
}

} // namespace dynd

#endif // _DYND__BYTES_DTYPE_HPP_
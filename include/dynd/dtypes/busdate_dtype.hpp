//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__BUSDATE_DTYPE_HPP_
#define _DYND__BUSDATE_DTYPE_HPP_

#include <dynd/dtype.hpp>
#include <dynd/dtype_assign.hpp>
#include <dynd/dtypes/view_dtype.hpp>
#include <dynd/string_encodings.hpp>
#include <dynd/ndarray.hpp>

namespace dynd {

enum busdate_roll_t {
    // Go forward in time to the following business day.
    busdate_roll_following,
    // Go backward in time to the preceding business day.
    busdate_roll_preceding,
    // Go forward in time to the following business day, unless it
    // crosses a month boundary, in which case go backward
    busdate_roll_modifiedfollowing,
    // Go backward in time to the preceding business day, unless it
    // crosses a month boundary, in which case go forward.
    busdate_roll_modifiedpreceding,
    // Produce a NaT for non-business days.
    busdate_roll_nat,
    // Raise an exception for non-business days.
    busdate_roll_throw
};

class busdate_dtype : public extended_dtype {
    /** Strategy for handling dates that are not business dates */
    busdate_roll_t m_roll;
    /** Which days of the week are work days vs weekend */
    bool m_workweek[7];
    /** Cache of the non-weekend day count in the weekmask */
    int m_busdays_in_weekmask;
    /**
     * If non-NULL, a one-dimensional contiguous array of day unit date_dtype
     * which has no duplicates or holidays falling on a weekend.
     */
    ndarray m_holidays;

public:
    busdate_dtype(busdate_roll_t roll, const bool *weekmask, const ndarray& holidays);

    type_id_t type_id() const {
        return busdate_type_id;
    }
    dtype_kind_t kind() const {
        return datetime_kind;
    }
    // Expose the storage traits here
    size_t alignment() const {
        return 4;
    }
    size_t get_element_size() const {
        return 4;
    }

    busdate_roll_t get_roll() const {
        return m_roll;
    }

    const bool *get_weekmask() const {
        return m_workweek;
    }

    ndarray get_holidays() const {
        return m_holidays;
    }

    bool is_default_workweek() const {
        return m_workweek[0] && m_workweek[1] && m_workweek[2] && m_workweek[3] &&
                m_workweek[4] && !m_workweek[5] && !m_workweek[6];
    }

    void print_workweek(std::ostream& o) const;
    void print_holidays(std::ostream& o) const;

    void print_element(std::ostream& o, const char *data, const char *metadata) const;

    void print_dtype(std::ostream& o) const;

    // This is about the native storage, buffering code needs to check whether
    // the value_dtype is an object type separately.
    dtype_memory_management_t get_memory_management() const {
        return pod_memory_management;
    }

    bool is_lossless_assignment(const dtype& dst_dt, const dtype& src_dt) const;

    void get_single_compare_kernel(single_compare_kernel_instance& out_kernel) const;

    void get_dtype_assignment_kernel(const dtype& dst_dt, const dtype& src_dt,
                    assign_error_mode errmode,
                    unary_specialization_kernel_instance& out_kernel) const;

    bool operator==(const extended_dtype& rhs) const;

    size_t get_metadata_size() const {
        return 0;
    }
    void metadata_destruct(char *DYND_UNUSED(metadata)) const {
    }
    void metadata_debug_dump(const char *DYND_UNUSED(metadata), std::ostream& DYND_UNUSED(o), const std::string& DYND_UNUSED(indent)) const {
    }
};

inline dtype make_busdate_dtype(busdate_roll_t roll = busdate_roll_following, const bool *weekmask = NULL, const ndarray& holidays = ndarray()) {
    return dtype(new busdate_dtype(roll, weekmask, holidays));
}

} // namespace dynd

#endif // _DYND__BUSDATE_DTYPE_HPP_
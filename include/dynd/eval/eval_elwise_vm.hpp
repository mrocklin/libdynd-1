//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__EVAL_ELWISE_VM_HPP_
#define _DYND__EVAL_ELWISE_VM_HPP_

#include <dynd/config.hpp>
#include <dynd/ndarray.hpp>
#include <dynd/memblock/memory_block.hpp>
#include <dynd/vm/elwise_program.hpp>
#include <dynd/eval/eval_context.hpp>

namespace dynd { namespace eval {

ndarray evaluate_elwise_vm(const vm::elwise_program& ep, std::vector<ndarray> inputs,
                    const eval::eval_context *ectx = &eval::default_eval_context);

}} // namespace dynd::eval

#endif // _DYND__EVAL_ELWISE_VM_HPP_
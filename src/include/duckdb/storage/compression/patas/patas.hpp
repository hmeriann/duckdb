//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/storage/compression/patas/patas.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/storage/compression/patas/algorithm/patas.hpp"
#include "duckdb/common/assert.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/limits.hpp"
#include "duckdb/function/compression_function.hpp"

namespace duckdb {

using byte_index_t = uint32_t;

//! FIXME: replace ChimpType with this
template <class T>
struct FloatingToExact {};

template <>
struct FloatingToExact<double> {
	using TYPE = uint64_t;
};

template <>
struct FloatingToExact<float> {
	using TYPE = uint32_t;
};

template <class T, bool EMPTY>
struct PatasState {
public:
	using EXACT_TYPE = typename FloatingToExact<T>::TYPE;

	explicit PatasState(void *state_p = nullptr) : data_ptr(state_p), patas_state() {
	}
	//! The Compress/Analyze State
	void *data_ptr;
	patas::PatasCompressionState<EXACT_TYPE, EMPTY> patas_state;

public:
	void AssignDataBuffer(uint8_t *data_out) {
		patas_state.SetOutputBuffer(data_out);
	}

	template <class OP>
	bool Update(T uncompressed_value, bool is_valid) {
		OP::template Operation<T>(uncompressed_value, is_valid, data_ptr);
		return true;
	}
};

} // namespace duckdb

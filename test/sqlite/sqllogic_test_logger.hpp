//===----------------------------------------------------------------------===//
//                         DuckDB
//
// sqllogic_test_logger.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#include "duckdb/common/mutex.hpp"
#include "sqllogic_command.hpp"

namespace duckdb {

class Command;
class LoopCommand;

class SQLLogicTestLogger {
public:
	SQLLogicTestLogger(ExecuteContext &context, const Command &command);
	~SQLLogicTestLogger();

	static string Log(const string &str);
	string PrintExpectedResult(const vector<string> &values, idx_t columns, bool row_wise);
	static string PrintLineSep();
	static string PrintHeader(string header);
	string PrintFileHeader();
	string PrintSQL();
	void PrintSQLFormatted();
	string PrintErrorHeader(const string &description);
	static string PrintErrorHeader(const string &file_name, idx_t query_line, const string &description);
	string PrintResultError(const vector<string> &result_values, const vector<string> &values,
	                        idx_t expected_column_count, bool row_wise);
	static string PrintSummaryHeader(const std::string &file_name);
	string PrintResultError(MaterializedQueryResult &result, const vector<string> &values, idx_t expected_column_count,
	                        bool row_wise);
	string PrintResultString(MaterializedQueryResult &result);
	void UnexpectedFailure(MaterializedQueryResult &result);
	void OutputResult(MaterializedQueryResult &result, const vector<string> &result_values_string);
	void OutputHash(const string &hash_value);
	void ColumnCountMismatch(MaterializedQueryResult &result, const vector<string> &result_values_string,
	                         idx_t expected_column_count, bool row_wise);
	void NotCleanlyDivisible(idx_t expected_column_count, idx_t actual_column_count);
	void WrongRowCount(idx_t expected_rows, MaterializedQueryResult &result, const vector<string> &comparison_values,
	                   idx_t expected_column_count, bool row_wise);
	void ColumnCountMismatchCorrectResult(idx_t original_expected_columns, idx_t expected_column_count,
	                                      MaterializedQueryResult &result);
	void SplitMismatch(idx_t row_number, idx_t expected_column_count, idx_t split_count);
	void WrongResultHash(QueryResult *expected_result, MaterializedQueryResult &result);
	void UnexpectedStatement(bool expect_ok, MaterializedQueryResult &result);
	void ExpectedErrorMismatch(const string &expected_error, MaterializedQueryResult &result);
	void InternalException(MaterializedQueryResult &result);
	static void LoadDatabaseFail(const string &dbpath, const string &message);

	static void SafeAppend(const string &log_message);
	static void LogBoth(const string &log_message);
	static string GetFailureSummary();

private:
	lock_guard<mutex> log_lock;
	string file_name;
	int query_line;
	string sql_query;
};
} // namespace duckdb

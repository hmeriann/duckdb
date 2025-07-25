#include "duckdb/common/string_util.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/pair.hpp"
#include "duckdb/common/stack.hpp"
#include "duckdb/common/to_string.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/exception/parser_exception.hpp"
#include "duckdb/common/random_engine.hpp"
#include "duckdb/original/std/sstream.hpp"
#include "jaro_winkler.hpp"
#include "utf8proc_wrapper.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <memory>
#include <stdarg.h>
#include <string.h>
#include <stack>

#include "yyjson.hpp"

using namespace duckdb_yyjson; // NOLINT

namespace duckdb {

string StringUtil::GenerateRandomName(idx_t length) {
	RandomEngine engine;
	duckdb::stringstream ss;
	for (idx_t i = 0; i < length; i++) {
		ss << "0123456789abcdef"[engine.NextRandomInteger(0, 15)];
	}
	return ss.str();
}

bool StringUtil::Contains(const string &haystack, const string &needle) {
	return Find(haystack, needle).IsValid();
}

optional_idx StringUtil::Find(const string &haystack, const string &needle) {
	auto index = haystack.find(needle);
	if (index == string::npos) {
		return optional_idx();
	}
	return optional_idx(index);
}

bool StringUtil::Contains(const string &haystack, const char &needle_char) {
	return (haystack.find(needle_char) != string::npos);
}

idx_t StringUtil::ToUnsigned(const string &str) {
	return std::stoull(str);
}

void StringUtil::LTrim(string &str) {
	auto it = str.begin();
	while (it != str.end() && CharacterIsSpace(*it)) {
		it++;
	}
	str.erase(str.begin(), it);
}

// Remove trailing ' ', '\f', '\n', '\r', '\t', '\v'
void StringUtil::RTrim(string &str) {
	str.erase(find_if(str.rbegin(), str.rend(), [](char ch) { return ch > 0 && !CharacterIsSpace(ch); }).base(),
	          str.end());
}

void StringUtil::RTrim(string &str, const string &chars_to_trim) {
	str.erase(find_if(str.rbegin(), str.rend(),
	                  [&chars_to_trim](char ch) { return ch > 0 && chars_to_trim.find(ch) == string::npos; })
	              .base(),
	          str.end());
}

void StringUtil::Trim(string &str) {
	StringUtil::LTrim(str);
	StringUtil::RTrim(str);
}

bool StringUtil::StartsWith(string str, string prefix) {
	if (prefix.size() > str.size()) {
		return false;
	}
	return equal(prefix.begin(), prefix.end(), str.begin());
}

bool StringUtil::EndsWith(const string &str, const string &suffix) {
	if (suffix.size() > str.size()) {
		return false;
	}
	return equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

string StringUtil::Repeat(const string &str, idx_t n) {
	std::ostringstream os;
	for (idx_t i = 0; i < n; i++) {
		os << str;
	}
	return (os.str());
}

namespace string_util_internal {

inline void SkipSpaces(const string &str, idx_t &index) {
	while (index < str.size() && StringUtil::CharacterIsSpace(str[index])) {
		index++;
	}
}

inline void ConsumeLetter(const string &str, idx_t &index, char expected) {
	if (index >= str.size() || str[index] != expected) {
		throw ParserException("Invalid quoted list: %s", str);
	}

	index++;
}

template <typename F>
inline void TakeWhile(const string &str, idx_t &index, const F &cond, string &taker) {
	while (index < str.size() && cond(str[index])) {
		taker.push_back(str[index]);
		index++;
	}
}

inline string TakePossiblyQuotedItem(const string &str, idx_t &index, char delimiter, char quote) {
	string entry;

	if (str[index] == quote) {
		index++;
		TakeWhile(
		    str, index, [quote](char c) { return c != quote; }, entry);
		ConsumeLetter(str, index, quote);
	} else {
		TakeWhile(
		    str, index,
		    [delimiter, quote](char c) { return c != delimiter && c != quote && !StringUtil::CharacterIsSpace(c); },
		    entry);
	}

	return entry;
}

} // namespace string_util_internal

vector<string> StringUtil::SplitWithQuote(const string &str, char delimiter, char quote) {
	vector<string> entries;
	idx_t i = 0;

	string_util_internal::SkipSpaces(str, i);
	while (i < str.size()) {
		if (!entries.empty()) {
			string_util_internal::ConsumeLetter(str, i, delimiter);
		}

		entries.emplace_back(string_util_internal::TakePossiblyQuotedItem(str, i, delimiter, quote));
		string_util_internal::SkipSpaces(str, i);
	}

	return entries;
}

vector<string> StringUtil::SplitWithParentheses(const string &str, char delimiter, char par_open, char par_close) {
	vector<string> result;
	string current;
	stack<char> parentheses;

	for (size_t i = 0; i < str.size(); ++i) {
		char ch = str[i];

		// stack to keep track if we are within parentheses
		if (ch == par_open) {
			parentheses.push(ch);
		}
		if (ch == par_close) {
			if (!parentheses.empty()) {
				parentheses.pop();
			} else {
				throw InvalidInputException("Incongruent parentheses in string: '%s'", str);
			}
		}
		// split if not within parentheses
		if (parentheses.empty() && ch == delimiter) {
			result.push_back(current);
			current.clear();
		} else {
			current += ch;
		}
	}
	// Add the last segment
	if (!current.empty()) {
		result.push_back(current);
	}
	if (!parentheses.empty()) {
		throw InvalidInputException("Incongruent parentheses in string: '%s'", str);
	}
	return result;
}

string StringUtil::Join(const vector<string> &input, const string &separator) {
	return StringUtil::Join(input, input.size(), separator, [](const string &s) { return s; });
}

string StringUtil::Join(const set<string> &input, const string &separator) {
	// The result
	std::string result;

	auto it = input.begin();
	while (it != input.end()) {
		result += *it;
		it++;
		if (it == input.end()) {
			break;
		}
		result += separator;
	}
	return result;
}

string StringUtil::BytesToHumanReadableString(idx_t bytes, idx_t multiplier) {
	D_ASSERT(multiplier == 1000 || multiplier == 1024);
	idx_t array[6] = {};
	const char *unit[2][6] = {{"bytes", "KiB", "MiB", "GiB", "TiB", "PiB"}, {"bytes", "kB", "MB", "GB", "TB", "PB"}};

	const int sel = (multiplier == 1000);

	array[0] = bytes;
	for (idx_t i = 1; i < 6; i++) {
		array[i] = array[i - 1] / multiplier;
		array[i - 1] %= multiplier;
	}

	for (idx_t i = 5; i >= 1; i--) {
		if (array[i]) {
			// Map 0 -> 0 and (multiplier-1) -> 9
			idx_t fractional_part = (array[i - 1] * 10) / multiplier;
			return to_string(array[i]) + "." + to_string(fractional_part) + " " + unit[sel][i];
		}
	}

	return to_string(array[0]) + (bytes == 1 ? " byte" : " bytes");
}

string StringUtil::Upper(const string &str) {
	string copy(str);
	transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c) { return std::toupper(c); });
	return (copy);
}

string StringUtil::Lower(const string &str) {
	string copy(str);
	transform(copy.begin(), copy.end(), copy.begin(),
	          [](unsigned char c) { return StringUtil::CharacterToLower(static_cast<char>(c)); });
	return (copy);
}

string StringUtil::Title(const string &str) {
	string copy;
	bool first_character = true;
	for (auto c : str) {
		bool is_alpha = StringUtil::CharacterIsAlpha(c);
		if (is_alpha) {
			if (first_character) {
				copy += StringUtil::CharacterToUpper(c);
				first_character = false;
			} else {
				copy += StringUtil::CharacterToLower(c);
			}
		} else {
			first_character = true;
			copy += c;
		}
	}
	return copy;
}

bool StringUtil::IsLower(const string &str) {
	return str == Lower(str);
}

bool StringUtil::IsUpper(const string &str) {
	return str == Upper(str);
}

// Jenkins hash function: https://en.wikipedia.org/wiki/Jenkins_hash_function
uint64_t StringUtil::CIHash(const string &str) {
	uint32_t hash = 0;
	for (auto c : str) {
		hash += static_cast<uint32_t>(StringUtil::CharacterToLower(static_cast<char>(c)));
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

bool StringUtil::CIEquals(const char *l1, idx_t l1_size, const char *l2, idx_t l2_size) {
	if (l1_size != l2_size) {
		return false;
	}
	const auto charmap = ASCII_TO_LOWER_MAP;
	for (idx_t c = 0; c < l1_size; c++) {
		if (charmap[(uint8_t)l1[c]] != charmap[(uint8_t)l2[c]]) {
			return false;
		}
	}
	return true;
}

bool StringUtil::CIEquals(const string &l1, const string &l2) {
	return CIEquals(l1.c_str(), l1.size(), l2.c_str(), l2.size());
}

bool StringUtil::CILessThan(const string &s1, const string &s2) {
	const auto charmap = ASCII_TO_UPPER_MAP;

	unsigned char u1 {}, u2 {};

	idx_t length = MinValue<idx_t>(s1.length(), s2.length());
	length += s1.length() != s2.length();
	for (idx_t i = 0; i < length; i++) {
		u1 = (unsigned char)s1[i];
		u2 = (unsigned char)s2[i];
		if (charmap[u1] != charmap[u2]) {
			break;
		}
	}
	return (charmap[u1] - charmap[u2]) < 0;
}

idx_t StringUtil::CIFind(vector<string> &vector, const string &search_string) {
	for (idx_t i = 0; i < vector.size(); i++) {
		const auto &string = vector[i];
		if (CIEquals(string, search_string)) {
			return i;
		}
	}
	return DConstants::INVALID_INDEX;
}

vector<string> StringUtil::Split(const string &str, char delimiter) {
	duckdb::stringstream ss(str);
	vector<string> lines;
	string temp;
	while (getline(ss, temp, delimiter)) {
		lines.push_back(temp);
	}
	return (lines);
}

vector<string> StringUtil::Split(const string &input, const string &split) {
	vector<string> splits;

	idx_t last = 0;
	idx_t input_len = input.size();
	idx_t split_len = split.size();
	while (last <= input_len) {
		idx_t next = input.find(split, last);
		if (next == string::npos) {
			next = input_len;
		}

		// Push the substring [last, next) on to splits
		string substr = input.substr(last, next - last);
		if (!substr.empty()) {
			splits.push_back(substr);
		}
		last = next + split_len;
	}
	if (splits.empty()) {
		splits.push_back(input);
	}
	return splits;
}

string StringUtil::Replace(string source, const string &from, const string &to) {
	if (from.empty()) {
		throw InternalException("Invalid argument to StringUtil::Replace - empty FROM");
	}
	idx_t start_pos = 0;
	while ((start_pos = source.find(from, start_pos)) != string::npos) {
		source.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like
		                          // replacing 'x' with 'yx'
	}
	return source;
}

vector<string> StringUtil::TopNStrings(vector<pair<string, double>> scores, idx_t n, double threshold) {
	if (scores.empty()) {
		return vector<string>();
	}
	sort(scores.begin(), scores.end(), [](const pair<string, double> &a, const pair<string, double> &b) -> bool {
		return a.second > b.second || (a.second == b.second && a.first.size() < b.first.size());
	});
	vector<string> result;
	result.push_back(scores[0].first);
	for (idx_t i = 1; i < MinValue<idx_t>(scores.size(), n); i++) {
		if (scores[i].second < threshold) {
			break;
		}
		result.push_back(scores[i].first);
	}
	return result;
}

static double NormalizeScore(idx_t score, idx_t max_score) {
	return 1.0 - static_cast<double>(score) / static_cast<double>(max_score);
}

vector<string> StringUtil::TopNStrings(const vector<pair<string, idx_t>> &scores, idx_t n, idx_t threshold) {
	// obtain the max score to normalize
	idx_t max_score = threshold;
	for (auto &score : scores) {
		if (score.second > max_score) {
			max_score = score.second;
		}
	}

	// normalize
	vector<pair<string, double>> normalized_scores;
	for (auto &score : scores) {
		normalized_scores.push_back(make_pair(score.first, NormalizeScore(score.second, max_score)));
	}
	return TopNStrings(std::move(normalized_scores), n, NormalizeScore(threshold, max_score));
}

struct LevenshteinArray {
	LevenshteinArray(idx_t len1, idx_t len2) : len1(len1) {
		dist = make_unsafe_uniq_array<idx_t>(len1 * len2);
	}

	idx_t &Score(idx_t i, idx_t j) {
		return dist[GetIndex(i, j)];
	}

private:
	idx_t len1;
	unsafe_unique_array<idx_t> dist;

	idx_t GetIndex(idx_t i, idx_t j) {
		return j * len1 + i;
	}
};

// adapted from https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
idx_t StringUtil::LevenshteinDistance(const string &s1_p, const string &s2_p, idx_t not_equal_penalty) {
	auto s1 = StringUtil::Lower(s1_p);
	auto s2 = StringUtil::Lower(s2_p);
	idx_t len1 = s1.size();
	idx_t len2 = s2.size();
	if (len1 == 0) {
		return len2;
	}
	if (len2 == 0) {
		return len1;
	}
	LevenshteinArray array(len1 + 1, len2 + 1);
	array.Score(0, 0) = 0;
	for (idx_t i = 0; i <= len1; i++) {
		array.Score(i, 0) = i;
	}
	for (idx_t j = 0; j <= len2; j++) {
		array.Score(0, j) = j;
	}
	for (idx_t i = 1; i <= len1; i++) {
		for (idx_t j = 1; j <= len2; j++) {
			// d[i][j] = std::min({ d[i - 1][j] + 1,
			//                      d[i][j - 1] + 1,
			//                      d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
			auto equal = s1[i - 1] == s2[j - 1] ? 0 : not_equal_penalty;
			idx_t adjacent_score1 = array.Score(i - 1, j) + 1;
			idx_t adjacent_score2 = array.Score(i, j - 1) + 1;
			idx_t adjacent_score3 = array.Score(i - 1, j - 1) + equal;

			idx_t t = MinValue<idx_t>(adjacent_score1, adjacent_score2);
			array.Score(i, j) = MinValue<idx_t>(t, adjacent_score3);
		}
	}
	return array.Score(len1, len2);
}

idx_t StringUtil::SimilarityScore(const string &s1, const string &s2) {
	return LevenshteinDistance(s1, s2, 3);
}

double StringUtil::SimilarityRating(const string &s1, const string &s2) {
	return duckdb_jaro_winkler::jaro_winkler_similarity(s1.data(), s1.data() + s1.size(), s2.data(),
	                                                    s2.data() + s2.size());
}

vector<string> StringUtil::TopNLevenshtein(const vector<string> &strings, const string &target, idx_t n,
                                           idx_t threshold) {
	vector<pair<string, idx_t>> scores;
	scores.reserve(strings.size());
	for (auto &str : strings) {
		if (target.size() < str.size()) {
			scores.emplace_back(str, SimilarityScore(str.substr(0, target.size()), target));
		} else {
			scores.emplace_back(str, SimilarityScore(str, target));
		}
	}
	return TopNStrings(scores, n, threshold);
}

vector<string> StringUtil::TopNJaroWinkler(const vector<string> &strings, const string &target, idx_t n,
                                           double threshold) {
	vector<pair<string, double>> scores;
	scores.reserve(strings.size());
	for (auto &str : strings) {
		scores.emplace_back(str, SimilarityRating(str, target));
	}
	return TopNStrings(scores, n, threshold);
}

string StringUtil::CandidatesMessage(const vector<string> &candidates, const string &candidate) {
	string result_str;
	if (!candidates.empty()) {
		result_str = "\n" + candidate + ": ";
		for (idx_t i = 0; i < candidates.size(); i++) {
			if (i > 0) {
				result_str += ", ";
			}
			result_str += "\"" + candidates[i] + "\"";
		}
	}
	return result_str;
}

string StringUtil::CandidatesErrorMessage(const vector<string> &strings, const string &target,
                                          const string &message_prefix, idx_t n) {
	auto closest_strings = StringUtil::TopNLevenshtein(strings, target, n);
	return StringUtil::CandidatesMessage(closest_strings, message_prefix);
}

static unique_ptr<ComplexJSON> ParseJSON(const string &json, yyjson_doc *doc, yyjson_val *root, bool ignore_errors) {
	auto result = make_uniq<ComplexJSON>();
	switch (yyjson_get_tag(root)) {
	case YYJSON_TYPE_ARR | YYJSON_SUBTYPE_NONE: {
		size_t idx, max;
		yyjson_val *val;
		yyjson_arr_foreach(root, idx, max, val) {
			result->AddArrayElement(ParseJSON(json, doc, val, ignore_errors));
		}
		return result;
	}
	case YYJSON_TYPE_OBJ | YYJSON_SUBTYPE_NONE: {
		size_t idx, max;
		yyjson_val *key, *value;
		yyjson_obj_foreach(root, idx, max, key, value) {
			const auto key_val = yyjson_get_str(key);
			const auto key_len = yyjson_get_len(key);
			result->AddObjectEntry(string(key_val, key_len), ParseJSON(json, doc, value, ignore_errors));
		}
		return result;
	}
	case YYJSON_TYPE_STR | YYJSON_SUBTYPE_NOESC:
	case YYJSON_TYPE_STR | YYJSON_SUBTYPE_NONE: {
		// Since this is a string, we can directly add the value
		const auto value_val = yyjson_get_str(root);
		const auto value_len = yyjson_get_len(root);
		return make_uniq<ComplexJSON>(string(value_val, value_len));
	}
	case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_TRUE:
	case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_FALSE: {
		// boolean values
		const bool bool_val = yyjson_get_bool(root);
		return make_uniq<ComplexJSON>(bool_val ? "true" : "false");
	}
	default:
		yyjson_doc_free(doc);
		throw SerializationException("Failed to parse JSON string: %s", json);
	}
}

unique_ptr<ComplexJSON> StringUtil::ParseJSONMap(const string &json, bool ignore_errors) {
	auto result = make_uniq<ComplexJSON>(json);
	if (json.empty()) {
		return result;
	}
	yyjson_read_flag flags = YYJSON_READ_ALLOW_INVALID_UNICODE;
	yyjson_doc *doc = yyjson_read(json.c_str(), json.size(), flags);
	if (!doc) {
		if (ignore_errors) {
			return result;
		}
		throw SerializationException("Failed to parse JSON string: %s", json);
	}
	yyjson_val *root = yyjson_doc_get_root(doc);
	if (!root || yyjson_get_type(root) != YYJSON_TYPE_OBJ) {
		yyjson_doc_free(doc);
		if (ignore_errors) {
			return result;
		}
		throw SerializationException("Failed to parse JSON string: %s", json);
	}

	result = ParseJSON(json, doc, root, ignore_errors);
	yyjson_doc_free(doc);
	return result;
}

string WriteJsonToString(yyjson_mut_doc *doc) {
	yyjson_write_err err;
	size_t len;
	constexpr yyjson_write_flag flags = YYJSON_WRITE_ALLOW_INVALID_UNICODE;
	char *json = yyjson_mut_write_opts(doc, flags, nullptr, &len, &err);
	if (!json) {
		yyjson_mut_doc_free(doc);
		throw SerializationException("Failed to write JSON string: %s", err.msg);
	}
	// Create a string from the JSON
	string result(json, len);

	// Free the JSON and the document
	free(json);
	yyjson_mut_doc_free(doc);

	// Return the result
	return result;
}

string ToJsonMapInternal(const unordered_map<string, string> &map, yyjson_mut_doc *doc, yyjson_mut_val *root) {
	for (auto &entry : map) {
		auto key = yyjson_mut_strncpy(doc, entry.first.c_str(), entry.first.size());
		auto value = yyjson_mut_strncpy(doc, entry.second.c_str(), entry.second.size());
		yyjson_mut_obj_add(root, key, value);
	}
	return WriteJsonToString(doc);
}
string StringUtil::ToJSONMap(const unordered_map<string, string> &map) {
	yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
	yyjson_mut_val *root = yyjson_mut_obj(doc);
	yyjson_mut_doc_set_root(doc, root);

	return ToJsonMapInternal(map, doc, root);
}

string ComplexJSON::GetValue(const string &key) const {
	if (type == ComplexJSONType::OBJECT) {
		if (obj_value.find(key) != obj_value.end()) {
			return GetValueRecursive(*obj_value.at(key));
		}
	}
	// Object either doesn't exist or this is just a string
	return "";
}

string ComplexJSON::GetValue(const idx_t &index) const {
	if (type == ComplexJSONType::ARRAY) {
		if (index >= arr_value.size()) {
			return "";
		}
		return GetValueRecursive(*arr_value[index]);
	}
	return "";
}

string ComplexJSON::GetValueRecursive(const ComplexJSON &child) {
	if (child.type == ComplexJSONType::OBJECT) {
		// We have to construct the nested json
		yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
		yyjson_mut_val *root = yyjson_mut_obj(doc);
		yyjson_mut_doc_set_root(doc, root);
		for (const auto &object : child.obj_value) {
			auto key = yyjson_mut_strncpy(doc, object.first.c_str(), object.first.size());
			auto value_str = GetValueRecursive(*object.second);
			auto value = yyjson_mut_strncpy(doc, value_str.c_str(), value_str.size());
			yyjson_mut_obj_add(root, key, value);
		}
		return WriteJsonToString(doc);
	} else if (child.type == ComplexJSONType::ARRAY) {
		yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
		yyjson_mut_val *root = yyjson_mut_arr(doc);
		yyjson_mut_doc_set_root(doc, root);
		for (const auto &elem : child.arr_value) {
			auto value_str = GetValueRecursive(*elem);
			auto value = yyjson_mut_strncpy(doc, value_str.c_str(), value_str.size());
			yyjson_mut_arr_append(root, value);
		}
		return WriteJsonToString(doc);
	} else {
		// simple string we can just write
		return child.str_value;
	}
}
string StringUtil::ToComplexJSONMap(const ComplexJSON &complex_json) {
	return ComplexJSON::GetValueRecursive(complex_json);
}

string StringUtil::ExceptionToJSONMap(ExceptionType type, const string &message,
                                      const unordered_map<string, string> &map) {
	D_ASSERT(map.find("exception_type") == map.end());
	D_ASSERT(map.find("exception_message") == map.end());

	yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
	yyjson_mut_val *root = yyjson_mut_obj(doc);
	yyjson_mut_doc_set_root(doc, root);

	auto except_str = Exception::ExceptionTypeToString(type);
	yyjson_mut_obj_add_strncpy(doc, root, "exception_type", except_str.c_str(), except_str.size());
	yyjson_mut_obj_add_strncpy(doc, root, "exception_message", message.c_str(), message.size());

	return ToJsonMapInternal(map, doc, root);
}

string StringUtil::GetFileName(const string &file_path) {

	idx_t pos = file_path.find_last_of("/\\");
	if (pos == string::npos) {
		return file_path;
	}
	auto end = file_path.size() - 1;

	// If the rest of the string is just slashes or dots, trim them
	if (file_path.find_first_not_of("/\\.", pos) == string::npos) {
		// Trim the trailing slashes and dots
		while (end > 0 && (file_path[end] == '/' || file_path[end] == '.' || file_path[end] == '\\')) {
			end--;
		}

		// Now find the next slash
		pos = file_path.find_last_of("/\\", end);
		if (pos == string::npos) {
			return file_path.substr(0, end + 1);
		}
	}

	return file_path.substr(pos + 1, end - pos);
}

string StringUtil::GetFileExtension(const string &file_name) {
	auto name = GetFileName(file_name);
	idx_t pos = name.find_last_of('.');
	// We dont consider e.g. `.gitignore` to have an extension
	if (pos == string::npos || pos == 0) {
		return "";
	}
	return name.substr(pos + 1);
}

string StringUtil::GetFileStem(const string &file_name) {
	auto name = GetFileName(file_name);
	if (name.size() > 1 && name[0] == '.') {
		return name;
	}
	idx_t pos = name.find_last_of('.');
	if (pos == string::npos) {
		return name;
	}
	return name.substr(0, pos);
}

string StringUtil::GetFilePath(const string &file_path) {
	// Trim the trailing slashes
	auto end = file_path.size() - 1;
	while (end > 0 && (file_path[end] == '/' || file_path[end] == '\\')) {
		end--;
	}

	auto pos = file_path.find_last_of("/\\", end);
	if (pos == string::npos) {
		return "";
	}

	while (pos > 0 && (file_path[pos] == '/' || file_path[pos] == '\\')) {
		pos--;
	}

	return file_path.substr(0, pos + 1);
}

struct URLEncodeLength {
	using RESULT_TYPE = idx_t;

	static void ProcessCharacter(idx_t &result, char) {
		result++;
	}

	static void ProcessHex(idx_t &result, const char *, idx_t) {
		result++;
	}
};

struct URLEncodeWrite {
	using RESULT_TYPE = char *;

	static void ProcessCharacter(char *&result, char c) {
		*result = c;
		result++;
	}

	static void ProcessHex(char *&result, const char *input, idx_t idx) {
		uint32_t hex_first = StringUtil::GetHexValue(input[idx + 1]);
		uint32_t hex_second = StringUtil::GetHexValue(input[idx + 2]);
		uint32_t hex_value = (hex_first << 4) + hex_second;
		ProcessCharacter(result, static_cast<char>(hex_value));
	}
};

template <class OP>
void URLEncodeInternal(const char *input, idx_t input_size, typename OP::RESULT_TYPE &result, bool encode_slash) {
	// https://docs.aws.amazon.com/AmazonS3/latest/API/sigv4-query-string-auth.html
	static const char *HEX_DIGIT = "0123456789ABCDEF";
	for (idx_t i = 0; i < input_size; i++) {
		char ch = input[i];
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_' ||
		    ch == '-' || ch == '~' || ch == '.') {
			OP::ProcessCharacter(result, ch);
		} else if (ch == '/' && !encode_slash) {
			OP::ProcessCharacter(result, ch);
		} else {
			OP::ProcessCharacter(result, '%');
			OP::ProcessCharacter(result, HEX_DIGIT[static_cast<unsigned char>(ch) >> 4]);
			OP::ProcessCharacter(result, HEX_DIGIT[static_cast<unsigned char>(ch) & 15]);
		}
	}
}

idx_t StringUtil::URLEncodeSize(const char *input, idx_t input_size, bool encode_slash) {
	idx_t result_length = 0;
	URLEncodeInternal<URLEncodeLength>(input, input_size, result_length, encode_slash);
	return result_length;
}

void StringUtil::URLEncodeBuffer(const char *input, idx_t input_size, char *output, bool encode_slash) {
	URLEncodeInternal<URLEncodeWrite>(input, input_size, output, encode_slash);
}

string StringUtil::URLEncode(const string &input, bool encode_slash) {
	idx_t result_size = URLEncodeSize(input.c_str(), input.size(), encode_slash);
	auto result_data = make_uniq_array<char>(result_size);
	URLEncodeBuffer(input.c_str(), input.size(), result_data.get(), encode_slash);
	return string(result_data.get(), result_size);
}

template <class OP>
void URLDecodeInternal(const char *input, idx_t input_size, typename OP::RESULT_TYPE &result, bool plus_to_space) {
	for (idx_t i = 0; i < input_size; i++) {
		char ch = input[i];
		if (plus_to_space && ch == '+') {
			OP::ProcessCharacter(result, ' ');
		} else if (ch == '%' && i + 2 < input_size && StringUtil::CharacterIsHex(input[i + 1]) &&
		           StringUtil::CharacterIsHex(input[i + 2])) {
			OP::ProcessHex(result, input, i);
			i += 2;
		} else {
			OP::ProcessCharacter(result, ch);
		}
	}
}

idx_t StringUtil::URLDecodeSize(const char *input, idx_t input_size, bool plus_to_space) {
	idx_t result_length = 0;
	URLDecodeInternal<URLEncodeLength>(input, input_size, result_length, plus_to_space);
	return result_length;
}

void StringUtil::URLDecodeBuffer(const char *input, idx_t input_size, char *output, bool plus_to_space) {
	char *output_start = output;
	URLDecodeInternal<URLEncodeWrite>(input, input_size, output, plus_to_space);
	if (!Utf8Proc::IsValid(output_start, NumericCast<idx_t>(output - output_start))) {
		throw InvalidInputException("Failed to decode string \"%s\" using URL decoding - decoded value is invalid UTF8",
		                            string(input, input_size));
	}
}

void StringUtil::SkipBOM(const char *buffer_ptr, const idx_t &buffer_size, idx_t &buffer_pos) {
	if (buffer_size >= 3 && buffer_ptr[0] == '\xEF' && buffer_ptr[1] == '\xBB' && buffer_ptr[2] == '\xBF' &&
	    buffer_pos == 0) {
		buffer_pos = 3;
	}
}

string StringUtil::URLDecode(const string &input, bool plus_to_space) {
	idx_t result_size = URLDecodeSize(input.c_str(), input.size(), plus_to_space);
	auto result_data = make_uniq_array<char>(result_size);
	URLDecodeBuffer(input.c_str(), input.size(), result_data.get(), plus_to_space);
	return string(result_data.get(), result_size);
}

uint32_t StringUtil::StringToEnum(const EnumStringLiteral enum_list[], idx_t enum_count, const char *enum_name,
                                  const char *str_value) {
	for (idx_t i = 0; i < enum_count; i++) {
		if (CIEquals(enum_list[i].string, str_value)) {
			return enum_list[i].number;
		}
	}
	// string to enum conversion failed - generate candidates
	vector<string> candidates;
	for (idx_t i = 0; i < enum_count; i++) {
		candidates.push_back(enum_list[i].string);
	}
	auto closest_values = TopNJaroWinkler(candidates, str_value);
	auto message = CandidatesMessage(closest_values, "Candidates");
	throw NotImplementedException("Enum value: unrecognized value \"%s\" for enum \"%s\"\n%s", str_value, enum_name,
	                              message);
}

const char *StringUtil::EnumToString(const EnumStringLiteral enum_list[], idx_t enum_count, const char *enum_name,
                                     uint32_t enum_value) {
	for (idx_t i = 0; i < enum_count; i++) {
		if (enum_list[i].number == enum_value) {
			return enum_list[i].string;
		}
	}
	throw NotImplementedException("Enum value: unrecognized enum value \"%d\" for enum \"%s\"", enum_value, enum_name);
}

const uint8_t StringUtil::ASCII_TO_UPPER_MAP[] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,
    66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
    88,  89,  90,  91,  92,  93,  94,  95,  96,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,
    78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
    154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

const uint8_t StringUtil::ASCII_TO_LOWER_MAP[] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  97,
    98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
    154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

} // namespace duckdb

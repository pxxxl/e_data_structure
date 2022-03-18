#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class string {
public:
	//return the copy of the string
	string copy() const;

	//if the string is not empty, return true
	bool is_not_empty() const noexcept;
	
	//if this > sample, return 1
	//if this == sample, return 0
	//if this < sample, return -1
	int compare(const string& sample) const noexcept;

	//return the length of the string
	unsigned length() const noexcept;

	//clear the string
	void clear() noexcept;

	//attach sample string to this string's tail
	void concat(string sample) const;

	//return the substring which begins at pos with length : len
	string sub_str(unsigned pos, unsigned len) const;
	
	//return the index of the beginning of the substring
	//search begins at pos
	//status == OK, founded
	//status == INFEASIBLE not found
	std::tuple<unsigned, status> index(const string& sample, unsigned pos) const;

	//replace the substring with the sample input
	//search begins at pos
	void replace(const string& replaced, const string& sample, unsigned pos);

	//insert the sample string to pos
	void insert(string inserted, unsigned pos);

	//delete the substring begins at pos with length : len
	void delete(unsigned pos, unsigned len);

	~string();
};

}
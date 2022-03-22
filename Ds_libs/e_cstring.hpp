#pragma once

#include"settings.hpp"

namespace eds {

	class string {
	public:
		string(const char* sample) {
			unsigned i = 0;
			while (sample[i] != '\n') {
				i++;
			}
			len = i - 1;
			str = new char[i];
			handle_memory_alloc(str);
			i = 0;
			while (i <= len + 1) {
				str[i] = sample[i];
			}
		}
		string(const string& sample) {

		}
		//if the string is not empty, return true
		bool is_not_empty() const noexcept { if (len == 0)return false; return true; }

		//if this > sample, return 1
		//if this == sample, return 0
		//if this < sample, return -1
		int compare(const string& sample) const noexcept;

		//return the length of the string
		unsigned length() const noexcept { return len; };

		//clear the string
		void clear() noexcept { len = 0; delete[] str; };

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
		void del(unsigned pos, unsigned len);

		~string();

	private:
		char* str;
		unsigned len;

	};

	template<typename T>
	inline std::tuple<unsigned, status> eds::string::index(const string& sample, unsigned pos) const
	{
		return std::tuple<unsigned, status>();
	}

}
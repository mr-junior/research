#pragma once

#include <string>
#include <stringstream>
#include <exception>

/**
 * @brief Exception while converting.
 */
class base_exception : public std::exception
{
	public:
		/**
		 * @brief Constructor.
		 * @param message Exception message.
		 */
		base_exception(const std::string& message);
		virtual const char* what() const noexcept;

	private:
		std::string m_message;
};

class converter
{
	public:
		static std::unique_ptr<converter> from(const std::pair<char, std::string>& input);

	public:
		void to(const std::pair<char, std::string>& output) const;

	private:
		void from_boost(const std::ifstream& in);
		void from_adjacency_list(const std::ifstream& in);
		void from_matrix(const std::ifstream& in);

		void to_boost(std::ofstream& out) const;
		void to_adjacency_list(std::ofstream& out) const;
		void to_matrix(std::ofstream& out) const;

		void read_file(std::ifstream& in);

		void check_for_matrix() const;

	private:
		converter(const char input_format);
		converter(const converter& other) = delete;
		converter& operator=(converter& other) = delete;

	private:
		char m_input_format;
		std::stringstream m_input_file;
};

//
// Created by Sin Denis on 2020-05-28.
//
#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <cstdlib>

namespace NArgParse {
	class TParseException : public std::exception {
	public:

		TParseException(std::string msg) noexcept;
		const char* what() const noexcept override;

	private:
		const std::string Msg;
	};

	std::unordered_map<std::string, std::string> Parse(int argc, char* argv[]);
}

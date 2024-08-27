#pragma once
#include <string>
#include <vector>
#include <utility>

std::vector<std::pair<std::string, char>> Map(const std::string& filename, const std::string& contents);
std::string Reduce(const std::string& key, const std::vector<char>& values);
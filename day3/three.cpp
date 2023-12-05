
#include <fstream>
#include <iostream>
#include <numeric>
#include <string.h>
#include <string>
#include <vector>
bool is_symbol(const std::string &c) {
  std::string symbols = "!@#$%^&*()-+=[]{};:,<>/?"
                        "|~";
  for (char ch : c) {
    if (symbols.find(ch) != std::string::npos) {
      return true;
    }
  }
  return false;
}
bool is_number(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}

void extractNumbersFromRow(std::vector<std::vector<std::string>> &matrix,
                           std::vector<int> &num, size_t *i, size_t *j) {

  std::string number = matrix[*i][*j];
  int k = *j + 1;
  while (k < matrix[*i].size() && matrix[*i][k] != "." &&
         is_symbol(matrix[*i][k]) == false) {
    number += matrix[*i][k];
    k++;
  }
  // Calculate how many digits we have
  int digits = k - *j;
  *j += digits;
  // if there is a symbol toucing the number above below,left right or diagonal,
  // we need to add it to the number

  // above
  if (*i > 0) {
    if (is_symbol(matrix[*i - 1][*j - digits]) ||
        is_symbol(matrix[*i - 1][(*j - digits) + 1]) ||
        is_symbol(matrix[*i - 1][(*j - digits) + 2])) {

      num.push_back(std::stoi(number));
      return;
    }
  }
  // below
  if (*i < matrix.size() - 1) {
    if (is_symbol(matrix[*i + 1][*j - digits]) ||
        is_symbol(matrix[*i + 1][(*j - digits) + 1]) ||
        is_symbol(matrix[*i + 1][(*j - digits) + 2])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
  // right
  if (*j < matrix.size()) {
    if (is_symbol(matrix[*i][*j])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
  // left
  if (*i != 0) {
    if (is_symbol(matrix[*i][*j - digits - 1])) {

      num.push_back(std::stoi(number));
      return;
    }
  }
  // diagonal left up
  if (*i > 0 && *j > 0 && *j < matrix.size()) {
    if (is_symbol(matrix[*i - 1][*j - digits - 1])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
  // diagonal right up
  if (*i > 0 && *j < matrix[*i].size()) {
    if (is_symbol(matrix[*i - 1][*j])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
  // diagonal left down
  if (*i < matrix.size() - 1 && *j > 0) {
    if (is_symbol(matrix[*i + 1][*j - digits - 1])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
  // diagonal right down
  if (*i < matrix.size() - 1 && *j < matrix.size()) {
    if (is_symbol(matrix[*i + 1][*j])) {
      num.push_back(std::stoi(number));
      return;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Please provide the input file\n");
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  std::vector<std::vector<std::string>> matrix;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> row;
    for (size_t i = 0; i < line.size(); i++) {
      std::string s(1, line[i]);
      row.push_back(s);
    }
    matrix.push_back(row);
  }

  std::vector<int> numbers;

  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix[i].size(); j++)
      if (is_number(matrix[i][j])) {
        // if the digit is not a dot, we need to keep looking so find the last
        // digit of number and then we can convert it to int
        extractNumbersFromRow(matrix, numbers, &i, &j);
      }
  }

  for (int num : numbers) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  std::cout << "Sum: " << std::accumulate(numbers.begin(), numbers.end(), 0)
            << std::endl;

  return 0;
}


#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string.h>
#include <string>
#include <vector>
bool is_symbol(const std::string &c) {
  std::string symbols = "!@#$%^&*()-+=[]{};:,<>/?"
                        "|~";
  if (c.size() > 1) {
    return false;
  }
  for (char ch : c) {
    if (symbols.find(ch) != std::string::npos) {
      return true;
    }
  }
  return false;
}
bool is_gear(const std::string &c) {
  std::string symbols = "*";
  if (c.size() > 1) {
    return false;
  }
  for (char ch : c) {
    if (symbols.find(ch) != std::string::npos) {
      return true;
    }
  }
  return false;
}
bool is_number(const std::string &s) {
  if (s.empty())
    return false;
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
  if (*i > 0 && *j > 0 && *j <= matrix.size()) {
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

        extractNumbersFromRow(matrix, numbers, &i, &j);
      }
  }

  std::vector<std::vector<std::string>> gearMatrix;
  std::vector<int> gearRatio;

  std::cout << "Sum: " << std::accumulate(numbers.begin(), numbers.end(), 0)
            << std::endl;

  // loop through matrix and replace mulitple digits number with a single string
  // number
  for (size_t i = 0; i < matrix.size(); i++) {
    gearMatrix.push_back(std::vector<std::string>());
    for (size_t j = 0; j < matrix[i].size(); j++)

      if (is_number(matrix[i][j])) {
        std::string number = matrix[i][j];
        int k = j + 1;
        while (k < matrix[i].size() && matrix[i][k] != "." &&
               is_symbol(matrix[i][k]) == false) {
          number += matrix[i][k];
          k++;
        }

        gearMatrix[i].push_back(number);

        int digits = k - j - 1;
        j += digits;
        for (int q = 0; q < digits; q++) {
          gearMatrix[i].push_back(number);
        }

      } else {
        gearMatrix[i].push_back(matrix[i][j]);
      }
  }

  std::vector<std::string> tv;
  for (size_t i = 0; i < gearMatrix.size(); i++) {
    for (size_t j = 0; j < gearMatrix[i].size(); j++) {
      if (is_gear(matrix[i][j])) {

        std::vector<int> temp_array;

        // above
        if (i != 0) {
          if (is_number(gearMatrix[i - 1][j])) {
            temp_array.push_back(std::stoi(gearMatrix[i - 1][j]));
          }
        }
        // below
  if (i != gearMatrix.size() - 1) {
        if (is_number(gearMatrix[i + 1][j])) {
          temp_array.push_back(std::stoi(gearMatrix[i + 1][j]));
        }
  }
        // right
    if (j != gearMatrix[i].size() - 1) {
        if (is_number(gearMatrix[i][j + 1])) {
          temp_array.push_back(std::stoi(gearMatrix[i][j + 1]));
        }
    }
        // left

        if (is_number(gearMatrix[i][j - 1])) {
          temp_array.push_back(std::stoi(gearMatrix[i][j - 1]));
        }

        // diagonal left up
        if (i != 0) {
          if (is_number(gearMatrix[i - 1][j - 1])) {
            temp_array.push_back(std::stoi(gearMatrix[i - 1][j - 1]));
          }
        }
        if (i != 0) {
          if (j != gearMatrix[i].size() - 1) {
          // diagonal right up
          if (is_number(gearMatrix[i - 1][j + 1])) {
            temp_array.push_back(std::stoi(gearMatrix[i - 1][j + 1]));
          }
          }
        }
        // diagonal left down
      if (i != gearMatrix.size() - 1) {
        if (is_number(gearMatrix[i + 1][j - 1])) {
          temp_array.push_back(std::stoi(gearMatrix[i + 1][j - 1]));
        }
      }
        // diagonal right down
if (i != gearMatrix.size() - 1) 
  if (j != gearMatrix[i].size() - 1)
  {
        if (is_number(gearMatrix[i + 1][j + 1])) {
          temp_array.push_back(std::stoi(gearMatrix[i + 1][j + 1]));
        }
}
      
if (!temp_array.empty()) {
  if (std::equal(temp_array.begin() + 1, temp_array.end(), temp_array.begin())) {
    if (temp_array.size() >= 2) {
      gearRatio.push_back(temp_array[0] * temp_array[1]);
      std::string temp = std::to_string(temp_array[0]) + matrix[i][j] + std::to_string(temp_array[1]);
      tv.push_back(temp);
      break;
    }
  }
}

        if (temp_array.size() == 4) {

        } else {
          std::sort(temp_array.begin(), temp_array.end());
          temp_array.erase(std::unique(temp_array.begin(), temp_array.end()),
                           temp_array.end());
          if (temp_array.size() == 2) {
            std::string temp = std::to_string(temp_array[0]) + matrix[i][j] +
                               std::to_string(temp_array[1]);
            tv.push_back(temp);

            gearRatio.push_back(temp_array[0] * temp_array[1]);
          }
        }
      }
    }
  }
  int p = 0;
  for (std::string i : tv) {
    std::cout << p << ": " << i << std::endl;
    p++;
  }
  std::cout << "Gear Ratio: "
            << std::accumulate(gearRatio.begin(), gearRatio.end(), 0)
            << std::endl;
  return 0;
}

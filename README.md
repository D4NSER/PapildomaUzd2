# Additional task for the OOP exam
## Includes
```
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <set>
#include <regex>
```
- **`#include <iostream>`**: This library is used for input and output stream operations, such as `std::cin` and `std::cout`.
- **`#include <fstream>`**: This library is used for file stream operations, allowing the program to read from and write to files.
- **`#include <sstream>`**: This library allows the use of string streams, which can convert between strings and other data types.
- **`#include <string>`**: This library provides support for string operations.
- **`#include <cctype>`**: This library provides functions to classify and transform individual characters, such as `std::isalnum`.
- **`#include <map>`**: This library defines the `std::map` container, which is a sorted associative container that contains key-value pairs with unique keys.
- **`#include <set>`**: This library defines the `std::set` container, which is a sorted associative container that contains unique elements.
- **`#include <regex>`**: This library provides support for regular expressions, which are used for pattern matching within strings.

## Punctuation Filtering
```
std::string filterPunctuation(const std::string &word) {
    std::string filtered_word;
    for (char ch : word) {
        if (std::isalnum(ch) || ch == '.' || ch == '/' || ch == ':') {
            filtered_word += ch;
        }
    }
    return filtered_word;
}
```
- **Purpose**: This function removes unwanted punctuation from a word, allowing only alphanumeric characters and the characters '.', '/', and ':'.
- **Parameters**: `const std::string &word` - a reference to the input string (word) that needs filtering.
- **Local Variables**: `std::string filtered_word` - stores the resulting filtered word.
- **Loop**: Iterates over each character in the input word:
  - `std::isalnum(ch)`: Checks if the character is alphanumeric.
  - `ch == '.' || ch == '/' || ch == ':'`: Allows '.', '/', and ':' characters.
- **Return**: The filtered word with only allowed characters.

## URL Detection
```
bool isURL(const std::string &word) {
    const std::regex url_pattern(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt|uk|us|ca)))");
    return std::regex_search(word, url_pattern);
}
```
- **Purpose**: This function determines if a word matches a URL pattern.
- **Parameters**: `const std::string &word` - a reference to the input string to check.
- **Local Variables**: const `std::regex url_pattern` - a regular expression object that defines the pattern for URLs.
- **Regex Pattern**: `R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt|uk|us|ca)))"` - matches common URL patterns.
- **Return**: `std::regex_search(word, url_pattern)` returns true if the word matches the URL pattern, otherwise false.

## File Processing
```
void processFile(const std::string &filename, 
                 std::map<std::string, std::pair<int, std::set<int>>> &word_map,
                 std::map<std::string, std::pair<int, std::set<int>>> &url_map) {
    std::ifstream fd(filename);
    if (!fd) {
        std::cerr << "Unable to open file" << std::endl;
        exit(1);
    }

    std::string line;
    int line_number = 0;

    while (std::getline(fd, line)) {
        ++line_number;
        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            std::string filtered_word = filterPunctuation(word);
            if (!filtered_word.empty()) {
                if (isURL(filtered_word)) {
                    url_map[filtered_word].first++;
                    url_map[filtered_word].second.insert(line_number);
                } else {
                    word_map[filtered_word].first++;
                    word_map[filtered_word].second.insert(line_number);
                }
            }
        }
    }

    fd.close();
}
```
- Purpose: This function reads a file, processes each word to filter punctuation, identifies URLs, and records the occurrences and line numbers of each word and URL.
- Parameters:
  - const `std::string &filename`: The name of the file to be processed.
  - `std::map<std::string`, `std::pair<int`, `std::set<int>>> &word_map`: A map to store non-URL words, their counts, and the line numbers they appear on.
  - `std::map<std::string`, `std::pair<int`, `std::set<int>>> &url_map`: A map to store URLs, their counts, and the line numbers they appear on.
- File Opening: `std::ifstream fd(filename)` attempts to open the file. If it fails, an error message is printed and the program exits.
- Line Reading: `while (std::getline(fd, line))` reads the file line by line.
- Line Number: `++line_number` increments the line number counter for each line.
- Word Extraction: `std::istringstream iss(line)` initializes a string stream with the line, and `while (iss >> word)` extracts each word.
- Word Processing:
  - `std::string filtered_word` = filterPunctuation(word) filters the punctuation.
  - `if (!filtered_word.empty())`: Checks if the filtered word is not empty.
  - `if (isURL(filtered_word))`: Checks if the filtered word is a URL.
    - Updates `url_map` with the count and line numbers.
  - Otherwise, updates `word_map` with the count and line numbers.
- File Closing: `fd.close()` closes the input file.


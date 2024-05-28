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
- **Purpose**: This function reads a file, processes each word to filter punctuation, identifies URLs, and records the occurrences and line numbers of each word and URL.
- **Parameters**:
  - const `std::string &filename`: The name of the file to be processed.
  - `std::map<std::string`, `std::pair<int`, `std::set<int>>> &word_map`: A map to store non-URL words, their counts, and the line numbers they appear on.
  - `std::map<std::string`, `std::pair<int`, `std::set<int>>> &url_map`: A map to store URLs, their counts, and the line numbers they appear on.
- **File Opening**: `std::ifstream fd(filename)` attempts to open the file. If it fails, an error message is printed and the program exits.
- **Line Reading**: `while (std::getline(fd, line))` reads the file line by line.
- L**ine Number**: `++line_number` increments the line number counter for each line.
- **Word Extraction**: `std::istringstream iss(line)` initializes a string stream with the line, and `while (iss >> word)` extracts each word.
- **Word Processing:**
  - `std::string filtered_word` = filterPunctuation(word) filters the punctuation.
  - `if (!filtered_word.empty())`: Checks if the filtered word is not empty.
  - `if (isURL(filtered_word))`: Checks if the filtered word is a URL.
    - Updates `url_map` with the count and line numbers.
  - Otherwise, updates `word_map` with the count and line numbers.
- **File Closing**: `fd.close()` closes the input file.

## Writing Words To File
```
void writeWordsToFile(const std::string &filename, const std::map<std::string, std::pair<int, std::set<int>>> &word_map) {
    std::ofstream fr(filename);
    if (!fr) {
        std::cerr << "Unable to open file" << std::endl;
        exit(1);
    }

    fr << "Words:\n";
    for (const auto &entry : word_map) {
        if (entry.second.first > 1) {
            fr << entry.first << ": " << entry.second.first << " [lines: ";
            for (const int &ln : entry.second.second) {
                fr << ln << " ";
            }
            fr << "]" << std::endl;
        }
    }

    fr.close();
}
```
- **Purpose**: This function writes words (excluding URLs) that appear more than once, along with their counts and line numbers, to an output file.
- **Parameters**:
    - `const std::string &filename`: The name of the output file.
    - `const std::map<std::string`, `std::pair<int>`, `std::set<int>>> &word_map`: The map containing words, their counts, and line numbers.
- **File Opening**: `std::ofstream fr(filename)` opens the output file. If it fails, an error message is printed and the program exits.
- **Writing Header**: `fr << "Words:\n"` writes a header to the file.
- **Writing Data**:
    - `for (const auto &entry : word_map)`: Iterates over the map entries.
    - `if (entry.second.first > 1)`: Checks if the word appears more than once.
        - Writes the word, its count, and the line numbers to the file.
- **File Closing**: `fr.close()` closes the output file.

## Writing URLs To File
```
void writeURLsToFile(const std::string &filename, const std::map<std::string, std::pair<int, std::set<int>>> &url_map) {
    std::ofstream fa(filename);
    if (!fa) {
        std::cerr << "Unable to open file" << std::endl;
        exit(1);
    }

    fa << "URLs:\n";
    for (const auto &entry : url_map) {
        fa << entry.first << std::endl;
    }

    fa.close();
}
```
- **Purpose**: This function writes the detected URLs to an output file.
- **Parameters**:
    - const `std::string &filename`: The name of the output file.
    - const `std::map<std::string`, `std::pair<int>`, `std::set<int>>> &url_map`: The map containing URLs and their counts.
- **File Opening**: `std::ofstream fa(filename)` opens the output file. If it fails, an error message is printed and the program exits.
- **Writing Header**: `fa << "URLs:\n"` writes a header to the file.
- **Writing Data**:
    - `for (const auto &entry : url_map)`: Iterates over the map entries.
        - Writes each URL to the file.
- **File Closing**: `fa.close()` closes the output file.

## Main Function
```
int main() {
    std::map<std::string, std::pair<int, std::set<int>>> word_map;
    std::map<std::string, std::pair<int, std::set<int>>> url_map;

    // Process the file
    processFile("text.txt", word_map, url_map);

    // Write the results to output files
    writeWordsToFile("output.txt", word_map);
    writeURLsToFile("url.txt", url_map);

    return 0;
}
```
- **Purpose**: This is the entry point of the program. It initializes the data structures, processes the input file, and writes the results to the output files.
- **Variables**:
    - `std::map<std::string`, `std::pair<int>`, `std::set<int>>> word_map`: Map to store words, their counts, and line numbers.
    - `std::map<std::string`, `std::pair<int>`, `std::set<int>>> url_map`: Map to store URLs and their counts.
- **File Processing**: `processFile("text.txt", word_map, url_map)` processes the input file and populates the maps.
- **Writing Results**:
    - `writeWordsToFile("output.txt", word_map)`: Writes the word data to output.txt.
    - `writeURLsToFile("url.txt", url_map)`: Writes the URL data to url.txt.
- **Return**: Returns 0 to indicate successful execution.

## Summary
In summary, this program reads an input text file (In this case `text.txt`), processes each word to filter out unwanted punctuation, identifies URLs, counts the occurrences of each word and URL, records the line numbers they appear on, and writes the results to separate output files for words and URLs.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <set>
#include <regex>

// Function to filter out punctuation except '.', '/', and ':'
std::string filterPunctuation(const std::string &word) {
    std::string filtered_word;
    for (char ch : word) {
        if (std::isalnum(ch) || ch == '.' || ch == '/' || ch == ':') {
            filtered_word += ch;
        }
    }
    return filtered_word;
}

// Function to check if a word is a URL
bool isURL(const std::string &word) {
    const std::regex url_pattern(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt|uk|us|ca)))");
    return std::regex_search(word, url_pattern);
}

// Function to process the file and populate the word and URL maps
void processFile(const std::string &filename, 
                 std::map<std::string, std::pair<int, std::set<int> > > &word_map,
                 std::map<std::string, std::pair<int, std::set<int> > > &url_map) {
    std::ifstream fd(filename);
    if (!fd) {
        std::cerr << "Unable to open the file" << std::endl;
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

// Function to write words and their counts to the output file
void writeWordsToFile(const std::string &filename, const std::map<std::string, std::pair<int, std::set<int> > > &word_map) {
    std::ofstream fr(filename);
    if (!fr) {
        std::cerr << "Unable to open the file" << std::endl;
        exit(1);
    }

    fr << "The word count:\n";
    for (const auto &entry : word_map) {
        if (entry.second.first > 1) {
            fr << entry.first << ": " << entry.second.first << " [ In lines: ";
            for (const int &ln : entry.second.second) {
                fr << ln << " ";
            }
            fr << "]" << std::endl;
        }
    }

    fr.close();
}

// Function to write URLs to the output file
void writeURLsToFile(const std::string &filename, const std::map<std::string, std::pair<int, std::set<int> > > &url_map) {
    std::ofstream fa(filename);
    if (!fa) {
        std::cerr << "Unable to open the file" << std::endl;
        exit(1);
    }

    fa << "The URL count:\n";
    for (const auto &entry : url_map) {
        fa << entry.first << std::endl;
    }

    fa.close();
}

int main() {
    std::map<std::string, std::pair<int, std::set<int> > > word_map;
    std::map<std::string, std::pair<int, std::set<int> > > url_map;

    // Process the file
    processFile("text.txt", word_map, url_map);

    // Write the results to output files
    writeWordsToFile("output.txt", word_map);
    writeURLsToFile("url.txt", url_map);

    return 0;
}

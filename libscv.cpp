//
// Created by Ângelo Mutti on 06/07/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstring>
#include <vector>

using namespace std;


/**
 * Split a string by a delimiter.
 *
 * @param str The string to be split.
 * @param delimiter The delimiter to split the string by.
 *
 * @return vector<string> The split string.
 */
vector<string> split(const string &str, const char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool is_equal(const string &a, const string &b) {
    return strcmp(a.c_str(), b.c_str());
}

bool is_greater_than(const string &a, const string &b) {
    return strcmp(a.c_str(), b.c_str()) > 0;
}

bool is_less_than(const string &a, const string &b) {
    return strcmp(a.c_str(), b.c_str()) < 0;
}

/**
 * Check if a string contains a substring.
 *
 * @param mainStr The string to be checked.
 * @param subStr The substring to check for.
 *
 * @return bool True if the string contains the substring, false otherwise.
 */
bool contains(const std::string &mainStr, const std::string &subStr) {
    return mainStr.find(subStr) != std::string::npos;
}

/**
 * Get the selected columns and their index pairs.
 *
 * @param header The header of the CSV data.
 * @param selectedRows The selected columns to be processed.
 *
 * @return map<string, int> The selected columns and their index pairs.
 */
map<string, int> get_selected_column_and_index(const char header[], const char selectedRows[]) {
    vector<string> headers = split(header, ',');
    map<string, int> map;
    for (int i = 0; i < headers.size(); i++) {
        if (contains(selectedRows, headers[i])) {
            map.insert(pair<string, int>(headers[i], i));
        }
    }
    return map;
}

void filter_greater_than(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '>');
    string column = filterParts[0];
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (!is_greater_than(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}

void filter_is_equal(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '=');
    string column = filterParts[0];
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (is_equal(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}

void filter_greater_or_equal(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '=');
    string column = filterParts[0].substr(filterParts[0].size() - 1);
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (!is_greater_than(it->at(column), value) && !is_equal(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}

void filter_is_less_equal(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '=');
    string column = filterParts[0];
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (is_equal(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * Filter the CSV data based on the row filter definitions. For this in secific is "!="
 * @param csvData csv data to be filtered
 * @param filter filter to be applied
 */
void filter_is_not_equal(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '=');
    string column = filterParts[0].substr(0, filterParts[0].size() - 1);
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (!is_equal(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}


void filter_less_than(vector<map<string, string> > &csvData, const string &filter) {
    vector<string> filterParts = split(filter, '<');
    string column = filterParts[0];
    string value = filterParts[1];

    auto it = csvData.begin();
    while (it != csvData.end()) {
        if (!is_less_than(it->at(column), value)) {
            it = csvData.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * Filter the CSV data based on the row filter definitions.
 * a filter that it excpets its the full rowFilterDefinitionsString, as in the example below:
 *
 * header1>1\nheader3<10
 *
 * @param csvData The CSV data to be filtered.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void filter_csv_data(vector<map<string, string> > &csvData, const char rowFilterDefinitions[]) {
    vector<string> filters = split(rowFilterDefinitions, '\n');
    for (const auto &filter: filters) {
        try {
            if (contains(filter, ">")) {
                filter_greater_than(csvData, filter);
                continue;
            }
            if (contains(filter, "<")) {
                filter_less_than(csvData, filter);
                continue;
            }
            if (contains(filter, "!=")) {
                filter_is_not_equal(csvData, filter);
                continue;
            }
            if (contains(filter, ">=")) {
                filter_greater_or_equal(csvData, filter);
                continue;
            }
            if (contains(filter, "<=")) {
                filter_is_less_equal(csvData, filter);
                continue;
            }
            if (contains(filter, "=")) {
                filter_is_equal(csvData, filter);
                continue;
            }

            cout << "Invalid filter: " << filter << "\n";
        } catch (const std::exception &e) {
            cout << "Header '" << "header" << "' not found in CSV file/string" << filter << "\n";
        }
    }
}

/**
 * Print the CSV header.
 *
 * @param csvData The CSV data to be printed.
 *
 * @return void
 */
void print_csv_header(const vector<map<string, string> > &csvData) {
    if (csvData.empty()) {
        return;
    }

    string headers;
    for (const auto &pair: csvData[0]) {
        headers += pair.first + ",";
    }
    std::cout << headers.substr(0, headers.size() - 1) << std::endl;
}

/**
 * Print the CSV data.
 *
 * @param csvData The CSV data to be printed.
 *
 * @return void
 */
void print_csv_data(const vector<map<string, string> > &csvData) {
    if (csvData.empty()) {
        return;
    }

    string currLine;
    for (const auto &row: csvData) {
        for (const auto &pair: row) {
            currLine += pair.second + ",";
        }
        std::cout << currLine.substr(0, currLine.size() - 1) << std::endl;
        currLine.clear();
    }
}

/**
 * Process a line of CSV data.
 *
 * @param line The line of CSV data to be processed.
 * @param mapColumnIndex The selected columns and their index pairs.
 * @param interestedColumns The columns to be selected from the CSV data.
 *
 * @return map<string, string> The processed line of CSV data.
 */
map<string, string> process_line(string line, map<string, int> mapColumnIndex,
                                 vector<string> interestedColumns) {
    vector<string> fields = split(line, ',');
    map<string, string> row;
    string currLine;
    for (const auto &interestedColumn: interestedColumns) {
        int index = mapColumnIndex[interestedColumn];
        currLine += fields[index] + ",";
        row.insert(pair<string, string>(interestedColumn, fields[index]));
    }

    return row;
}

/**
 * Process the CSV data by applying filters and selecting columns.
 *
 * @param csv The CSV data to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[]) {
    vector<string> csvLines = split(csv, '\n');
    vector<map<string, string> > csvData;

    map<string, int> mapColumnIndex = get_selected_column_and_index(csvLines[0].c_str(), selectedColumns);
    vector<string> interestedColumns = split(selectedColumns, ',');
    for (int i = 1; i < csvLines.size(); i++) {
        csvData.push_back(process_line(csvLines[i], mapColumnIndex, interestedColumns));
    }

    print_csv_header(csvData);
    filter_csv_data(csvData, rowFilterDefinitions);
    print_csv_data(csvData);
}

/**
 * Process the CSV data by applying filters and selecting columns.
 *
 * @param csvFilePath The file path of the CSV to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsvFile(const char filename[], const char selectedColumns[], const char rowFilterDefinitions[]) {
    vector<map<string, string> > csvData;
    ifstream file(filename);
    string line;
    getline(file, line);

    map<string, int> mapColumnIndex = get_selected_column_and_index(line.c_str(), selectedColumns);

    vector<string> interestedColumns = split(selectedColumns, ',');
    while (getline(file, line)) {
        csvData.push_back(process_line(line, mapColumnIndex, interestedColumns));
    }
    file.close();

    print_csv_header(csvData);
    filter_csv_data(csvData, rowFilterDefinitions);
    print_csv_data(csvData);
}

int main() {
    processCsvFile("data.csv", "col1,col4,col3,col5", "col3!=l3c3");
    return 0;
}

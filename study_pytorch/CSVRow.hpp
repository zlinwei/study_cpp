//
// Created by linwei on 2019-07-22.
//

#ifndef STUDY_CPP_CSVROW_HPP
#define STUDY_CPP_CSVROW_HPP

#include <iostream>
#include <sstream>
#include <vector>

class CSVRow {
public:
    std::string const &operator[](std::size_t index) const {
        return m_data.at(index);
    }

    std::size_t size() const {
        return m_data.size();
    }

    void readNextRow(std::istream &str) {
        std::string line;
        std::getline(str, line);

        std::stringstream lineStream(line);
        std::string cell;

        m_data.clear();
        while (std::getline(lineStream, cell, ',')) {
            m_data.push_back(cell);
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty()) {
            // If there was a trailing comma then add an empty element.
            m_data.emplace_back("");
        }
    }

private:
    std::vector<std::string> m_data;
};

std::istream &operator>>(std::istream &str, CSVRow &data) {
    data.readNextRow(str);
    return str;
}


#endif //STUDY_CPP_CSVROW_HPP

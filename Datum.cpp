#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <string>
using namespace std;

#include "Datum.h"
#include "Vector.h"

// You may want to write this function, but you do not have to
[[nodiscard]] bool isBadDataEntry(const Datum& datum) {
    if (datum.total != datum.compute_total()) {
        return true;
    }
    float x = datum.positivity - (datum.compute_positivity()*100);
    if (std::abs(x) > 0.1) {
        return true;
    }
    return false;
}


unsigned int Datum::compute_total() const noexcept {
    return negative + positive;
}

float Datum::compute_positivity() const noexcept{
    return ((positive * 1.0) / compute_total());
}

std::istream& operator>>(std::istream& in, Datum& datum) {
    getline(in, datum.week, ',');
    string temp;
    getline(in, temp, ',');
    datum.negative = std::stoi(temp);
    getline(in, temp, ',');
    datum.positive = std::stoi(temp);
    getline(in, temp, ',');
    datum.total = std::stoi(temp);
    getline(in, temp, '%');
    datum.positivity = std::stof(temp);

    return in;

}

Vector<Datum> readData(std::istream& file) {
    istringstream ss;
    Vector<Datum> vector;
    string str;
    getline(file, str); // Get header
    while (getline(file, str)) {
        istringstream ss(str);
        Datum dt;
        ss >> dt;
        vector.push_back(dt);
    }

    return vector;
}

Vector<Datum> badDataEntries(const Vector<Datum>& data) noexcept {
    Vector<Datum> vec;
    for (size_t i = 0; i < data.size(); i++) {
        if (isBadDataEntry(data[i])) {
            vec.push_back(data[i]);
        }  
    }
    return vec;
}

bool goodData(const Vector<Datum>& data) noexcept {
    for (size_t i = 0; i < data.size(); i++) {
        if (isBadDataEntry(data[i])) {
            return false;
        }  
    }
    return true;
}
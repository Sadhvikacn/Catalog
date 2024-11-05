#include <iostream>
#include <fstream>
#include <json/json.h> // Requires JSON parsing library for C++
#include <vector>
#include <cmath>

struct Root {
    int x;
    long long y;
};

// Helper function to convert value from any base to decimal
long long decodeValue(const std::string& value, int base) {
    long long decimalValue = 0;
    for (char digit : value) {
        decimalValue = decimalValue * base + (isdigit(digit) ? digit - '0' : digit - 'a' + 10);
    }
    return decimalValue;
}

// Function to read JSON input and parse roots
std::vector<Root> parseInput(const std::string& filename, int &n, int &k) {
    std::ifstream file(filename);
    Json::Value data;
    file >> data;
    n = data["n"].asInt();
    k = data["k"].asInt();

    std::vector<Root> roots;
    for (const auto& key : data.getMemberNames()) {
        if (key != "n" && key != "k") {
            int x = std::stoi(key);
            int base = data[key]["base"].asInt();
            std::string value = data[key]["value"].asString();
            long long y = decodeValue(value, base);
            roots.push_back({x, y});
        }
    }
    return roots;
}

// Lagrange Interpolation to find the constant term (secret)
long long findConstantTerm(const std::vector<Root>& roots, int k) {
    long long secret = 0;

    for (int i = 0; i < k; ++i) {
        long long term = roots[i].y;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= -roots[j].x;
                term /= (roots[i].x - roots[j].x);
            }
        }
        secret += term;
    }
    return secret;
}

int main() {
    int n, k;
    std::vector<Root> roots = parseInput("testcase1.json", n, k);
    long long secret1 = findConstantTerm(roots, k);
    
    roots = parseInput("testcase2.json", n, k);
    long long secret2 = findConstantTerm(roots, k);
    
    std::cout << "Secret for test case 1: " << secret1 << std::endl;
    std::cout << "Secret for test case 2: " << secret2 << std::endl;
    
    return 0;
}

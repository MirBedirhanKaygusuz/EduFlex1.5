#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

bool isVariable(const string &token) {
    return isalpha(token[0]);
}

vector<string> extractVariables(const string &expr) {
    vector<string> variables;
    istringstream iss(expr);
    string token;
    while (iss >> token) {
        if (isVariable(token)) {
            variables.push_back(token);
        }
    }
    return variables;
}

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    int n;
    infile >> n;
    infile.ignore();

    vector<string> lines(n);
    vector<vector<int>> adjacency_list(n);
    map<string, int> last_assignment;

    for (int i = 0; i < n; ++i) {
        getline(infile, lines[i]);

        if (lines[i].substr(0, 2) == "if" || lines[i].substr(0, 5) == "while") {
            size_t condition_start = lines[i].find('(') + 1;
            size_t condition_end = lines[i].find(')');
            string condition = lines[i].substr(condition_start, condition_end - condition_start);

            vector<string> variables = extractVariables(condition);
            for (const string &var : variables) {
                if (last_assignment.find(var) != last_assignment.end()) {
                    adjacency_list[last_assignment[var]].push_back(i);
                }
            }

            if (lines[i].substr(0, 5) == "while") {
                size_t body_start = lines[i].find(':') + 1;
                string body = lines[i].substr(body_start);

                variables = extractVariables(body);
                for (const string &var : variables) {
                    if (last_assignment.find(var) != last_assignment.end()) {
                        adjacency_list[last_assignment[var]].push_back(i);
                    }
                }

                // Handle self-dependency in while loop
                for (const string &var : variables) {
                    if (body.find(var + " =") != string::npos) {
                        adjacency_list[i].push_back(i);
                    }
                }
            }
        } else {
            size_t equals_pos = lines[i].find('=');
            string lhs = lines[i].substr(0, equals_pos);
            lhs.erase(remove(lhs.begin(), lhs.end(), ' '), lhs.end());

            string rhs = lines[i].substr(equals_pos + 1);
            vector<string> variables = extractVariables(rhs);
            for (const string &var : variables) {
                if (last_assignment.find(var) != last_assignment.end()) {
                    adjacency_list[last_assignment[var]].push_back(i);
                }
            }

            // Handle self-dependency
            if (rhs.find(lhs) != string::npos) {
                adjacency_list[i].push_back(i);
            }

            last_assignment[lhs] = i;
        }
    }

    int edge_count = 0;
    for (const auto &edges : adjacency_list) {
        edge_count += edges.size();
    }

    outfile << n << "\n" << edge_count << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j : adjacency_list[i]) {
            outfile << i + 1 << " " << j + 1 << "\n";
        }
    }

    return 0;
}
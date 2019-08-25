//
// Created by linwei on 2019-08-22.
//
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Solution {
public:
    void printQueue(queue<int> q) {
        while (!q.empty()) {
            cout << q.front() << " ";
            q.pop();
        }
        cout << endl;
    }

    /**
     * @param org: a permutation of the integers from 1 to n
     * @param seqs: a list of sequences
     * @return: true if it can be reconstructed only one or false
     */
    bool sequenceReconstruction(vector<int> &org, vector<vector<int>> &seqs) {
        unordered_map<int, unordered_set<int>> g;
        unordered_map<int, int> indegree;
        queue<int> Q;
        vector<int> tp;

        for (auto &s : seqs) {
            for (int i = 0; i < s.size(); ++i) {
                cout << s[i] << endl;
                g[s[i]];
                if (i + 1 < s.size()) {
                    if (g[s[i]].count(s[i + 1]) == 0) {
                        indegree[s[i + 1]] += 1;
                    }
                    g[s[i]].insert(s[i + 1]);
                    cout << s[i] << "->" << s[i + 1] << endl;
                }
            }
        }

        for (auto &gg: g) {
            cout << gg.first << " : ";
            for (auto &i  : gg.second) {
                std::cout << i << " ";
            }
            std::cout << endl;
        }

        cout << "\n\n";
        for (auto i : indegree) {
            cout << i.first << " " << i.second << endl;
        }

        cout << "\n\n" << endl;

        if (g.size() != org.size())
            return false;

        if (g.size() == 0 && org.size() == 0)
            return true;

        for (auto p : g) {
            if (indegree[p.first] == 0) {
                Q.push(p.first);
                tp.push_back(p.first);
            }
        }

        std::cout << "q size: " << Q.size() << endl;
        printQueue(Q);

        cout << "tp size: " << tp.size() << endl;
        for (auto i : tp) {
            cout << i << " ";
        }
        cout << endl;

        if (tp.size() != 1)
            return false;

        while (!Q.empty()) {
            if (Q.size() != 1)
                return false;

            int curr = Q.front();
            Q.pop();

            int count = 0;
            for (auto next : g[curr]) {
                indegree[next] -= 1;
                if (indegree[next] == 0) {
                    count += 1;
                    Q.push(next);
                    tp.push_back(next);
                }
            }
        }

        return tp == org;
    }


};

int main(int, char *[]) {
    vector<int> org = {4, 1, 5, 2, 6, 3};
    vector<vector<int> > seqs = {
            {5, 2, 6, 3},
            {4, 1, 5, 6}
    };

    Solution solution{};
    cout << boolalpha << solution.sequenceReconstruction(org, seqs) << endl;

    return 0;
}


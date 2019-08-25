//
// Created by linwei on 2019-08-22.
//

#include<iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

class Solution {
public:
    /*
     * @param numCourses: a total of n courses
     * @param prerequisites: a list of prerequisite pairs
     * @return: true if can finish all courses or false
     */
    bool canFinish(int numCourses, vector<pair<int, int>> &prerequisites) {
        // Write your code here
        vector<unordered_multiset<int>> edges(numCourses);
        vector<int> d(numCourses, 0);
        for (int i = 0; i < prerequisites.size(); ++i) {
            edges[prerequisites[i].second].insert(prerequisites[i].first);
            d[prerequisites[i].first]++;
        }

        for (auto &item : edges) {
            for (auto &i : item) {
                cout << i << " ";
            }
            cout << endl;
        }

        queue<int> q;
        for (int i = 0; i < numCourses; ++i)
            if (d[i] == 0)
                q.push(i);

        int node = 0;
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            node++;
            for (auto it = edges[x].begin(); it != edges[x].end(); ++it) {
                --d[*it];
                if (d[*it] == 0) {
                    q.push(*it);
                }
            }
        }
        return node == numCourses;

    }
};


int main(int, char *[]) {
    Solution solution{};
    vector<std::pair<int, int>> input = {{1, 0},
                                         {0, 4},
                                         {2, 3}};
    cout << boolalpha << solution.canFinish(5, input) << endl;


    return 0;
}
//
// Created by linwei on 2019-07-25.
//
#include <iostream>

using namespace std;

class ListNode {
public:
    int val;
    ListNode *next;

    explicit ListNode(int val) {
        this->val = val;
        this->next = nullptr;
    }
};

void Append(ListNode *head, int val) {
    auto p = head;
    while (p->next != nullptr) {
        p = p->next;
    }
    p->next = new ListNode(val);
}

void Show(ListNode *head) {
    auto p = head;
    while (p != nullptr) {
        std::cout << p->val << " ";
        p = p->next;
    }
    std::cout << std::endl;
}

class Solution {
public:
    /**
     * @param head: The head of linked list.
     * @return: You should return the head of the sorted linked list, using constant space complexity.
     */
    static ListNode *sortList(ListNode *head) {
        // write your code here
        if (head == nullptr || head->next == nullptr)return head;
        Qsort(head, nullptr);
        return head;
    }

    static ListNode *partition(ListNode *head, ListNode *tail) {
        int key = head->val;
        auto *left = head;
        auto *right = head->next;
        while (right != tail) {
            if (right->val < key) {
                left = left->next;
                swap(left->val, right->val);
                Show(head);
            }
            right = right->next;
        }

        swap(head->val, left->val);
        Show(head);
        return left;
    }

    static void Qsort(ListNode *head, ListNode *tail) {
        if (head != tail) {
            ListNode *mid = partition(head, tail);
            std::cout << "\n\n\n\n" << std::endl;
            Qsort(head, mid);
            Qsort(mid->next, tail);
        }
    }
};

int main(int, char *[]) {
    auto *head = new ListNode(3);
    Append(head, 6);
    Append(head, 3);
    Append(head, 6);
    Append(head, 5);
    Append(head, 3);
    Append(head, 1);
    Append(head, 4);
    Append(head, 2);
    Append(head, 1);

    Append(head, 2);
    Append(head, 7);
    Append(head, 9);
    Append(head, 1);
    Append(head, 4);

    Show(head);

    Solution::sortList(head);

    Show(head);

    return 0;
}
// https://leetcode.com/problems/reverse-linked-list/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* currentNode = head;
        vector<int> v = {};
        
        while (currentNode){
            v.push_back(currentNode->val);
            currentNode = currentNode->next;
        }
        int i = v.size()-1;
        
        currentNode = head;
        while (currentNode){
            currentNode->val = v[i];
            i--;
            currentNode = currentNode->next;
        }        
        
        return head;
    }
};
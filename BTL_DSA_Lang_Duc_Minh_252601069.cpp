#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f, Node* l = NULL, Node* r = NULL)
        : ch(c), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

Node* buildTree(const map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (map<char, int>::const_iterator it = freq.begin(); it != freq.end(); ++it)
        pq.push(new Node(it->first, it->second));
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node('\0', a->freq + b->freq, a, b));
    }
    return pq.top();
}

void assignCodes(Node* root, string code, map<char, string>& table) {
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL) {
        table[root->ch] = code.empty() ? "0" : code;
        return;
    }
    assignCodes(root->left,  code + "0", table);
    assignCodes(root->right, code + "1", table);
}

string compress(const string& text, map<char, string>& table) {
    string bits;
    for (size_t i = 0; i < text.size(); i++)
        bits += table[text[i]];
    return bits;
}

string decompress(const string& bits, Node* root) {
    string out;
    if (root->left == NULL && root->right == NULL) {
        out.assign(bits.size(), root->ch);
        return out;
    }
    Node* cur = root;
    for (size_t i = 0; i < bits.size(); i++) {
        cur = (bits[i] == '0') ? cur->left : cur->right;
        if (cur->left == NULL && cur->right == NULL) {
            out += cur->ch;
            cur = root;
        }
    }
    return out;
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    string text;
    while (true) {
        cout << "Nhap chuoi can nen (Enter de thoat): ";
        getline(cin, text);
        if (text.empty()) break;

        map<char, int> freq;
        for (size_t i = 0; i < text.size(); i++) freq[text[i]]++;

        Node* root = buildTree(freq);
        map<char, string> table;
        assignCodes(root, "", table);

        string bits = compress(text, table);
        string back = decompress(bits, root);

        cout << "\nKy tu | Tan suat | Ma Huffman\n";
        for (map<char, int>::iterator it = freq.begin(); it != freq.end(); ++it) {
            string name = (it->first == ' ') ? "(cach)" : string(1, it->first);
            cout << "  " << name << "   |    " << it->second << "     | " << table[it->first] << "\n";
        }
        cout << "\nChuoi bit sau nen : " << bits << "\n";
        cout << "Kich thuoc goc    : " << text.size() * 8 << " bit\n";
        cout << "Kich thuoc nen    : " << bits.size() << " bit\n";
        cout << "Chuoi sau giai nen: " << back << "\n";
        cout << (back == text ? "=> Giai nen DUNG.\n\n" : "=> SAI!\n\n");

        freeTree(root);
    }
    return 0;
}

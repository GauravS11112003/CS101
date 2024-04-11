#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>

struct Node {
    int value;
    Node *left, *right;

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class HuffmanTree {
public:
    Node* buildTree(const std::vector<int>& inorder, const std::vector<int>& levelorder);
    std::string decode(const std::string& encoded, Node* root);
     void readTraversalData(const std::string& filename, std::vector<int>& traversal);
private:
    Node* buildTreeUtil(const std::vector<int>& inorder, int inStart, int inEnd,
                        std::unordered_map<int, int>& levelOrderMap);
    int findIndex(const std::vector<int>& arr, int start, int end, int value);
    
};

void HuffmanTree::readTraversalData(const std::string& filename, std::vector<int>& traversal) {
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::istringstream iss(line);
            int num;
            while (iss >> num) {
                traversal.push_back(num);
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int HuffmanTree::findIndex(const std::vector<int>& arr, int start, int end, int value) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}

Node* HuffmanTree::buildTreeUtil(const std::vector<int>& inorder, int inStart, int inEnd,
                                 std::unordered_map<int, int>& levelOrderMap) {
    if (inStart > inEnd) {
        return nullptr;
    }

    int minValueIndex = inStart;
    for (int i = inStart + 1; i <= inEnd; i++) {
        if (levelOrderMap[inorder[i]] < levelOrderMap[inorder[minValueIndex]]) {
            minValueIndex = i;
        }
    }

    Node* root = new Node(inorder[minValueIndex]);

    if (inStart == inEnd) {
        return root;
    }

    root->left = buildTreeUtil(inorder, inStart, minValueIndex - 1, levelOrderMap);
    root->right = buildTreeUtil(inorder, minValueIndex + 1, inEnd, levelOrderMap);

    return root;
}

Node* HuffmanTree::buildTree(const std::vector<int>& inorder, const std::vector<int>& levelorder) {
    std::unordered_map<int, int> levelOrderMap;
    for (size_t i = 0; i < levelorder.size(); i++) {
        levelOrderMap[levelorder[i]] = i;
    }
    return buildTreeUtil(inorder, 0, inorder.size() - 1, levelOrderMap);
}

std::string HuffmanTree::decode(const std::string& encoded, Node* root) {
    std::string decoded = "";
    Node* current = root;
    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (!current->left && !current->right) {
            decoded += char(current->value);
            current = root;
        }
    }
    return decoded;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <inorder file> <levelorder file> <encoded text file>" << std::endl;
        return 1;
    }

    std::vector<int> inorder, levelorder;
    HuffmanTree tree;
    tree.readTraversalData(argv[1], inorder);
    tree.readTraversalData(argv[2], levelorder);

    Node* root = tree.buildTree(inorder, levelorder);

    std::string encodedText, line;
    std::ifstream encodedFile(argv[3]);
    while (std::getline(encodedFile, line)) {
        encodedText += line; // Concatenate each line to form the complete encoded text
    }
    encodedFile.close();

    std::cout << tree.decode(encodedText, root);

    return 0;
}



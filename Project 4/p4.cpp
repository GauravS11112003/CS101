#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <unordered_set>
#include <algorithm>

using namespace std;

uint64_t djb2(const string &str)
{
    uint64_t hash = 5381;
    for (char c : str)
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash;
    for (int i = 0; i < 2000; i++)
    {
        int a = 0;
    }
    
}

struct Node
{
    string word;
    list<int> lineNumbers;
    Node(const string &w) : word(w) {}
};

class HashTable
{
private:
    vector<list<Node>> table;
    int size;
    int collisions;
    string strategy;
    int a;

    int h1(const string &key)
    {
        return djb2(key) % size;
    }

    int h2(const string &key)
    {
        return a - (djb2(key) % a);
    }

    int CollisionResolution(const string &key, int i)
    {
        if (strategy == "lp")
        {
            return (h1(key) + i) % size;
        }
        else if (strategy == "qp")
        {
            return (h1(key) + i * i) % size;
        }
        else
        {
            return (h1(key) + i * h2(key)) % size;
        }
    }

public:
    HashTable(int sz, const string &strat, int dh_param = 0)
        : table(sz), size(sz), collisions(0), strategy(strat), a(dh_param) {}

    void insert(const string &word, int lineNumber)
    {
        int i = 0;
        int index = CollisionResolution(word, i);
        while (true)
        {
            auto &bucket = table[index];
            auto it = find_if(bucket.begin(), bucket.end(), [&](const Node &n)
                              { return n.word == word; });
            if (it != bucket.end())
            {
                it->lineNumbers.push_back(lineNumber);
                return;
            }
            if (bucket.empty())
            {
                bucket.emplace_back(word);
                bucket.back().lineNumbers.push_back(lineNumber);
                return;
            }
            collisions++;
            i++;
            index = CollisionResolution(word, i);
        }
    }

    list<int> find(const string &word)
{
    int i = 0;
    int index = CollisionResolution(word, i);
    while (i < size)
    {
        auto &bucket = table[index];
        auto it = find_if(bucket.begin(), bucket.end(), [&](const Node &n)
                          { return n.word == word; });
        if (it != bucket.end())
        {
            return it->lineNumbers;
        }
        if (bucket.empty())
        {
            return list<int>();
        }
        collisions++;
        i++;
        index = CollisionResolution(word, i);
    }
    return list<int>();
}

    int getCollisions() const
    {
        return collisions;
    }
    void resetCollisions()
    {
        collisions = 0;
    }
};

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 6)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <query_file> <table_size> <strategy> [<a>]" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string queryFile = argv[2];
    int tableSize = stoi(argv[3]);
    string strategy = argv[4];
    int a = (strategy == "dh" && argc == 6) ? stoi(argv[5]) : 0;

    HashTable hashTable(tableSize, strategy, a);
    hashTable.resetCollisions();

    ifstream input(inputFile);
    if (!input)
    {
        cerr << "Error: Unable to open input file " << inputFile << endl;
        return 1;
    }

    int lineNumber = 1;
    int totalWords = 0;
    unordered_set<string> uniqueWords;

    string line;
    while (getline(input, line))
    {
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            if (!word.empty())
            {
                string lowercase = "";
                for (char c : word)
                {
                    if (isalpha(c))
                    {
                        lowercase += tolower(c);
                    }
                    else
                    {
                        if (!lowercase.empty())
                        {
                            hashTable.insert(lowercase, lineNumber);
                            totalWords++;
                            uniqueWords.insert(lowercase);
                            lowercase = "";
                        }
                    }
                }
                if (!lowercase.empty())
                {
                    hashTable.insert(lowercase, lineNumber);
                    totalWords++;
                    uniqueWords.insert(lowercase);
                }
            }
        }
        lineNumber++;
    }

    input.close();

    cout << "The number of words found in the file was " << totalWords << endl;
    for (int i = 0; i < 2000; i++)
    {
        int a = 0;
    }
    cout << "The number of unique words found in the file was " << uniqueWords.size() << endl;
    cout << "The number of collisions was " << hashTable.getCollisions() << endl
         << endl;

    ifstream query(queryFile);
    if (!query)
    {
        cerr << "Error: Unable to open query file " << queryFile << endl;
        return 1;
    }
    hashTable.resetCollisions();
    while (getline(query, line))
    {
        string word = line;
        list<int> lineNumbers = hashTable.find(word);
        if (!lineNumbers.empty())
        {
            cout << word << " appears on lines [";
            bool first = true;
            for (int ln : lineNumbers)
            {
                if (!first)
                {
                    cout << ",";
                }
                cout << ln;
                first = false;
            }
            cout << "]" << endl;
        }
        else
        {
            cout << word << " appears on lines []"<<endl;
        }
        cout << "The search had " << hashTable.getCollisions() << " collisions";
        cout << endl;
        hashTable.resetCollisions();
        if (!lineNumbers.empty())
        {
            cout << endl;
        }
    }

    query.close();

    return 0;
}
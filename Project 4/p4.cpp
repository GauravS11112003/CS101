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


uint64_t generateHash(const string &str) {
    uint64_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(tolower(c));
    }
    return hash;
}

struct WordEntry {
    string word;
    list<int> lineNumbers;
    WordEntry(const string &word) : word(word) {}
};

class WordHashTable {
private:
    vector<list<WordEntry>> table;
    int size;
    int collisions;
    string strategy;
    int step;

    int primaryIndex(const string &key) {
        return generateHash(key) % size;
    }

    int secondaryIndex(const string &key) {
        return step - (generateHash(key) % step);
    }

    int resolveIndex(const string &key, int attempt) {
        int index = primaryIndex(key);
        if (strategy == "lp") {
            return (index + attempt) % size;
        } else if (strategy == "qp") {
            return (index + attempt * attempt) % size;
        } else {
            return (index + attempt * secondaryIndex(key)) % size;
        }
    }

public:
    WordHashTable(int capacity, const string &resStrategy, int stepSize = 0)
        : table(capacity), size(capacity), collisions(0), strategy(resStrategy), step(stepSize) {}

    void insert(const string &word, int line) {
        for (int i = 0; i < size; ++i) {
            int idx = resolveIndex(word, i);
            auto &bucket = table[idx];
            auto it = find_if(bucket.begin(), bucket.end(), [&](const WordEntry &entry) { return entry.word == word; });
            if (it != bucket.end()) {
                it->lineNumbers.push_back(line);
                return;
            }
            if (bucket.empty()) {
                bucket.emplace_back(word);
                bucket.back().lineNumbers.push_back(line);
                return;
            }
            ++collisions;
        }
    }

    list<int> find(const string &word) {
        for (int i = 0; i < size; ++i) {
            int idx = resolveIndex(word, i);
            auto &bucket = table[idx];
            auto it = find_if(bucket.begin(), bucket.end(), [&](const WordEntry &entry) { return entry.word == word; });
            if (it != bucket.end()) {
                return it->lineNumbers;
            }
            if (bucket.empty()) {
                break;
            }
            ++collisions;
        }
        return list<int>();
    }

    int getCollisionCount() const { return collisions; }
    void clearCollisions() { collisions = 0; }
};

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 6)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <query_file> <table_size> <strategy> [<a>]" << endl;
        return 1;
    }

    string sourceFile = argv[1];
    string searchFile = argv[2];
    int hashSize = stoi(argv[3]);
    string collisionStrategy = argv[4];
    int hashStep = (collisionStrategy == "dh" && argc == 6) ? stoi(argv[5]) : 0;

    WordHashTable wordsTable(hashSize, collisionStrategy, hashStep);
    wordsTable.clearCollisions();

    ifstream sourceStream(sourceFile);
    if (!sourceStream)
    {
        cerr << "Error: Could not open source file " << sourceFile << endl;
        return 1;
    }

    int lineNo = 1;
    int totalWordCount = 0;
    unordered_set<string> distinctWords;

    string currentLine;
        while (getline(sourceStream, currentLine))
    {
        istringstream lineStream(currentLine);
        string word;
        while (lineStream >> word)
        {
            string processedWord = "";
            for (char character : word)
            {
                if (isalpha(character))
                {
                    processedWord += tolower(character);
                }
                else
                {
                    if (!processedWord.empty())
                    {
                        wordsTable.insert(processedWord, lineNo);
                        totalWordCount++;
                        distinctWords.insert(processedWord);
                        processedWord = "";
                    }
                }
            }
            if (!processedWord.empty())
            {
                wordsTable.insert(processedWord, lineNo);
                totalWordCount++;
                distinctWords.insert(processedWord);
            }
        }
        lineNo++;
    }
    sourceStream.close();

   cout << "The number of words found in the file was " << totalWordCount << endl;
    cout << "The number of unique words found in the file was " << distinctWords.size() << endl;
    cout << "The number of collisions was " << wordsTable.getCollisionCount() << endl << endl;

    ifstream queryStream(searchFile);
    if (!queryStream) {
        cerr << "Error: Could not open query file " << searchFile << endl;
        return 1;
    }

    while (getline(queryStream, currentLine)) {
        string queryWord = currentLine;
        int preSearchCollisions = wordsTable.getCollisionCount();
        list<int> occurrences = wordsTable.find(queryWord);
        int searchCollisions = wordsTable.getCollisionCount() - preSearchCollisions;

        if (!occurrences.empty()) {
            cout << queryWord << " appears on lines [";
            for (auto iter = occurrences.begin(); iter != occurrences.end(); ++iter) {
                if (iter != occurrences.begin()) cout << ",";
                cout << *iter;
            }
            cout << "]" << endl;
        } else {
            cout << queryWord << " appears on lines []" << endl;
        }
        cout << "The search had " << searchCollisions << " collisions" << endl;

        wordsTable.clearCollisions();
        if (!occurrences.empty()) {
            cout << endl;
        }

    }

    queryStream.close();
    return 0;
}


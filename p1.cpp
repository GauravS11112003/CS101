#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <map>
#include <cctype>
#include <cfloat>
#include <algorithm>
#include <iomanip>

using namespace std;

// Function declarations
vector<string> tokenize(const string &s, char delimiter);
string stripQuotes(const string &s);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 1;
    }

    string filename = argv[1];
    int colWidth = stoi(argv[2]);

    pair<vector<string>, vector<vector<pair<string, string>>>> dataRows;

    {
        vector<string> headers;
        vector<vector<pair<string, string>>> data;
        ifstream file(filename);
        string line;
        while (getline(file, line))
        {
            vector<string> values = tokenize(line, ',');
            if (line.back() == ',')
            {
                values.push_back("");
            }
            if (headers.empty())
            {
                for (string &value : values)
                {
                    headers.push_back(stripQuotes(value));
                }
            }
            else
            {
                vector<pair<string, string>> entry;
                for (size_t i = 0; i < headers.size() && i < values.size(); ++i)
                {
                    entry.push_back(make_pair(headers[i], stripQuotes(values[i])));
                }
                data.push_back(entry);
            }
        }
        dataRows = make_pair(headers, data);
    }

    vector<string> headerNames = dataRows.first;
    vector<vector<pair<string, string>>> data = dataRows.second;
    if (data.empty())
    {
        cout << "Failed to open \"" << filename << "\"" << endl;
        return 1;
    }

    string userCommand;
    cout << "Enter a command or \"help\" for a command list:" << endl;
    while (cin >> userCommand)
    {
        switch (userCommand[0])
        {
        case 'q':
            return 0;
        case 'h':
            cout << "command list:\n\tprint\n\tcols\n\tsearch *|col_name val\n\tmin col_name\n\tmax col_name\n\tavg col_name" << endl;
            break;
        case 'p':
            // Print data
            if (!data.empty())
            {
                for (const auto &header : headerNames)
                {
                    cout << setw(colWidth) << header;
                }
                cout << endl;
                for (const auto &entry : data)
                {
                    for (const auto &pair : entry)
                    {
                        cout << setw(colWidth) << pair.second;
                    }
                    cout << endl;
                }
            }
            break;
        case 'c':
            // Print column names
            if (!data.empty())
            {
                for (const auto &pair : data[0])
                {
                    cout << pair.first << endl;
                }
            }
            break;
        case 's':
        {
            // Search
            string columnName;
            string value;
            string input;
            getline(cin, input);
            istringstream iss(input);
            int quoteCount = count(input.begin(), input.end(), '\"');
            if (quoteCount == 4)
            {
                getline(iss, columnName, '\"');
                getline(iss, columnName, '\"');
                getline(iss, value, '\"');
                getline(iss, value, '\"');
            }
            else if (quoteCount == 2 && input[1] == '\"')
            {
                getline(iss, columnName, '\"');
                getline(iss, columnName, '\"');
                iss >> value;
            }
            else if (quoteCount == 2 && input[1] != '\"')
            {
                iss >> columnName;
                getline(iss, value, '\"');
                getline(iss, value, '\"');
            }
            else
            {
                iss >> columnName >> value;
            }

            bool found = false;
            bool c = false;
            bool columnExists = false;
            for (const auto &entry : data)
            {

                for (const auto &pair : entry)
                {
                    if (pair.first == columnName)
                    {
                        columnExists = true;
                        break;
                    }
                }
                if (columnName == "*" || columnExists)
                {

                    for (const auto &pair : entry)
                    {
                        if ((columnName == "*" || pair.first == columnName) && pair.second == value)
                        {
                            if (!c)
                            {
                                c = true;
                                for (const auto &header : headerNames)
                                {
                                    cout << setw(colWidth) << header;
                                }
                                cout << endl;
                            }
                            for (const auto &pair : entry)
                            {
                                cout << setw(colWidth) << pair.second;
                            }
                            cout << endl;
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (!found)
            {
                if (columnExists || columnName == "*")
                {
                    cout << "No results" << endl;
                }
                else
                {
                    cout << "Invalid column \"" << columnName << "\"" << endl;
                }
            }
            break;
        }
        case 'm':
        case 'a':
        case 'x':
        {
            // Calculate
            string columnName;
            string input;
            getline(cin, input);
            istringstream iss(input);
            if (count(input.begin(), input.end(), '\"') >= 2)
            {
                getline(iss, columnName, '\"');
                getline(iss, columnName, '\"');
            }
            else
            {
                iss >> columnName;
            }

            double minVal = DBL_MAX;
            double maxVal = DBL_MIN;
            double sum = 0.0;
            double numValue = 0.0;
            int count = 0;
            bool ct = false;
            bool columnExists = false;
            for (const auto &entry : data)
            {

                for (const auto &pair : entry)
                {
                    if (pair.first == columnName)
                    {
                        columnExists = true;
                        break;
                    }
                }
                if (columnExists)
                {

                    for (const auto &pair : entry)
                    {
                        if (pair.first == columnName)
                        {
                            try
                            {
                                numValue = stod(pair.second);
                            }
                            catch (const invalid_argument &)
                            {
                                ct = true;
                            }
                        }
                    }
                    if (ct)
                    {
                        ct = false;
                        continue;
                    }
                    minVal = min(minVal, numValue);
                    maxVal = max(maxVal, numValue);
                    sum += numValue;
                    ++count;
                }
            }
            if (count > 0)
            {
                if (userCommand == "min")
                {
                    cout << "The min for col \"" << columnName << "\" = " << minVal << endl;
                }
                else if (userCommand == "max")
                {
                    cout << "The max for col \"" << columnName << "\" = " << maxVal << endl;
                }
                else if (userCommand == "avg")
                {
                    cout << "The avg for col \"" << columnName << "\" = " << sum / count << endl;
                }
            }
            else if (columnExists)
            {
                cout << "The " << userCommand << " for col \"" << columnName << "\" = N/A" << endl;
            }
            else
            {
                cout << "Invalid column \"" << columnName << "\" " << endl;
            }
            break;
        }
        default:
            cout << "Invalid command" << endl;
            cin.ignore(100000, '\n');
        }
        cout << "\nEnter a command or \"help\" for a command list:" << endl;
    }

    return 0;
}

vector<string> tokenize(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

string stripQuotes(const string &s)
{
    string result;
    for (char c : s)
    {
        if (c != '"')
        {
            result += c;
        }
    }
    return result;
}
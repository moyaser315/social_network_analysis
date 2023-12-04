#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
using namespace std;


string strip_line(const string &line) {
    size_t first_non_space = line.find_first_not_of(" \t\n\r");
    size_t last_non_space = line.find_last_not_of(" \t\n\r");

    if (first_non_space == string::npos) {
        return "";
    }

    return line.substr(first_non_space, last_non_space - first_non_space + 1);
}

class Node
{
public:
    string name;
    string value;
    vector<Node *> children;

    Node(const string &name, const string &value = "") : name{name}, value{value}
    {
        if (name == "")
        {
            throw runtime_error("Node must have a name.");
        }
    }
};

class XML
{
private:
    Node *root{nullptr};

    string read_file(const string &file_path)
    {
        // open file
        ifstream file(file_path);
        if (!file.is_open()) {  // if error opening file
            throw runtime_error("FileSystem Error: Unable to read the file");
        }

        // read lines
        string txt {""};
        string line;
        while (getline(file, line)) {
            txt += strip_line(line) + " ";
        }

        // close file
        file.close();

        txt = regex_replace(txt, regex(">(\\s*)"), ">\n");
        txt = regex_replace(txt, regex("(\\s*)</"), "\n</");
        return txt;
    }

public:
    XML(const string &file_path)
    {

    }

    string read(const string &path) {
        return read_file(path);
    }
};

int main()
{
    cout << XML("").read("./sample.xml") << endl;
}
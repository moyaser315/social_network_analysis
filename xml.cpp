#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
using namespace std;

struct Node
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

    string to_string(const bool &prettify = true, const int &pre_spaces = 0)
    {
        string res{""};
        if (prettify)
        {
            for (int i = 0; i < pre_spaces; i++)
            {
                res += " ";
            }
        }

        if (name != "__TEXT__")
        {

            res += "<" + name + ">";
            if (prettify)
            {
                res += "\n";
            }

            for (const auto child : children)
            {
                res += child->to_string(prettify, pre_spaces + 4);
            }

            if (prettify)
            {
                res += "\n";
                for (int i = 0; i < pre_spaces; i++)
                {
                    res += " ";
                }
            }

            res += "</" + name + ">";
            if (prettify) {
                res += "\n";
            }
        }
        else
        {
            res += value;
        }

        return res;
    }
};

class XmlParser
{
private:
    static bool is_opening_tag(const string &token)
    {
        return regex_match(token, regex("<[a-zA-Z]+>"));
    }

    static bool is_tag(const string &token)
    {
        return regex_match(token, regex("<[a-zA-Z]+>|</[a-zA-Z]+>"));
    }

    static bool is_closing_tag(const string &token)
    {
        return regex_match(token, regex("</[a-zA-Z]+>"));
    }

    static bool is_text_node(const string &token)
    {
        return regex_match(token, regex("[_a-zA-Z0-9.,?&\"'\\s]+"));
    }

    static string strip_line(const string &line)
    {
        size_t first_non_space = line.find_first_not_of(" \t\n\r");
        size_t last_non_space = line.find_last_not_of(" \t\n\r");

        if (first_non_space == string::npos)
        {
            return "";
        }

        return line.substr(first_non_space, last_non_space - first_non_space + 1);
    }

    static vector<string> read_file(const string &xml_file_path)
    {
        // open file
        ifstream file;file.open(xml_file_path);
        if (!file.is_open())
        { // if error opening file
            throw runtime_error("FileSystem Error: Unable to read the file");
        }

        // read lines
        string txt{""};
        string line;
        while (getline(file, line))
        {
            txt += strip_line(line) + " ";
        }

        // close file
        file.close();

        txt = regex_replace(txt, regex(">(\\s*)"), ">\n");
        txt = regex_replace(txt, regex("(\\s*)</"), "\n</");

        istringstream txt_stream(txt);
        vector<string> tokens;
        while (getline(txt_stream, line))
        {
            tokens.push_back(line);
        }

        return tokens;
    }

    static void clear(Node *node)
    {
        if (!node)
        {
            return;
        }

        for (const auto n : node->children)
        {
            clear(n);
        }

        delete node;
    }

    static pair<Node *, int> create_node(const vector<string> &tokens, const int &token_idx)
    {
        if (token_idx >= (int)tokens.size())
        {
            return make_pair(nullptr, -1);
        }

        string token = tokens[token_idx];

        if (is_text_node(token))
        {
            Node *node{new Node{"__TEXT__", token}};
            return make_pair(node, token_idx + 1);
        }

        if (is_opening_tag(token))
        {
            string node_name{token.substr(1, (int)token.size() - 2)};
            Node *node{new Node{node_name}};

            int next_token_idx{token_idx + 1};
            while (is_opening_tag(tokens[next_token_idx]) || is_text_node(tokens[next_token_idx]))
            {
                pair<Node *, int> res;
                try
                {
                    res = create_node(tokens, next_token_idx);
                }
                catch (const exception &e)
                {
                    clear(node);
                    throw e;
                }

                node->children.push_back(res.first);
                next_token_idx = res.second;
            }

            if (is_closing_tag(tokens[next_token_idx]))
            {
                string closing_node{tokens[next_token_idx].substr(2, (int)tokens[next_token_idx].size() - 3)};
                if (node_name == closing_node)
                {
                    return make_pair(node, next_token_idx + 1);
                }
                else
                {
                    clear(node);
                    throw runtime_error("Parsing Error: Closing tag doesn't match.");
                }
            }
            else
            {
                clear(node);
                throw runtime_error("Parsing Error: Closing tag couldn't be found");
            }
        }
        else
        {
            throw runtime_error("Parsing Error: Reached a closing tag that shouldn't be there");
        }
    }

public:
    static Node *parse(const string &file_path)
    {
        if (file_path == "")
        {
            throw runtime_error("Input Error: must enter a path for a file");
        }

        vector<string> tokens{read_file(file_path)};

        pair<Node *, int> res{create_node(tokens, 0)};
        return res.first;
    }
};

class XmlTree
{
private:
    Node *root{nullptr};

    void clear(Node *node)
    {
        if (!node)
        {
            return;
        }

        for (const auto n : node->children)
        {
            clear(n);
        }

        delete node;
    }

public:
    XmlTree(const string &file_path)
    {
        root = XmlParser::parse(file_path);
    }

    ~XmlTree()
    {
        clear(root);
    }

    string to_string(const bool &prettify = true) {
        return root->to_string(prettify);
    }
};

int main()
{
    XmlTree xml{".\\projprojsample.xml"};
    cout << xml.to_string(false) << endl;
}
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
using namespace std;

class XmlNode
{
public:
    string name;

    XmlNode(const string &name) : name(name)
    {
        if (name == "")
        {
            throw runtime_error("Node must have a name.");
        }
    }
    virtual string to_string(const bool &prettify = true, const int &pre_spaces = 0) = 0;
};

class XmlTag : public XmlNode
{
public:
    string value;
    vector<XmlNode *> children;
    XmlTag(const string &name) : XmlNode(name) {}

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
        if (prettify)
        {
            res += "\n";
        }

        return res;
    }
};

class TextNode : public XmlNode
{
public:
    string value;
    TextNode(const string &value) : XmlNode("__TEXT__"), value{value} {}

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

        res += value;

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
        ifstream file(xml_file_path);
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

    static void clear(XmlNode *node)
    {
        if (!node)
        {
            return;
        }

        XmlTag *tag{dynamic_cast<XmlTag *>(node)};
        if (tag)
        {
            for (const auto n : tag->children)
            {
                clear(n);
            }
        }

        delete node;
    }

    static pair<XmlNode *, int> create_node(const vector<string> &tokens, const int &token_idx)
    {
        if (token_idx >= (int)tokens.size())
        {
            return make_pair(nullptr, -1);
        }

        string token = tokens[token_idx];

        if (is_text_node(token))
        {
            TextNode *node{new TextNode{token}};
            return make_pair(node, token_idx + 1);
        }

        if (is_opening_tag(token))
        {
            string node_name{token.substr(1, (int)token.size() - 2)};
            XmlTag *node{new XmlTag{node_name}};

            int next_token_idx{token_idx + 1};
            while (is_opening_tag(tokens[next_token_idx]) || is_text_node(tokens[next_token_idx]))
            {
                pair<XmlNode *, int> res;
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
    static XmlTag *parse(const string &file_path)
    {
        if (file_path == "")
        {
            throw runtime_error("Input Error: must enter a path for a file");
        }

        vector<string> tokens{read_file(file_path)};

        pair<XmlNode *, int> res{create_node(tokens, 0)};
        XmlTag *tag{dynamic_cast<XmlTag *>(res.first)};
        if (!tag)
        {
            throw runtime_error("Parsing Error: The root node isn't an XML tag");
        }

        return tag;
    }
};

class XmlTree
{
private:
    XmlTag *root{nullptr};

    void clear(XmlNode *node)
    {
        if (!node)
        {
            return;
        }

        XmlTag *tag{dynamic_cast<XmlTag *>(node)};
        if (tag)
        {
            for (const auto n : tag->children)
            {
                clear(n);
            }
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

    string to_string(const bool &prettify = true)
    {
        return root->to_string(prettify);
    }
};

int main()
{
    XmlTree xml{"./sample.xml"};
    cout << xml.to_string(false) << endl;
}
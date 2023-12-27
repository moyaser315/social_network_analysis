#include "./parse.h"

/// @brief strips the line from every leading and trailing non letters charachters
/// @param line
/// @return a string without white spaces
/// time complexity :
string strip_line(const string &line)
{
    size_t first_non_space = line.find_first_not_of(" \t\n\r");
    size_t last_non_space = line.find_last_not_of(" \t\n\r");

    if (first_non_space == string::npos)
    {
        return "";
    }

    return line.substr(first_non_space, last_non_space - first_non_space + 1);
}

/// @brief takes the file parse it and return it as a line
/// @param fpath the path of the file to parse it
/// @param unmini // the file as an xml without any stripped spaces will be passed by ref
/// @param sp // the string with spaces in values
/// @return  // the string without any spaces at all to take less computtion power while building tree
/// time complexity :
string toline(string fpath, string &unmini, string &sp)
{
    // open file
    ifstream file;
    file.open(fpath);

    string res = "", line, nonspace = "";
    while (getline(file, line))
    {
        unmini += line;
        res += strip_line(line);
    }
    file.close();
    sp = res;
    for (int i = 0; i < res.size(); i++)
    {
        if (res[i] == ' ')
            continue;
        nonspace += res[i];
    }
    return nonspace;
}

/// @brief gets the name of the tag
/// @param line the xml string to take the tag out of
/// @param st the position to start parsing from passed by refrence to continue parsing after the tag
/// @param f passed as refrence to specifify if it's closing or opening tag 1 : closing ,0  : opening
/// @return string containing tag name
/// time complexity :
string get_tag(string line, int &st, bool &f)
{
    int i = st, be = st + 1, cnt = 0;
    f = line[i + 1] == '/' ? 1 : 0;
    while (line[i] != '>')
    {
        i++, cnt++;
    }

    st += (cnt + 1);
    // if it's a closing tag we need to ignore "/" as it satrts by </
    // so we will substring from index 2 </x-->2 and end at len of word excluding /
    // if it's opening tag we start from index 1 and end at len of word
    return (f ? line.substr(be + 1, cnt - 2) : line.substr(be, cnt - 1));
}

/// @brief gets the value of the tag
/// @param line the xml string to take the tag vlaue out of
/// @param st the position to start parsing from passed by refrence to continue parsing after the tag
/// @return string containg value of tag
/// time complexity :
string get_value(string line, int &st)
{

    int i = st, be = st, cnt = 0;
    while (line[i] != '<')
    {
        i++, cnt++;
    }
    st += (cnt);

    return line.substr(be, cnt);
}

/// @brief parses the string and builds a xml tree to use later
/// @param xml the string to parse
/// @param tag the tree to store the tags
/// @param st where to start parsing pass 0 in the first call
void build_tree(string &xml, tree_mul *&tag, int &st)
{
    int n = xml.length() - 1;
    bool f = 0;
    string tag_name, value;
    // if the tree is empty push the first tag
    if (tag == nullptr)
    {

        tag_name = get_tag(xml, st, f);
        tag = new tree_mul(tag_name);
        // keep a frequncy array for knowing what's missing
        missing(tag_name, 0);
        build_tree(xml, tag, st);
        return;
    }
    while (st < n - 1)
    {
        if (xml[st] == '<')
        {
            tag_name = get_tag(xml, st, f);
            if (f)
            {
                missing(tag_name, 1);
                if (tag->get_data() != tag_name)
                {
                    // check number of errors for confirmation
                    errors++;
                }
                return;
            }
            else
            {
                missing(tag_name, 0);
                tree_mul *child = new tree_mul(tag_name);
                tag->insert(child);
                build_tree(xml, child, st);
            }
        }
        else
        {

            value = get_value(xml, st);
            tag->insert(value);
        }
    }
}

/// @brief takes the tagname and tracks how many times it was opened or closed
/// @param tagname the tag to track
/// @param f know if it's opening or closing ,opening = +1 ,closing = -1,the best case is that all tags are valued 0
void missing(string tagname, bool f)
{
    for (int i = 0; i < 11; i++)
    {
        if (ids[i] == tagname)
        {
            if (f)
                freq[i]--;
            else
                freq[i]++;
            return;
        }
    }
}
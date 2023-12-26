#include "./parse.h"

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

string toline(string fpath)
{
    // open file
    ifstream file;
    file.open(fpath);

    string res = "", line, nonspace = "";
    while (getline(file, line))
    {
        res += strip_line(line);
    }
    file.close();
    for (int i = 0; i < res.size(); i++)
    {
        if (res[i] == ' ')
            continue;
        nonspace += res[i];
    }
    return nonspace;
}


string get_tag(string line, int &st, bool &f)
{
    // f is for checking it's closing
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

void build_tree(string &xml, tree_mul<string> *&tag, int &st)
{
    int n = xml.length() - 1;
    bool f = 0;
    string tag_name, value;
    // pushing the opening tag
    if (tag == nullptr)
    {
        tag_name = get_tag(xml, st, f);
        tag = new tree_mul<string>(tag_name);

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

                // knows if ther's mismatch in closing tags
                if (tag->get_data() != tag_name)
                {
                    errors++;
                    
                }
                return;
                
            }
            else
            {
        
                tree_mul<string> *child = new tree_mul<string>(tag_name);
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


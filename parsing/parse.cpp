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


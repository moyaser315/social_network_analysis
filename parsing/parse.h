#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include "../data_structres/tree.h"
#include <regex>
#include <fstream>

using namespace std;

string strip_line(const string &line);
string toline(string fpath, string &unmini, string &sp);

string get_tag(string line, int &st, bool &f);
string get_value(string line, int &st);

void build_tree(string &xml, tree_mul *&tag, int &st);
void missing(string tagname, bool f);

string ids[] = {"users", "user", "topics", "topic", "posts", "post", "name", "id", "followers", "follower", "body"};
int freq[11] = {0};
int errors;

#endif
#ifndef parse_H
#define parse_H
#include <iostream>
#include "../data_structres/tree.h"
#include <regex>
#include <fstream>

using namespace std;


string strip_line(const string &line);// to delete space
string toline(string fpath);// return xml file as string
string get_tag(string line,int &st,bool &f);// get tag name
string get_value(string line,int &st);// get value of the tag like name or id
void build_tree(string &xml, tree_mul<string> *&tag, int &st);// xml to parse ,tag for building tree , st to trace where i stopped parsing

int errors;
#endif
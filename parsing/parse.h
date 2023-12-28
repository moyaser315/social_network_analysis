#ifndef PARSE_H
#define PARSE_H

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "../xml_tree/tree.h"

std::string strip_line(const std::string &line);
std::string toline(std::string fpath, std::string &unmini, std::string &sp);

std::string get_tag(std::string line, int &st, bool &f);
std::string get_value(std::string line, int &st);

void build_tree(std::string &xml, tree_mul *&tag, int &st);
void missing(std::string tagname, bool f);

extern std::string IDs[];
extern int freq[11];

extern int errors;

#endif

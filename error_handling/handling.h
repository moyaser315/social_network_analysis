#ifndef HANDLING_H
#define HANDLING_H

#include <iostream>
#include <stack>
#include <string>

#include "../parsing/parse.h"

void detect(std::string &xml);
int get_freq(std::string s);
void correct(std::string &xml);

extern std::stack<std::string> tags;
extern std::stack<std::string> err;
extern std::vector<int> positions;

#endif

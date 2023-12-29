#ifndef HANDLING_H
#define HANDLING_H
#include "../parsing/parse.h"
#include <stack>
#include <iostream>
using namespace std;

void detect(string &xml);
int get_freq(std::string s);

stack<string> tags;
stack<string> err;
vector<int> positions;

#endif
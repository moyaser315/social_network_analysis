#ifndef HANDLING_H
#define HANDLING_H
#include "../parsing/parse.h"
#include <stack>
#include <iostream>
using namespace std;

void detect(string &xml);


stack<string> tags;
stack<string> err;
vector<int> positions;

#endif
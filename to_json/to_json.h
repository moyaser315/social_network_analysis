#pragma once
#include <iostream>
#include <../xml_tree/tree.h>
#include <vector>

using namespace std;

class XmlToJsonConverter
{
public:
    static string to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr);
};

class TextTagConverter : public XmlToJsonConverter
{
public:
    static string to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr);
};

class ParentTagConverter : public XmlToJsonConverter
{
private:
    static vector<pair<string, vector<tree_mul *>>> get_children_type(const tree_mul *node);
    static bool same_type(const vector<pair<string, vector<tree_mul *>>> &types, const string &parent_type);
    static bool is_text_tag(const tree_mul *node);

public:
    static string to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr);
};

class XmlToJson
{
public:
    static string to_json(tree_mul *root, const bool &prettify = true);
};

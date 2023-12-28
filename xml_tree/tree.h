#ifndef TREE_H
#define TREE_H
#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
using namespace std;

class tree_mul{
    private : 
        string data;
        vector<tree_mul * > children;
        bool closed;
        bool text;
        
    public:
        tree_mul(string data, bool is_text = false);
        void insert(tree_mul *child);
        void insert(string data);
        string get_data();
        void level(string fpath);
        bool is_leaf();
        string to_string(const bool &prettify = true, const int &pre_tabs = 0);
        void set_closed(bool t);
        bool is_text();
        vector<tree_mul *> get_children();

        friend class XmlToJsonConverter;
        friend class TextTagConverter;
        friend class ParentTagConverter;
};
#endif

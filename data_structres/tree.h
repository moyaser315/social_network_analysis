#ifndef TREE_H
#define TREE_H
#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
using namespace std;

template<typename T>
class tree_mul{
    private : 
        T data;
        vector<tree_mul * > children;
    public:
        tree_mul(T data);
        void insert(tree_mul<T>* child);
        void insert(T data);
        T get_data();
        void level(string fpath); // testing if the tree is correct


};
#endif
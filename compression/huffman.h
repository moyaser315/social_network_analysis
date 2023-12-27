#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

struct Node
{
    char data;
    int freq;
    string code;
    Node *left, *right;

    Node(int freq) : freq(freq), left(nullptr), right(nullptr) {}
};

// for priority queue to make minimum heap
struct Comparison
{
    bool operator()(Node *left, Node *right)
    {
        return (left->freq) > (right->freq);
    }
};

Node *st;
vector<Node *> char_arr;
priority_queue<Node *, vector<Node *>, Comparison> huffman_tree; // the heap to construct
string enc, decoded_string;

// build huffman nodes
void arrange_freq(string fpath);
void build();
void generate_codes(Node *cur, string code);

#endif
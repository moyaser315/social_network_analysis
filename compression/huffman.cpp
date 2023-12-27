#include "huffman.h"

/// @brief internal function to get the frequency of each letter
/// @param fpath 
void arrange_freq(string fpath)
{

    // initializing charachter arrays for taking frequency
    Node *temp = nullptr;
    for (int i = 0; i < 128; i++)
    {
        temp = new Node(0);
        temp->data = i;
        char_arr.push_back(temp);
    }

    ifstream file;
    file.open(fpath);
    char c;
    file.get(c);

    while (!file.eof())
    {
        char_arr[c]->freq++;
        file.get(c);
    }
    file.close();
    // making the minimum heap for building ordered tree
    for (int i = 0; i < 128; i++)
    {
        if (char_arr[i]->freq > 0)
            huffman_tree.push(char_arr[i]);
    }
}

/*
huffman coding works as following :
1/ we need to assign each charachter to a binary code
2/ we want the most frequent to have the shortest binary code
3/ so the lowest frequency must be at the bottom
4/ in our huffman_tree prior queue we have the minimum frequency ready to be popped
5/ our st will be at the bottom after we constructed our tree
*/

/// @brief building huffman tree
void build()
{
    Node *l, *r;
    priority_queue<Node *, vector<Node *>, Comparison> temp(huffman_tree);
    while (temp.size() > 1)
    {
        l = temp.top();
        temp.pop();
        r = temp.top();
        temp.pop();

        st = new Node((l->freq + r->freq));
        st->left = l;
        st->right = r;

        temp.push(st);
    }
}
// for generating code : we assign 0 to left and 1  to right

/// @brief generating binary code for each charachter
/// @param cur the node we are at 
/// @param code the code to be assigned to it
void generate_codes(Node *cur, string code)
{
    if (!cur)
        return;

    if (!cur->left && !cur->left)
    {
        cur->code = code;
        return;
    }
    generate_codes(cur->left, "0"+code );
    generate_codes(cur->right, "1"+code);
}
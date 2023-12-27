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


/// @brief this to convert biary represented as string to value so that it would be smaller in size
/// @param s the binary code
/// @return an integer representing it
int bin_conv(string s)
{

    // nullptr is the '\0' but when it was '\0' it didn't work
    // 2 is base 2 to specify it's binary
    return stoi(s, nullptr, 2);
}

// this key is for decoding it would be apppended in the begining

/// @brief generating key for decoding, it would be apppended in the begining
void construct_key()
{
    string temp = "";
    // firstly we will add the size of the tree to reconstruct it
    priority_queue<Node *, vector<Node *>, Comparison> codes(huffman_tree);
    enc += (char)huffman_tree.size();
    Node *cur;
    while (!codes.empty())
    {
        cur = codes.top();
        enc += cur->data; // this the charachter
        // we will make each char 128 bits as we have 128 char used
        // if we know which chars would be input we could have made it smaller
        // we will make the MSB all 0s and put 1 as seperator then the code
        temp.assign(127 - cur->code.size(), '0');
        temp += '1';
        temp += cur->code;
        enc += (char)bin_conv(temp.substr(0, 8));
        // as each char in ascii is 8 bits we will divide it to 8 bits
        for (int i = 0; i < 15; i++)
        {
            temp = temp.substr(8);
            enc += (char)bin_conv(temp.substr(0, 8));
        }
        codes.pop();
    }
}

/// @brief compress the file
/// @param fpath 
void encode(string fpath)
{
    construct_key();

    // reading file to compress
    string line;
    ifstream file;
    file.open(fpath);
    char c;
    while (!file.eof())
    {
        line += char_arr[c]->code;
        // encoding every 8 bit
        while (line.length() > 8)
        {
            enc += (char)bin_conv(line.substr(0, 8));
            line = line.substr(8);
        }
        file.get(c);
    }
    file.close();

    // seeing if there are remaining non-encoded bits
    int x = 8 - line.size();
    if (x < 8)
        line.append(x, '0');
    enc += (char)bin_conv(line.substr(0, 8));
    enc += (char)x;
}


/// @brief organizing the compressin process can be discarded by calling the functions in order
/// @param fpath 
void compress(string fpath)
{
    arrange_freq(fpath);

    build();
    generate_codes(st, "");
    encode(fpath);
}

/*******************general functions********************************/
/// @brief for writing the file
/// @param fpath the path to output to
/// @param f outputting : (1: decompressed) , (0:compressed)
void write_file(string fpath,bool f)
{
    fstream file;
    file.open(fpath, ios::out | ios::binary);

    file << (f ? decoded_string:enc);

    file.close();
}
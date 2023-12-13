#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define mo                            \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);
template<typename T>
class binaryTree
{
private:
    binaryTree *left{};
    binaryTree *right{};
    T data{};

public:
    binaryTree(T data):data(data),left(nullptr),right(nullptr){}
    void set_right(T data){
        right=binaryTree(data);
    }
    void set_left(T data){
        left=binaryTree(data);
    }
    binaryTree * get_right(){
        return right;
    }
    binaryTree * get_left(){
        return left;
    }
    bool is_leaf(){
        return !(right || left);
    }


};

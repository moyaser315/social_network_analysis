#include "./tree.h"


template<typename T>
tree_mul<T>::tree_mul(T data):data(data){}

template<typename T>
void tree_mul<T>::insert(T data){
    this->children.push_back(new tree_mul(data));
}

template<typename T>
void tree_mul<T>::insert(tree_mul* child){
    this->children.push_back(child);
}

template<typename T>
T tree_mul<T>::get_data(){
    return this->data;

}

template<typename T>
void tree_mul<T>::level(string fpath){
    if(!this) return ;
    std::ofstream outputFile(fpath);
    queue<tree_mul<T>*> q;
    tree_mul<T>* cur = this;
    q.push(cur);
    q.push(nullptr);
    while(!q.empty()){

        //to make break in levels
        if (q.front() == nullptr) {
           outputFile  <<"\n\n\n" ;
            q.pop();
            if (!q.empty()) {
                q.push(nullptr);
            }else{
                break;
            }
        }

        cur =q.front();
        q.pop();

        //to ignore lorem ipsom and only output a letter to make the tree readable
        if(cur->data[0]=='L'){
            outputFile<<"L\t";
        }
        else outputFile << cur->data<<"\t";


        for(auto & child:(cur->children)){
            q.push(child);
        }

    }
}


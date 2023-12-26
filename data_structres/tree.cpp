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
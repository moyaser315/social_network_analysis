#include "./tree.h"


tree_mul::tree_mul(string data, bool is_text) : data(data), text(is_text) {}

void tree_mul::insert(string data){
    this->children.push_back(new tree_mul(data, true));
}

void tree_mul::insert(tree_mul* child){
    this->children.push_back(child);
}

string tree_mul::get_data(){
    return this->data;

}

void tree_mul::level(string fpath){
    if(!this) return ;
    std::ofstream outputFile(fpath);
    queue<tree_mul*> q;
    tree_mul* cur = this;
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

bool tree_mul::is_leaf() { return this->children.empty(); }

void tree_mul::set_closed(bool t) { this->closed = t; }

string tree_mul::to_string(const bool &prettify, const int &pre_tabs)
{
    string res{""};

    if (this->is_leaf())
    { // text node
        if (this->text)
        {
            if (prettify)
            {
                for (int i = 0; i < pre_tabs - 1; i++)
                {
                    res += "\t";
                }

                for (int i = 0; i < 8; i++)
                {
                    res += " ";
                }
            }

            res += this->data;
        }
        else
        {
            if (prettify)
            {
                for (int i = 0; i < pre_tabs; i++)
                {
                    res += "\t";
                }
            }

            res += "<" + this->data + ">";
            if (prettify)
            {
                res += "\n";
                for (int i = 0; i < pre_tabs; i++)
                {
                    res += "\t";
                }
            }

            res += "</" + this->data + ">";
            if (prettify) {
                res += "\n";
            }
        }
    }
    else
    {

        if (prettify)
        {
            for (int i = 0; i < pre_tabs; i++)
            {
                res += "\t";
            }
        }

        res += "<" + this->data + ">";
        if (prettify)
        {
            res += "\n";
        }

        for (const auto child : this->children)
        {
            res += child->to_string(prettify, pre_tabs + 1);
        }

        if (prettify)
        {
            res += "\n";
            for (int i = 0; i < pre_tabs; i++)
            {
                res += "\t";
            }
        }

        res += "</" + this->data + ">";
        if (prettify)
        {
            res += "\n";
        }
    }

    return res;
}

vector<tree_mul *> tree_mul::get_children() {
    return this->children;
}
#include "./to_json.h"

string XmlToJsonConverter::to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr)
{
    string res{""};

    if (prettify)
    {
        for (int i{0}; i < pre_tabs; i++)
        {
            res += "\t";
        }
    }

    if (!in_arr) // if in dictionary
    {
        res += "\"" + node->data + "\": "; // put the key
    }

    return res;
};

string TextTagConverter::to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr)
{
    return XmlToJsonConverter::to_json(node, prettify, pre_tabs, in_arr) + "\"" + node->children[0]->data + "\"";
}

vector<pair<string, vector<tree_mul *>>> ParentTagConverter::get_children_type(const tree_mul *node)
{
    vector<pair<string, vector<tree_mul *>>> types;
    for (const auto &child : node->children)
    {
        string type{child->data};
        int idx{-1};
        for (int i{0}; i < (int)types.size(); i++)
        {
            if (types[i].first == type)
            {
                idx = i;
                break;
            }
        }

        if (idx == -1)
        {
            types.push_back(make_pair(type, vector<tree_mul *>{child}));
        }
        else
        {
            types[idx].second.push_back(child);
        }
    }

    return types;
}

bool ParentTagConverter::same_type(const vector<pair<string, vector<tree_mul *>>> &types, const string &parent_type)
{
    return (int)types.size() == 1 && parent_type.substr(0, (int)parent_type.size() - 1) == types[0].first;
}

bool ParentTagConverter::is_text_tag(const tree_mul *node)
{
    bool res = false;
    try {
        res = node->children.at(0)->is_leaf();
    } catch (const exception &e) {
        // nothing
    }

    return res;
}


string ParentTagConverter::to_json(const tree_mul *node, const bool &prettify, const int &pre_tabs, const bool &in_arr)
{
    string res{XmlToJsonConverter::to_json(node, prettify, pre_tabs, in_arr)};

    // check if children of same type
    vector<pair<string, vector<tree_mul *>>> types{get_children_type(node)};
    if (same_type(types, node->data))
    {
        res += "[";
        if (prettify)
        {
            res += "\n";
        };

        int i{0};
        for (const auto &child : node->children)
        {
            if (i)
            {
                res += ",";
                if (prettify)
                {
                    res += "\n";
                }
            }

            if (is_text_tag(child))
            {
                res += TextTagConverter::to_json(child, prettify, pre_tabs + 1, true);
            }
            else
            {
                res += to_json(child, prettify, pre_tabs + 1, true);
            }

            i++;
        }

        if (prettify)
        {
            res += "\n";
            for (int j = 0; j < pre_tabs; j++)
            {
                res += "\t";
            }
        }

        res += "]";
    }
    else
    {
        res += "{";
        if (prettify)
        {
            res += "\n";
        };

        int i{0};
        for (const auto &type : types)
        {
            if (i)
            {
                res += ",";
                if (prettify)
                {
                    res += "\n";
                }
            }

            if ((int)type.second.size() == 1)
            {
                if (is_text_tag(type.second[0]))
                {
                    res += TextTagConverter::to_json(type.second[0], prettify, pre_tabs + 1, false);
                }
                else
                {
                    res += ParentTagConverter::to_json(type.second[0], prettify, pre_tabs + 1, false);
                }
            }
            else
            {
                if (prettify)
                {
                    for (int j = 0; j < pre_tabs + 1; j++)
                    {
                        res += "\t";
                    }
                }

                res += "\"" + type.first + "\": [";
                if (prettify)
                {
                    res += "\n";
                }

                int j{0};
                for (const auto &child : type.second)
                {
                    if (j)
                    {
                        res += ",";
                        if (prettify)
                        {
                            res += "\n";
                        }
                    }

                    if (is_text_tag(child))
                    {
                        res += TextTagConverter::to_json(child, prettify, pre_tabs + 2, true);
                    }
                    else
                    {
                        res += ParentTagConverter::to_json(child, prettify, pre_tabs + 2, true);
                    }

                    j++;
                }

                if (prettify)
                {
                    res += "\n";
                    for (int j = 0; j < pre_tabs + 1; j++)
                    {
                        res += "\t";
                    }
                }

                res += "]";
            }

            i++;
        }

        if (prettify)
        {
            res += "\n";
            for (int j = 0; j < pre_tabs; j++)
            {
                res += "\t";
            }
        }

        res += "}";
    }

    return res;
}

string XmlToJson::to_json(tree_mul *root, const bool &prettify)
{
    string res{"{"};
    if (prettify)
    {
        res += "\n";
    }
    
    res += ParentTagConverter::to_json(root, prettify, 1, false);

    if (prettify)
    {
        res += "\n";
    }
    res += "}";

    return res;
}

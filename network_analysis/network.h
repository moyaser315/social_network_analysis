#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>

#include "../parsing/parse.h"
#include "../xml_tree/tree.h"

struct User {
    int id;
    uint num_of_followers = 0;
    uint num_of_posts = 0;
    std::string user_name = "";
    std::vector<int> followers;
    std::vector<int> following;

    void add_follower(int follower_id);
};


struct Network {

    std::vector<User> users; // edge list
    User most_followed;
    std::vector<int> total_degree;
    bool reconfigured = false;

    void add_edge(User from, User to);
    void add_user(User user) ;
    User get_user(int id);
    User get_most_followed_user();
    void reconfigure_network() ;
    User get_most_active_user() ;
    std::vector<User> get_mutual_followers(int id1, int id2) ;
    std::vector<User> suggest_user_to_follow(int id) ;
};


Network convert_xml_tree_to_graph(tree_mul *&parent) ;
std::vector<std::string> search_by_word_or_topic(tree_mul *&parent, std::string word);

#endif

#include "network.h"

using namespace std;

void User::add_follower(int follower_id) { followers.push_back(follower_id); }

void Network::add_user(User user) {

    if (users.size() == 0)
        most_followed = user;

    else if (user.num_of_followers > most_followed.num_of_followers)
        most_followed = user;

    users.push_back(user);
}

User Network::get_user(int id) { return users[id]; }

User Network::get_most_followed_user() { return most_followed; }

void Network::reconfigure_network() {
    int num_of_users = users.size();
    total_degree.resize(num_of_users);
    fill(total_degree.begin(), total_degree.end(), 0);
    for (int i = 0; i < num_of_users; i++) {
        total_degree[i] += users[i].num_of_followers; // in_degree ;
        // accumlating out_degree ;
        for (auto follower : users[i].followers) {
            users[i].following.push_back(follower);
            total_degree[follower - 1]++;
        }
    }
    reconfigured = true;
}

User Network::get_most_active_user() {
    if (!reconfigured)
        reconfigure_network();

    int argmax, max_degree_so_far = 0;
    for (int i = 0; i < total_degree.size(); i++) {
        if (max_degree_so_far < total_degree[i]) {
            max_degree_so_far = total_degree[i];
            argmax = i;
        }
    }
    return users[argmax];
}

vector<User> Network::get_mutual_followers(int id1, int id2) {
    if (!reconfigured)
        reconfigure_network();

    vector<User> mutual_followers;
    vector<int> user1_followers = users[id1 - 1].followers,
                user2_followers = users[id2 - 1].followers;

    for (auto follower1 : user1_followers) {
        for (auto follower2 : user2_followers) {
            if (follower1 == follower2)
                mutual_followers.push_back(users[follower1 - 1]);
        }
    }

    return mutual_followers;
}

vector<User> Network::suggest_user_to_follow(int id) {

    vector<int> followers = users[id - 1].followers;
    vector<User> suggested;
    for (auto follower : followers) {
        for (auto follower_follower : users[follower - 1].followers) {
            if (follower != follower_follower)
                suggested.push_back(users[follower_follower - 1]);
        }
    }

    return suggested;
}


Network convert_xml_tree_to_graph(tree_mul *&parent) {
    Network network;
    vector<tree_mul *> users = parent->get_children();
    for (const auto user : users) {
        User network_user;
        for (const auto tag : user->get_children()) {
            if (tag->get_data() == "id")
                network_user.id = stoi(tag->get_children()[0]->get_data());
            else if (tag->get_data() == "name")
                network_user.user_name = tag->get_children()[0]->get_data();
            else if (tag->get_data() == "posts")
                network_user.num_of_posts = tag->get_children().size();
            else if (tag->get_data() == "followers") {
                network_user.num_of_followers = tag->get_children().size();
                for (const auto follower : tag->get_children()) {
                    network_user.add_follower(stoi(follower->get_children()[0]
                                                       ->get_children()[0]
                                                       ->get_data()));
                }
            }
        }

        network.add_user(network_user);
    }
    return network;
}

vector<string> search_by_word_or_topic(tree_mul *&parent, string word) {
    vector<tree_mul *> users = parent->get_children();
    vector<string> posts;
    string body;
    bool include = false;
    for (const auto user : users) {
        for (const auto tag : user->get_children()) {
            if (tag->get_data() == "posts") {
                for (const auto post : tag->get_children()) {
                    include = false;
                    for (const auto inner : post->get_children()) {
                        if (inner->get_data() == "body") {
                            body = inner->get_children()[0]->get_data();
                            include |= body.find(word) != string::npos;
                        } else {
                            for (const auto topic : inner->get_children())
                                include |=
                                    topic->get_children()[0]->get_data().find(
                                        word) != string::npos;
                        }
                    }
                    if (include)
                        posts.push_back(body);
                }
            }
        }
    }
    return posts;
}


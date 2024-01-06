#include "netwindow.h"
#include "ui_netwindow.h"


string NetWindow::create_graph_img() {
    string dir_path {QFileInfo(QCoreApplication::applicationFilePath()).absolutePath().toStdString()};
    string dot_file_path {dir_path + "/graph.dot"};
    std::ofstream graph_file(dot_file_path);

    if (!graph_file.is_open()) {
        return "";
    }

    graph_file << "digraph { layout=\"neato\" ";
    for (const auto &user: network.users) {
        graph_file << std::to_string(user.id) + "[label=\"" + user.user_name + " (" + std::to_string(user.id) + ")\";";
        if (user.id == most_followed.id && user.id == most_active.id) {
            graph_file << "color=red";
        } else if (user.id == most_active.id) {
            graph_file << "color=orange";
        } else if (user.id == most_followed.id) {
            graph_file << "color=green";
        }

        graph_file << "] {";
        int i {0};
        for (const auto &follower: user.followers) {
            if (i) {
                graph_file << ",";
            }

            graph_file << follower;
            i++;
        }

        graph_file << "} -> " + std::to_string(user.id) + " ";
    }
    graph_file << "}";
    graph_file.close();

    std::string command {"dot -Tpng " + dot_file_path + " -o " + "graph.png"};
    if (system(command.c_str())) {
        return "";
    }

    return dir_path + "/graph.png";
}

NetWindow::NetWindow(QWidget *parent, tree_mul *xml_root)
    : QMainWindow(parent)
    , ui(new Ui::NetWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("Green: Most Followed  |  Orange: Most Active  |  Red: Most Followed and Most Active");
    setFixedSize(800, 700);

    network = convert_xml_tree_to_graph(xml_root);
    most_followed = network.get_most_followed_user();
    most_active = network.get_most_active_user();

    std::string graph_img_path {create_graph_img()};
    if (graph_img_path == "") {
        QMessageBox::warning(this, "File System Error", "Couldn't create the graph");
        return;
    }

    QPixmap pix(QString::fromStdString(graph_img_path));
    ui->label->setPixmap(pix.scaled(width(), height(), Qt::KeepAspectRatio));
}

NetWindow::~NetWindow()
{
    delete ui;
}

void NetWindow::on_actionget_mutual_followers_triggered()
{
    int id1, id2;

    bool ok;
    bool failed {false};
    // ID 1
    QString userInput = QInputDialog::getText(this, "Mutual Friends", "Enter 1st User ID:", QLineEdit::Normal, "", &ok);
    try {
        id1 = std::stoi(userInput.toStdString());
    } catch (const exception &e) {
        failed = true;
    }
    if (!ok || userInput.isEmpty() || id1 <= 0 || id1 > (int)network.users.size()) {
        failed = true;
    }
    if (failed) {
        QMessageBox::warning(this, "Invalid ID", "Invalid ID for User 1");
        return;
    }

    // ID2
    userInput = QInputDialog::getText(this, "Mutual Friends", "Enter 2nd User ID:", QLineEdit::Normal, "", &ok);
    failed = false;
    try {
        id2 = std::stoi(userInput.toStdString());
    } catch (const exception &e) {
        failed = true;
    }
    if (!ok || userInput.isEmpty() || id2 <= 0 || id2 > (int)network.users.size()) {
        failed = true;
    }
    if (failed) {
        QMessageBox::warning(this, "Invalid ID", "Invalid ID for User 2");
        return;
    }

    vector<User> mutual {network.get_mutual_followers(id1, id2)};
    string res {""};
    if ((int)mutual.size() == 0) {
        res += "They got no mutual users";
    } else {
        int i {1};
        for (const auto &usr: mutual) {
            if (i) {
                res += "\n";
            }

            res += to_string(i) + ") ID: " + to_string(usr.id) + "\tName: " + usr.user_name;
            i++;
        }
    }

    QMessageBox::information(this, "Mutual Friends", QString::fromStdString(res));
}


void NetWindow::on_actionsuggest_followers_triggered()
{
    int id;
    bool ok;
    bool failed {false};
    QString userInput = QInputDialog::getText(this, "Suggest Followers", "Enter User ID:", QLineEdit::Normal, "", &ok);
    try {
        id = std::stoi(userInput.toStdString());
    } catch (const exception &e) {
        failed = true;
    }
    if (!ok || userInput.isEmpty() || id <= 0 || id > (int)network.users.size()) {
        failed = true;
    }
    if (failed) {
        QMessageBox::warning(this, "Invalid ID", "Invalid ID for User 1");
        return;
    }

    vector<User> suggested {network.suggest_user_to_follow(id)};
    string res {""};
    if ((int)suggested.size() == 0) {
        res += "There is no one following. I can't suggest any.";
    } else {
        int i {1};
        for (const auto &usr: suggested) {
            if (usr.id == network.users[id - 1].id) {
                continue;
            }

            if (i) {
                res += "\n";
            }

            res += to_string(i) + ") ID: " + to_string(usr.id) + "\tName: " + usr.user_name;
            i++;
        }
    }

    QMessageBox::information(this, "Suggested Friends", QString::fromStdString(res));
}


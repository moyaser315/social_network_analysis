#include "postswindow.h"
#include "ui_postswindow.h"

PostsWindow::PostsWindow(QWidget *parent, vector<string> *matching_posts)
    : QDialog(parent)
    , ui(new Ui::PostsWindow)
{
    ui->setupUi(this);

    string posts {""};
    int i {0};
    for (const auto &post: *matching_posts) {
        if (i) {
            posts += "\n";
        }

        posts += to_string(i + 1) + "\n---------\n" + post;
        i++;
    }

    label = new QLabel(QString::fromStdString(posts), ui->scrollArea);
    label->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(label);
}

PostsWindow::~PostsWindow()
{
    delete ui;
    delete label;
}

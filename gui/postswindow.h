#ifndef POSTSWINDOW_H
#define POSTSWINDOW_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <vector>
using namespace std;

namespace Ui {
class PostsWindow;
}

class PostsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PostsWindow(QWidget *parent = nullptr, vector<string> *matching_posts = nullptr);
    ~PostsWindow();

private:
    Ui::PostsWindow *ui;
    QLabel *label;
};

#endif // POSTSWINDOW_H

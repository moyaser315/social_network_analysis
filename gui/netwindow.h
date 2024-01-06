#ifndef NETWINDOW_H
#define NETWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QInputDialog>
#include <QStringList>
#include <cstdlib>

#include <../network_analysis/network.h>
#include <../xml_tree/tree.h>
#include <fstream>

namespace Ui {
class NetWindow;
}

class NetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetWindow(QWidget *parent = nullptr, tree_mul *xml_tree = nullptr);
    ~NetWindow();

private slots:
    void on_actionget_mutual_followers_triggered();

    void on_actionsuggest_followers_triggered();

private:
    Ui::NetWindow *ui;
    Network network;
    User most_followed, most_active;

    string create_graph_img();
};

#endif // NETWINDOW_H

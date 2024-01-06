#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT libs
#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QLabel>

// custom windows and helpers
#include <jsonwindow.h>
#include <netwindow.h>
#include <postswindow.h>
#include <helpers.h>

// business logic
#include <../to_json/to_json.h>
#include <../parsing/parse.h>
#include <../error_handling/handling.h>
#include <../xml_tree/tree.h>
#include <../compression/huffman.h>

// std libs
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    tree_mul *root;

private slots:
    void on_actionopen_triggered();

    void on_actionexit_triggered();

    void on_actiondetect_errors_triggered();

    void on_actioncorrect_errors_triggered();

    void on_actionsave_triggered();

    void on_actionconvert_to_json_triggered();

    void on_actionundo_triggered();

    void on_actionredo_triggered();

    void on_actioncompress_triggered();

    void on_actiondecompress_triggered();

    void on_actionminify_triggered();

    void on_actionanalyze_network_triggered();

    void on_actionsearch_posts_triggered();

    void on_actionprettify_triggered();

private:
    Ui::MainWindow *ui;
    JsonWindow *json_window;
    NetWindow *net_window;
    PostsWindow *posts_window;
    string err_msg {""};
    string xml_txt;
    bool corrected {false};

    stack<QString> undo;
    stack<QString> redo;
    QString file_path;

    void locked_state();
    void open_file_state();
    void corrected_state();
};
#endif // MAINWINDOW_H

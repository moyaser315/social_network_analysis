#ifndef JSONWINDOW_H
#define JSONWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <helpers.h>
#include <QRegExp>

namespace Ui {
class jsonwindow;
}

class JsonWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JsonWindow(QWidget *parent = nullptr, const QString &txt = "", QString path = "");
    ~JsonWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionSave_triggered();

private:
    Ui::jsonwindow *ui;
};

#endif // JSONWINDOW_H

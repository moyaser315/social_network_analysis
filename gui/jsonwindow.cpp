#include "jsonwindow.h"
#include "ui_jsonwindow.h"

QString json_path;
JsonWindow::JsonWindow(QWidget *parent, const QString &txt, QString path)
    : QMainWindow(parent)
    , ui(new Ui::jsonwindow)
{
    ui->setupUi(this);
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    QLabel *txt_label = new QLabel(txt);
    txt_label->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(txt_label);
    json_path = path.replace(QRegExp(".xml"), ".json");
}

JsonWindow::~JsonWindow()
{
    delete ui;
}

void JsonWindow::on_actionSave_triggered()  // on trigerring save
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    save(this, json_path.replace(QRegExp(".xml"), ".json"), label->text());
}


void JsonWindow::on_actionExit_triggered()
{
    //empty
}


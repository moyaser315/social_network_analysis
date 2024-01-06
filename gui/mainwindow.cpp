#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::locked_state() {
    ui->actionconvert_to_json->setEnabled(false);
    ui->actioncorrect_errors->setEnabled(false);
    ui->actiondetect_errors->setEnabled(false);
    ui->actionsave->setEnabled(false);
    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actioncompress->setEnabled(false);
    ui->actionminify->setEnabled(false);
    ui->actionsearch_posts->setEnabled(false);
    ui->actionanalyze_network->setEnabled(false);
    ui->actiondecompress->setEnabled(true);
    ui->actionprettify->setEnabled(false);

}

void MainWindow::corrected_state() {
    err_msg = "";
    corrected = true;
    ui->actionconvert_to_json->setEnabled(true);
    ui->actioncorrect_errors->setEnabled(true);
    ui->actiondetect_errors->setEnabled(true);
    ui->actionsave->setEnabled(true);
    ui->actioncompress->setEnabled(true);
    ui->actionanalyze_network->setEnabled(true);
    ui->actionminify->setEnabled(true);
    ui->actionsearch_posts->setEnabled(true);
    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actionprettify->setEnabled(true);
    undo = stack<QString>();
    redo = stack<QString>();
}

void MainWindow::open_file_state() {
    err_msg = "";
    corrected = false;
    ui->actionconvert_to_json->setEnabled(false);
    ui->actioncorrect_errors->setEnabled(true);
    ui->actiondetect_errors->setEnabled(true);
    ui->actionsave->setEnabled(true);
    ui->actioncompress->setEnabled(true);
    ui->actionanalyze_network->setEnabled(false);
    ui->actionminify->setEnabled(false);
    ui->actionsearch_posts->setEnabled(false);
    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actionprettify->setEnabled(false);
    undo = stack<QString>();
    redo = stack<QString>();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLabel *scroll_label = new QLabel("");
    scroll_label->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(scroll_label);
    locked_state();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (json_window)
        delete json_window;

    if (net_window)
        delete net_window;

    if (posts_window)
        delete posts_window;
}

void MainWindow::on_actionopen_triggered()
{
    file_path = QFileDialog::getOpenFileName(this, "Select XML file", QDir::homePath(), "xml file (*.xml)");
    if (file_path.isEmpty()) {
        return;
    }

    QFile file(file_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error!!", "Can't Open File!!");
        return;
    }
    open_file_state();

    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    string unmini;
    try {
        string s = toline(file_path.toStdString(), unmini, xml_txt);
        root = nullptr;
        int st = 0;
        build_tree(xml_txt, root, st);
    } catch (const exception &e) {
        QMessageBox::warning(this, "Error!!", "Can't parse the file");
        return;
    }

    label->setText(QString::fromStdString(xml_txt));
}


void MainWindow::on_actionexit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actiondetect_errors_triggered() // on triggering detect errors
{
    int i{0};
    for (const int &f: freq) {
        if (f < 0) {
            err_msg += "Missing opening tag for: " + IDs[i] + "\n------\n";
        } else if (f > 0) {
            err_msg += "Missing closing tag for: " + IDs[i] + "\n------\n";
        }

        i++;
    }

    detect(xml_txt);
    stack<string> errors_found = err;
    while(!errors_found.empty()) {
        err_msg += "Mismatch for: " + errors_found.top() + "\n------\n";
        errors_found.pop();
    }

    if (err_msg != "") {
        QMessageBox::information(this, "Potential Errors", QString::fromStdString(err_msg));
    } else {
        QMessageBox::information(this, "No Errors Found", "Looks like your xml file is crystal clear");
        corrected_state();
    }

}


void MainWindow::on_actioncorrect_errors_triggered()    // on trigerring correct errors
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    undo.push(label->text());
    correct(xml_txt);
    label->setText(QString::fromStdString(xml_txt));
    ui->actionundo->setEnabled(true);
    QMessageBox::information(this, "Correction", "Errors was corrected");
    corrected_state();
}


void MainWindow::on_actionsave_triggered()  // on trigerring save
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    save(this, file_path, label->text());
}


void MainWindow::on_actionconvert_to_json_triggered()   // on trigerring convert to json
{
    json_window = new JsonWindow(this, QString::fromStdString(XmlToJson::to_json(root)), file_path);
    json_window->show();
}


void MainWindow::on_actionundo_triggered()  // on trigerring undo
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    redo.push(label->text());
    ui->actionredo->setEnabled(true);
    label->setText(undo.top());
    undo.pop();
    if (undo.empty()) {
        ui->actionundo->setEnabled(false);
    }
}


void MainWindow::on_actionredo_triggered()  // on trigerring redo
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    undo.push(label->text());
    ui->actionundo->setEnabled(true);
    label->setText(redo.top());
    redo.pop();
    if (redo.empty()) {
        ui->actionredo->setEnabled(false);
    }
}


void MainWindow::on_actioncompress_triggered()
{
    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "Compression", "No file given to compress");
        return;
    }

    try {
        compress(file_path.toStdString());
        write_file(file_path.replace(QRegExp(".xml"), ".bin").toStdString(), false);
    } catch (const exception &e) {
        QMessageBox::warning(this, "Compression", "Error doing compression");
        return;
    }

    QMessageBox::information(this, "Compression", "Compressed file can be found in same directory as the xml file");
}


void MainWindow::on_actiondecompress_triggered()
{
    file_path = QFileDialog::getOpenFileName(this, "Select Compressed File", QDir::homePath(), "binary file (*.bin)");
    if (file_path.isEmpty()) {
        return;
    }

    try {
        decompress(file_path.toStdString());
        file_path = file_path.replace(QRegExp(".bin"), ".xml");
        write_file(file_path.toStdString(), true);
    } catch (const exception &e) {
        QMessageBox::warning(this, "Decompress Error!!", "Can't Open or Decompress File!!");
        return;
    }
    QMessageBox::information(this, "Decompression", "Decompressed file saved in the same directory");

    QString file_txt {QString::fromStdString(get_decoded_string())};
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    label->setText(file_txt);
    locked_state();
}


void MainWindow::on_actionminify_triggered()
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    undo.push(label->text());
    label->setText(QString::fromStdString(root->to_string(false)));
    ui->actionundo->setEnabled(true);
}


void MainWindow::on_actionanalyze_network_triggered()
{
    net_window = new NetWindow(this, root);
    net_window->show();
}


void MainWindow::on_actionsearch_posts_triggered()
{
    bool ok;
    QString userInput = QInputDialog::getText(this, "Search for Posts", "Enter the topic:", QLineEdit::Normal, "", &ok);
    if (!ok || userInput.isEmpty()) {
        return;
    }

    vector<string> matching_posts {search_by_word_or_topic(root, userInput.toStdString())};
    if (matching_posts.empty()) {
        QMessageBox::information(this, "Posts", "No Posts Found!");
        return;
    }

    posts_window = new PostsWindow(this, &matching_posts);
    posts_window->show();
}


void MainWindow::on_actionprettify_triggered()
{
    QLabel *label = static_cast<QLabel *>(ui->scrollArea->widget());
    undo.push(label->text());
    label->setText(QString::fromStdString(root->to_string(true)));
    ui->actionundo->setEnabled(true);
}


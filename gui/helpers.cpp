#include <helpers.h>

void save(QWidget *parent, const QString &file_path, const QString &txt) {
    QFile file(file_path);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(parent, "Writing Error!", "Can't open or create file for write");
        return;
    }

    QTextStream stream(&file);
    stream << txt;
    file.flush();
    file.close();
    QMessageBox::information(parent, "File Saved!", "File was saved in the same directory");
}

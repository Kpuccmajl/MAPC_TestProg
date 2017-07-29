#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnSaveXML_clicked();

    void on_btnLoadXML_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

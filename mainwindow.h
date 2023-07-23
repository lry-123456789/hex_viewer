#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QTableView>
#include<QStandardItemModel>
#include<QDebug>
#include<QThread>
#include<QFileDialog>
#include<string>
#include<QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_spinBox_4_valueChanged(int arg1);


    void on_spinBox_valueChanged(int arg1);

public slots:
    void runtime_show();


public:
    QTimer* timer;
    QThread m_thread;

private:
    Ui::MainWindow *ui;
    void thread_start_control(std::vector<char> bin_data);
};
#endif // MAINWINDOW_H

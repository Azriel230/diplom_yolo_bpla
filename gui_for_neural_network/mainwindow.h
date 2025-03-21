
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMovie>
#include <QProcess>
#include <QMessageBox>


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
    void on_pb_selectImage_clicked();
    void on_pb_run_nnyolo_clicked();
    void onNetworkFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void readProcessOutput();

private:
    Ui::MainWindow *ui;
    QString m_inputFilePath;
    QString m_outputFilePath;
    QMovie* m_loadingMovie;

};

#endif // MAINWINDOW_H

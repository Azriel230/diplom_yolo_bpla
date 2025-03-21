#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_run_nnyolo->setEnabled(false);
    ui->lbl_inputImage->setStyleSheet("border: 1px solid black;");
    ui->lbl_outputImage->setStyleSheet("border: 1px solid black;");

    // Анимация загрузки
    m_loadingMovie = new QMovie(":/images/loading.gif"); // Загрузите GIF-анимацию в ресурсы
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pb_selectImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty())
    {
        m_inputFilePath = fileName;
        QPixmap pixmap(fileName);
//        ui->lbl_inputImage->setPixmap(pixmap.scaled(512, 512, Qt::KeepAspectRatio));
        ui->lbl_inputImage->setPixmap(pixmap);
//        ui->lbl_inputImage->setScaledContents(false);
        ui->lbl_inputImage->setPixmap(pixmap.scaled(ui->lbl_inputImage->width(), ui->lbl_inputImage->height(), Qt::KeepAspectRatio));
        ui->pb_run_nnyolo->setEnabled(true);
    }
}


void MainWindow::on_pb_run_nnyolo_clicked()
{
    if (m_inputFilePath.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Изображение не выбрано!");
        return;
    }

    // Make sure the GIF was loaded correctly
    if (!m_loadingMovie->isValid())
    {
        QMessageBox::warning(this, "Ошибка", "Гифка не грузится");
    }

    // Показываем анимацию загрузки
    ui->lbl_outputImage->setMovie(m_loadingMovie);
    m_loadingMovie->start();


    // Запуск Python-скрипта нейросети
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onNetworkFinished);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readProcessOutput);  // Чтение вывода
    QStringList arguments;
    arguments << m_inputFilePath;
    QString pathToPython = "../../venv/Scripts/python.exe";
    QString pathToYoloScript = "../../YOLO_8/main.py";
    process->start(pathToPython, QStringList() << pathToYoloScript << arguments);
}

void MainWindow::onNetworkFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_loadingMovie->stop();
    ui->lbl_outputImage->setMovie(nullptr);

    if (exitStatus == QProcess::NormalExit && exitCode == 0)
    {
        // Путь к новому изображению уже получен в readProcessOutput
        if (!m_outputFilePath.isEmpty())
        {
            QPixmap processedPixmap(m_outputFilePath);
            ui->lbl_outputImage->setPixmap(processedPixmap);
//            ui->lbl_outputImage->setScaledContents(false);
            ui->lbl_outputImage->setPixmap(processedPixmap.scaled(ui->lbl_outputImage->width(), ui->lbl_outputImage->height(), Qt::KeepAspectRatio));
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось получить путь к обработанному изображению.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Нейросеть завершила работу с ошибкой.");
    }
}

void MainWindow::readProcessOutput()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    if (process)
    {
        QByteArray output = process->readAllStandardOutput();
        m_outputFilePath = QString(output).trimmed();  // Сохраняем путь к новому изображению
    }
}

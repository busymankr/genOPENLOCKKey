#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SierraGenerator.h"

#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGenerate_clicked()
{
    std::string openlock = ui->leKey->text().toLocal8Bit().constData();
    std::vector<uint8_t> challenge = SierraGenerator::unhexlify(openlock);

    SierraGenerator keygen;
    std::vector<uint8_t> response = keygen.SierraKeygen(challenge);

    std::string hex_response = SierraGenerator::hexlify(response);

    ui->leGenKey->setText(hex_response.c_str());
}

void MainWindow::on_btoCopy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();

    QString gen = QString("AT!OPENLOCK=\"%1\"").arg(ui->leGenKey->text().toLocal8Bit().constData());
    clipboard->setText(gen);
}


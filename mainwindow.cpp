#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inputPushButton_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,"打开输入图像","D:\\Fake_Desk","Images (*.jpg *.png *.bmp)");
    if(QFile::exists(fileName))
    {
         ui->inputLineEdit->setText(fileName);
    }
}


void MainWindow::on_OutputPushButton_pressed()
{
    QString fileName = QFileDialog::getSaveFileName(this,"选择输出图像路径","D:\\Fake_Desk","Images (*.jpg *.png *.bmp)");
    if(!fileName.isEmpty())
    {
        ui->outputLineEdit->setText(fileName);
        using namespace cv;
        Mat inImage, outImage;
        inImage = imread(ui->inputLineEdit->text().toStdString());
        if(ui->medianBlurRadioButton->isChecked())
            medianBlur(inImage, outImage, 5);
        else if(ui->gaussianBlurRadioButton->isChecked())
            GaussianBlur(inImage, outImage, Size(5,5), 1.25);
        else if(ui->grayRadioButton->isChecked())
            cvtColor(inImage, outImage, CV_RGB2GRAY);
        imwrite(fileName.toStdString(), outImage);
        if(ui->displayImageCheckBox->isChecked())
            imshow("Output Image", outImage);   //名字设为中文无法正常显示图片
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, "提示", "确定要退出吗？", QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        saveSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("packt", "Open_Pictures", this);
    ui->inputLineEdit->setText(settings.value("inputLineEdit", "").toString());
    ui->outputLineEdit->setText(settings.value("outputLineEdit", "").toString());
    //第一次打开程序的时候，默认选择“中值滤波”和“显示图片”
    //第一次打开过后，以后的设置都加载最近的一次
    ui->medianBlurRadioButton->setChecked(settings.value("medianBlurRadioButton", true).toBool());
    ui->gaussianBlurRadioButton->setChecked(settings.value("gaussianBlurRadioButton", false).toBool());
    ui->grayRadioButton->setChecked(settings.value("grayRadioButton", false).toBool());
    ui->displayImageCheckBox->setChecked(settings.value("displayImageCheckBox", true).toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("packt", "Open_Pictures", this);
    settings.setValue("inputLineEdit", ui->inputLineEdit->text());
    settings.setValue("outputLineEdit", ui->outputLineEdit->text());
    settings.setValue("medianBlurRadioButton", ui->medianBlurRadioButton->isChecked());
    settings.setValue("gaussianBlurRadioButton", ui->gaussianBlurRadioButton->isChecked());
    settings.setValue("grayRadioButton", ui->grayRadioButton->isChecked());
    settings.setValue("displayImageCheckBox", ui->displayImageCheckBox->isChecked());
}




#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QStandardPaths>
#include <QDir>
#include <QLabel>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 v%2").arg(APP_NAME, APP_VERSION));

    QDir _dir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/%1").arg(APP_NAME));
    if(!_dir.exists())
        _dir.mkpath(_dir.absolutePath());
    settings = new QSettings(_dir.absolutePath().append("/%1.ini").arg(APP_NAME),QSettings::IniFormat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu _menu(this);
    _menu.addAction(ui->actionCopyPointsToClipboard);
    _menu.addAction(ui->actionclearPoints);
    _menu.exec(event->globalPos());
}


void MainWindow::on_actionopenImage_triggered()
{
    const QString _filename = QFileDialog::getOpenFileName(this,APP_NAME,settings->value("LastFile",QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString(),"Images (*.png *.jpeg *.jpg *.bmp)");
    QImage _image = QImage(_filename);
    if(!_image.isNull()) {
        ui->widget->setImage(_image);
        settings->setValue("LastFile",_filename);
    }
}

void MainWindow::on_actionCopyPointsToClipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->widget->getPoints());
}

void MainWindow::on_actionclearPoints_triggered()
{
    ui->widget->clearPoints();
}

void MainWindow::on_actionShowAbout_triggered()
{
    QDialog aboutDialog(this);
    aboutDialog.setPalette(palette());
    aboutDialog.setFont(font());
    int pointSize = aboutDialog.font().pointSize();
    QVBoxLayout layout;
    layout.setMargin(pointSize);
    QLabel appLabel(QString("%1 v.%2").arg(APP_NAME, APP_VERSION));
    appLabel.setAlignment(Qt::AlignCenter);
    appLabel.setFrameStyle(QFrame::Box | QFrame::Sunken);
    QLabel authorLabel(tr("%1").arg(APP_DESIGNER));
    authorLabel.setAlignment(Qt::AlignCenter);
    QLabel infoLabel(tr("The application is designed for manual stroke and extraction of contours on images"));
    infoLabel.setAlignment(Qt::AlignCenter);
    infoLabel.setWordWrap(true);
    infoLabel.setFrameStyle(QFrame::Box | QFrame::Sunken);
    infoLabel.setMargin(pointSize);
    aboutDialog.setLayout(&layout);
    layout.addWidget(&appLabel);
    layout.addWidget(&authorLabel);
    layout.addWidget(&infoLabel);
    aboutDialog.setFixedSize(pointSize * 40, pointSize * 20);
    aboutDialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

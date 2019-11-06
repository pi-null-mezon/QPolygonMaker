#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <QContextMenuEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_actionopenImage_triggered();

    void on_actionCopyPointsToClipboard_triggered();

    void on_actionclearPoints_triggered();

    void on_actionShowAbout_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
};
#endif // MAINWINDOW_H

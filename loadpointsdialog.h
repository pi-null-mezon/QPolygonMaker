#ifndef LOADPOINTSDIALOG_H
#define LOADPOINTSDIALOG_H

#include <QDialog>
#include <QPointF>

namespace Ui {
class LoadPointsDialog;
}

class LoadPointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadPointsDialog(QWidget *parent = nullptr);
    ~LoadPointsDialog();

    QVector<QPointF> points() const;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::LoadPointsDialog *ui;
};

#endif // LOADPOINTSDIALOG_H

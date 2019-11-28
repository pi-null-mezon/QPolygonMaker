#include "loadpointsdialog.h"
#include "ui_loadpointsdialog.h"

LoadPointsDialog::LoadPointsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadPointsDialog)
{
    ui->setupUi(this);
}

LoadPointsDialog::~LoadPointsDialog()
{
    delete ui;
}

QVector<QPointF> LoadPointsDialog::points() const
{
    QStringList _pointsnames = ui->textEdit->toPlainText().split('(',QString::SkipEmptyParts);
    QVector<QPointF> _points;
    _points.reserve(_pointsnames.size());
    QPointF _point;
    bool _ok = false;
    for(const QString &_name :_pointsnames) {
        _point.setX(_name.section(',',0,0).toFloat(&_ok));
        if(_ok)
            _point.setY(_name.section(',',1,1).section(')',0,0).toFloat(&_ok));
        if(_ok)
            _points.push_back(qMove(_point));
    }
    return _points;
}

void LoadPointsDialog::on_pushButton_2_clicked()
{
    reject();
}

void LoadPointsDialog::on_pushButton_clicked()
{
    accept();
}

void LoadPointsDialog::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
}

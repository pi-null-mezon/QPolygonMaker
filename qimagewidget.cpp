#include "qimagewidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMimeData>

#include <cmath>

bool isNear(const QPointF &_lp, const QPointF &_rp)
{
    if(std::sqrt((_lp.x()-_rp.x())*(_lp.x()-_rp.x()) + (_lp.y()-_rp.y())*(_lp.y()-_rp.y())) < 0.0075)
        return true;
    return false;
}

QImageWidget::QImageWidget(QWidget *parent) : QWidget(parent), allowpointmove(false), scale(1), allowtranslation(false)
{
    setAcceptDrops(true);
}

void QImageWidget::paintEvent(QPaintEvent *_event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(translation);
    painter.scale(scale,scale);

    painter.fillRect(rect(),Qt::gray);
    if(!image.isNull()) {
        painter.drawImage(inscribedrect,image);

        QPen _pen = painter.pen();
        _pen.setColor(Qt::red);
        painter.setBrush(QColor(255,255,255,175));
        painter.setPen(_pen);

        QVector<QPointF> abspoints;
        abspoints.reserve(points.size());
        for(int i = 0; i < points.size(); ++i)
            abspoints.push_back(QPointF(points[i].x()*inscribedrect.width() + inscribedrect.x(),
                                        points[i].y()*inscribedrect.height() + inscribedrect.y()));
        painter.drawPolygon(abspoints);
        for(int i = 0; i < points.size(); ++i)
            painter.drawEllipse(abspoints[i],4,4);
    }
}

void QImageWidget::resizeEvent(QResizeEvent *event)
{
    if(!image.isNull())
        inscribedrect = makeInscribedRect(rect(),image.rect());
    else
        inscribedrect = QRectF();
}

void QImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(inscribedrect.isValid() && (event->button() == Qt::LeftButton)) {
        QPointF _point((event->x()/scale - inscribedrect.x() - translation.x()/scale)/(inscribedrect.width()),
                       (event->y()/scale - inscribedrect.y() - translation.y()/scale)/(inscribedrect.height()));
        qDebug("Absolute (%d;%d) >> relative to image's rect (%.3f;%.3f)",event->x(),event->y(),_point.x(),_point.y());
        for(int i = 0; i < points.size(); ++i) {
            if(isNear(points[i],_point)) {
                pointindex = i;
                allowpointmove = true;
                return;
            }
        }
        points.push_back(std::move(_point));
        update();
    } else if(event->button() == Qt::MiddleButton) {
        allowtranslation = true;
        translationstart = event->pos();
    }
}

void QImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(allowpointmove) {
        QPointF _point((event->x() - inscribedrect.x())/inscribedrect.width(),(event->y() - inscribedrect.y())/inscribedrect.height());
        points[pointindex] = std::move(_point);
        update();
    }
    if(allowtranslation) {
        translation = translationend + event->pos() - translationstart;
        update();
    }
}

void QImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(allowpointmove) {
        allowpointmove = false;
        update();
    }
    if(allowtranslation) {
        translationend = translation;
        allowtranslation = false;
        update();
    }
}

void QImageWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 120;
    //qDebug("wheel: %d, %d",numDegrees.x(),numDegrees.y());
    scale = scale + 0.1*numDegrees.y();
    if(scale < 1)
        scale = 1;
    else if(scale > 5.0)
        scale = 5.0;
    update();
}

void QImageWidget::dropEvent(QDropEvent *_event)
{
    QString _filename = QUrl(_event->mimeData()->text()).path();
#ifdef Q_OS_WIN
    _filename = _filename.section('/',1);
#endif
    QImage _image(_filename);
    qDebug("%s",_filename.toUtf8().constData());
    if(!_image.isNull())
        setImage(_image);
}

void QImageWidget::dragEnterEvent(QDragEnterEvent *_event)
{
    if(_event->mimeData()->hasText())
        _event->acceptProposedAction();
}


void QImageWidget::setImage(const QImage &value)
{
    points.clear();
    points.reserve(1024);
    image = value;
    if(!image.isNull())
        inscribedrect = makeInscribedRect(rect(),image.rect());
    else
        inscribedrect = QRectF();
    update();
}

QRectF QImageWidget::makeInscribedRect(const QRectF &_bound, const QRectF &_source)
{
    QRectF _output;
    if(_bound.width()/_bound.height() > _source.width()/_source.height()) {
        _output.setHeight(_bound.height());
        _output.setWidth(_bound.height() * _source.width()/_source.height());
        _output.moveLeft((_bound.width() - _output.width())/2.0);
    } else {
        _output.setWidth(_bound.width());
        _output.setHeight(_bound.width() * _source.height()/_source.width());
        _output.moveTop((_bound.height() - _output.height())/2.0);
    }
    return _output;
}

QString QImageWidget::getPoints() const
{
    QString _str = "{";
    for(int i = 0; i < (points.size()-1); ++i)
        _str.append(QString("QPointF(%1,%2), ").arg(QString::number(points[i].x(),'f'),QString::number(points[i].y(),'f')));
    _str.append(QString("QPointF(%1,%2)};").arg(QString::number(points[points.size()-1].x(),'f'),QString::number(points[points.size()-1].y(),'f')));
    return _str;
}

void QImageWidget::clearPoints()
{
    points.clear();
    update();
}

#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H

#include <QWidget>
#include <QImage>

#include <QWheelEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QRectF>
#include <QPointF>
#include <QVector>

class QImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QImageWidget(QWidget *parent = nullptr);

    void setImage(const QImage &value);

    static QRectF makeInscribedRect(const QRectF &_bound,const QRectF &_source);

    void    setPoints(const QVector<QPointF> &_points);
    QString getPoints() const;
    void    clearPoints();

public slots:

protected:
    void paintEvent(QPaintEvent *_event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void dropEvent(QDropEvent *_event);
    void dragEnterEvent(QDragEnterEvent *_event);

private:

    QImage image;
    QRectF inscribedrect;
    QVector<QPointF> points;
    int pointindex;
    bool allowpointmove;
    qreal scale;
    QPoint translation;
    QPoint translationstart;
    QPoint translationend;
    bool allowtranslation;
};

#endif // QIMAGEWIDGET_H

//
// Created by ahmed Ibrahim on 30-Nov-24.
//

#ifndef DESIGNI_RECTQML_H
#define DESIGNI_RECTQML_H
#include <QObject>
#include <QVector>
#include <QRectF>
#include <QQmlContext>
#include <QObject>
#include <QQmlApplicationEngine>
#include "QString"

class RectQML : public QObject {
    Q_OBJECT
    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString roomId READ roomId WRITE setRoomId NOTIFY roomIdChanged)

public:
    RectQML(QObject* parent = nullptr) : QObject(parent), m_x(0), m_y(0), m_width(0), m_height(0) {}

    double x() const { return m_x; }
    void setX(double x) { if (m_x != x) { m_x = x; emit xChanged(); } }

    double y() const { return m_y; }
    void setY(double y) { if (m_y != y) { m_y = y; emit yChanged(); } }

    double width() const { return m_width; }
    void setWidth(double width) { if (m_width != width) { m_width = width; emit widthChanged(); } }

    double height() const { return m_height; }
    void setHeight(double height) { if (m_height != height) { m_height = height; emit heightChanged(); } }

    QString roomId() const { return m_roomId; }
    void setRoomId(const QString& id) { if (m_roomId != id) { m_roomId = id; emit roomIdChanged(); } }

    signals:
            void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();
    void roomIdChanged();

private:
    double m_x, m_y, m_width, m_height;
    QString m_roomId;
};

#endif //DESIGNI_RECTQML_H

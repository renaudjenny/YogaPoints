#ifndef YOGAPOINT_H
#define YOGAPOINT_H

#include <QString>
#include <QWidget>

class YogaPoint
{
public:
    YogaPoint();
    YogaPoint(int id, const QString& name, unsigned int points = 0);
    virtual ~YogaPoint();
    virtual unsigned int calculatePoints() = 0;
    QString name() const;
    int id();
    void setId(int id);
    void setPoint(int points);
    void setName(const QString &name);
protected:
    QString m_name;
    unsigned int m_points;
    int m_id;
};

#endif // YOGAPOINT_H

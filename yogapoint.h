#ifndef YOGAPOINT_H
#define YOGAPOINT_H

#include <QString>
#include <QList>
#include <QWidget>

class YogaPoint
{
public:
    YogaPoint();
    YogaPoint(int id, const QString& name, unsigned int points);
    virtual ~YogaPoint();
    virtual unsigned int calculatePoints();
    QString name() const;
    int id();
    int setId(int id);
    void setPoint(int points);
    void setName(const QString &name);

    static YogaPoint yogaPointFromDatabase(const QString &positionName, QWidget *window);
    static YogaPoint yogaPointFromDatabase(int positionId, QWidget* window);
private:
    QString m_name;
    unsigned int m_points;
    int m_id;
};

#endif // YOGAPOINT_H

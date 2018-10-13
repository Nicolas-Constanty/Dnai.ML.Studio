#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>

class ApiHandler : public QObject
{
    Q_OBJECT
public:
    ApiHandler(QObject *parent = nullptr);
};

#endif // APIHANDLER_H

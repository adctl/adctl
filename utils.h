#ifndef UTIL_H
#define UTIL_H
#include <QObject>
#include <QString>

class QQmlEngine;

class Util:public QObject
{
    Q_OBJECT
public:
    static bool javaScriptFuncExists(QObject* object, const char* method);
    static QString takeScreenShot(QQmlEngine *engine);
    static Util *getInstance();
    Q_INVOKABLE QString screenShot();
    QQmlEngine* qml_engine;
private:
    static Util* m_instance;
};

#endif // UTIL_H

#include "utils.h"
#include <QMetaMethod>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QDebug>

Util* Util::m_instance = nullptr;

Util* Util::getInstance() {
    if(!m_instance){
        m_instance = new Util;
    }
    return m_instance;
}

QString Util::screenShot()
{
    if(qml_engine){
        return Util::takeScreenShot(qml_engine);
    }
    return QString();
}

bool Util::javaScriptFuncExists(QObject* object, const char* method)
{
    const QMetaObject* m = object->metaObject();
    for (int i = 0 ; i < m->methodCount() ; i++ ) {
        if (m->method(i).name() == method) {
            return true;
        }
    }
    return false;
}

QString Util::takeScreenShot(QQmlEngine* engine)
{
    QString filename= QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    filename+= "/app_screenshot";
    filename+= QDateTime::currentDateTime().toString(Qt::ISODate).replace(":","");
    filename+= ".png";

    QQmlApplicationEngine* app_eng = qobject_cast<QQmlApplicationEngine*>(engine);
    if(app_eng){
        foreach(QObject* obj, app_eng->rootObjects()) {
          QQuickWindow* window = qobject_cast<QQuickWindow*>(obj);
          if (window) {
            QImage image = window->grabWindow();
            bool saved =image.save(filename);
            qDebug() << "image_path = " << filename;
            qDebug() << "image_size = " << image.size();
            if(saved){
                qDebug() << "Image saved";
            }else{
                qDebug()  << "Image save failed";
            }
          }
        }
    }
    return filename;
}


#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QEventLoop>
#include "webpage.h"

class Manager : public QObject
{
  Q_OBJECT

public:
  Manager (QObject *parent=0);
  ~Manager();
  QString getUrlContent(QUrl url);

private slots:
  void proxy(void);

signals:
  void finishedProcessing(bool ok);

private:
  WebPage *page;
  QEventLoop *loop;
};


#endif

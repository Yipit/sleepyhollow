#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QApplication>
#include "webpage.h"

class Manager : public QObject
{
  Q_OBJECT

public:
  Manager (QObject *parent=0);
  ~Manager();
  QString getUrlContent(QUrl url);
  QApplication *app;

private slots:
  void proxyExit(void);
  void proxyProcessEvents(void);

signals:
  void finishedProcessing(bool ok);

private:
  WebPage *page;
};


#endif

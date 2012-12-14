// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_NETWORKACCESSMANAGER_H
#define HOLLOW_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


class NetworkAccessManager : public QNetworkAccessManager
{
  Q_OBJECT

public:
  NetworkAccessManager(QObject *parent=0);
  static NetworkAccessManager* instance(void);
signals:
  void resourceRequested(const QNetworkRequest&);

protected:
  QNetworkReply* createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData=0);
};


#endif  // HOLLOW_NETWORKACCESSMANAGER_H

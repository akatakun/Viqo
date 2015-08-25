﻿#ifndef AUTOEXTEND_H
#define AUTOEXTEND_H

#include <QObject>
#include <QDateTime>
#include <QMessageBox>
#include "nicolivemanager.h"
#include "httpgetter.h"

class MainWindow;

class AutoExtend : public HttpGetter
{
  Q_OBJECT
public:
  AutoExtend(MainWindow* mwin, NicoLiveManager* nlman, QObject* parent);
  ~AutoExtend();
  void get();

private:
  QNetworkAccessManager* mExtendManager;
  void getExtend(QString code, QString item, QString num);
  NicoLiveManager* nlman;

private slots:
  void got(QNetworkReply* reply);
  void gotExtend(QNetworkReply* reply);
};

#endif // AUTOEXTEND_H

﻿#include "usermanager.h"
#include "../ui/mainwindow.h"

UserManager::UserManager(MainWindow* mwin, QObject *parent) :
  QObject(parent)
{
  this->mwin = mwin;

  mwin->insLog("UserManager::UserManager");
  db = QSqlDatabase::addDatabase("QSQLITE");

  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not found");
    return;
  }
  db.setDatabaseName(dir[0] + "/user.sqlite");

  if (db.open()) {
    mwin->insLog("db open succeeded");
  } else {
    mwin->insLog("db open error occured");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral(u"ユーザのデータベースオープンに失敗しました"));
  }

  QSqlQuery query(db);

  query.prepare("create table if not exists user "
                "(id integer unique primary key, name varchar(60))");

  if (!query.exec()) {
    mwin->insLog("UserManager::UserManager create table error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral(u"ユーザのデータベーステーブル作成に失敗しました"));
  }

  mwin->insLog();
}

// return true if ID is found in username DB or fetching from user web page.
// The state of fetching does not affect return value.
bool UserManager::getUserName(QTreeWidgetItem* item, QString userID, bool useHTTP, bool useDB)
{
  // return if empty
  if (userID.isEmpty()) return false;
  // return if 184 or broadcaster comment.
  if (item->text(6) == "@" || item->text(7) == "@") return false;

  if (useDB) {
    QSqlQuery query(db);
    query.prepare("select distinct name from user where id=:id");
    query.bindValue(":id", userID);

    if (query.exec()) {
      if (query.next()) {
        item->setText(2, query.value(0).toString());
      } else if (useHTTP) {
        UserNameGetter* ug = new UserNameGetter(mwin, this, userID);
        connect(ug, &UserNameGetter::got, this, [=](QString n){
          mwin->userManager->setUserName(item,n);
        });
        ug->get();
      } else {
          return false;
      }
    } else {
      mwin->insLog("UserManager::getUserName user db get error\n");
      QMessageBox::information(mwin, "Viqo",
                               QStringLiteral(u"ユーザのデータベーステーブル取得に失敗しました"));
      return false;
    }
  } else if (useHTTP) {
    UserNameGetter* ug = new UserNameGetter(mwin, this, userID);
    connect(ug, &UserNameGetter::got, this, [=](QString n){
      mwin->userManager->setUserName(item,n);
    });
    ug->get();
  }

  return true;
}

void UserManager::setUserName(QTreeWidgetItem* item, QString username)
{
  item->setText(2, username);

  QSqlQuery query(db);
  query.prepare("insert or replace into user (id, name) values (:id, :name)");
  query.bindValue(":id", item->text(5));
  query.bindValue(":name", username);

  if (!query.exec()) {
    mwin->insLog("user db set error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral(u"ユーザのデータベーステーブル書き込みに失敗しました"));
  }
}

void UserManager::removeUser(QTreeWidgetItem* item)
{
  QSqlQuery query(db);
  query.prepare("delete from user where id = :id");
  query.bindValue(":id", item->text(5));

  if (!query.exec()) {
    mwin->insLog("user db set error");
    QMessageBox::information(mwin, "Viqo",
                             QStringLiteral(u"ユーザのデータベーステーブル書き込みに失敗しました"));
  }
}

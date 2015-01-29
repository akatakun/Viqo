﻿#include "newwakusettingswindow.h"
#include "ui_newwakusettingswindow.h"
#include "mainwindow.h"

NewWakuSettingsWindow::NewWakuSettingsWindow(MainWindow* mwin, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewWakuSettingsWindow)
{
  ui->setupUi(this);

  this->mwin = mwin;
}

NewWakuSettingsWindow::~NewWakuSettingsWindow()
{
  delete ui;
}

void NewWakuSettingsWindow::on_befWakuReuse_clicked()
{
  mwin->getNewWakuAPI(0);
}

void NewWakuSettingsWindow::setSelectedCategory(const QString& value)
{
  selectedCategory = value;
}

void NewWakuSettingsWindow::setSelectedCommunity(const QString& value)
{
  selectedCommunity = value;
}

void NewWakuSettingsWindow::formInit()
{
  ui->communityOnly->setChecked(false);
}

void NewWakuSettingsWindow::listStateSave()
{
  selectedCommunity = ui->community->currentText();
  selectedCategory = ui->category->currentText();

  clearListForm();
}

void NewWakuSettingsWindow::clearListForm()
{
  ui->community->clear();
  ui->category->clear();
  ui->tags_list->clear();
}

void NewWakuSettingsWindow::setIndex(QString name, QString value)
{
  if (name == "default_community") {
    selectedCommunity = value;
    return;
  }
  if (name == "tags[]c") {
    selectedCategory = value;
    return;
  }
}

void NewWakuSettingsWindow::set(QString name, QString value, QString disp)
{
  if (name == "title") {
    ui->title->setText(value);
    return;
  }
  if (name == "description") {
    ui->description->setPlainText(value);
    return;
  }
  if (name == "default_community") {
    ui->community->addItem(disp, value);
    return;
  }
  if (name == "tags[]c") {
    ui->category->addItem(disp, value);
    return;
  }
  if (name == "tags[]") {
    if (value == "顔出し") {
      ui->additional_unmask->setChecked(true);
    } else if (value == "凸待ち") {
      ui->additional_callMe->setChecked(true);
    } else if (value == "クルーズ待ち") {
      ui->additional_cruise->setChecked(true);
    }
    return;
  }
  if (name.startsWith("livetags")) {
    int index = name.mid(8).toInt() - 1;
    while (index >= ui->tags_list->count()) {
      auto newitem = new QListWidgetItem(ui->tags_list);
      newitem->setFlags(newitem->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      newitem->setCheckState(Qt::Unchecked);
    }
    ui->tags_list->item(index)->setText(value);
    return;
  }
  if (name.startsWith("taglock")) {
    int index = name.mid(7).toInt() - 1;
    ui->tags_list->item(index)->setCheckState(Qt::Checked);
    return;
  }
  if (name == "public_status") {
    if (value == "2")
      ui->communityOnly->setChecked(true);
    else
      ui->communityOnly->setChecked(false);
    return;
  }
  if (name == "timeshift_enabled") {
    if (value == "1")
      ui->timeshift->setChecked(true);
    else
      ui->timeshift->setChecked(false);
    return;
  }
  if (name == "twitter_disabled") {
    if (value == "0")
      ui->twitter->setChecked(true);
    else
      ui->twitter->setChecked(false);
    return;
  }
  if (name == "input_twitter_tag") {
    ui->twitterTag->setText(value);
    return;
  }
  if (name == "ad_enable") {
    if (value == "0")
      ui->advertising->setChecked(true);
    else
      ui->advertising->setChecked(false);
    return;
  }
  if (name == "ichiba_type") {
    if (value == "1")
      ui->ichiba->setChecked(true);
    else
      ui->ichiba->setChecked(false);
    return;
  }
}

void NewWakuSettingsWindow::listStateLoad()
{
  ui->community->setCurrentIndex(ui->community->findText(selectedCommunity));
  ui->category->setCurrentIndex(ui->category->findText(selectedCategory));
}

void NewWakuSettingsWindow::applySettingsPostData()
{
  // necessary
  mwin->nicolivemanager->newWakuSetFormData("title", ui->title->text());
  mwin->nicolivemanager->newWakuSetFormData("description", ui->description->toPlainText());
  mwin->nicolivemanager->newWakuSetFormData("default_community", ui->community->currentData().toString());
  mwin->nicolivemanager->newWakuSetFormData("tags[]c", ui->category->currentData().toString());

  // tags
  for (int i = 0; i < ui->tags_list->count(); ++i) {
    QListWidgetItem* item = ui->tags_list->item(i);
    mwin->nicolivemanager->newWakuSetFormData("livetags" + QString::number(i+1), item->text());
    if (item->checkState() == Qt::Checked)
      mwin->nicolivemanager->newWakuSetFormData("taglock" + QString::number(i+1), "ロックする");
  }

  // other
  if (ui->tag_allLock->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("taglock", "ロックする");
  if (ui->additional_unmask->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "顔出し");
  if (ui->additional_callMe->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "凸待ち");
  if (ui->additional_cruise->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("tags[]", "クルーズ待ち");
  if (ui->communityOnly->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("public_status", "2");
  if (ui->timeshift->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("timeshift_enabled", "1");
  else
    mwin->nicolivemanager->newWakuSetFormData("timeshift_enabled", "0");
  if (ui->twitter->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("twitter_disabled", "0");
  else
    mwin->nicolivemanager->newWakuSetFormData("twitter_disabled", "1");
  mwin->nicolivemanager->newWakuSetFormData("twitter_tag", ui->twitterTag->text());
  if (ui->advertising->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("ad_enable", "0");
  else
    mwin->nicolivemanager->newWakuSetFormData("ad_enable", "1");
  if (ui->ichiba->isChecked())
    mwin->nicolivemanager->newWakuSetFormData("ichiba_type", "1");
  else
    mwin->nicolivemanager->newWakuSetFormData("ichiba_type", "0");

}

bool NewWakuSettingsWindow::isSetNecessary()
{
  return ui->title->text() != "" &&
      ui->description->toPlainText() != "" &&
      ui->community->currentText() != "" &&
      ui->category->currentText() != "";
}

bool NewWakuSettingsWindow::isTwitterTagValid()
{
  return !ui->twitter->isChecked() || ui->twitter->text().startsWith('#');
}

void NewWakuSettingsWindow::on_tag_add_clicked()
{
  QListWidgetItem* item = new QListWidgetItem(ui->tags_list);
  item->setText("タグ名");
  item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
  item->setCheckState(Qt::Unchecked);
}

void NewWakuSettingsWindow::on_tag_delete_clicked()
{
  delete ui->tags_list->currentItem();
}

void NewWakuSettingsWindow::savePresets()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }

  QJsonDocument jsd;
  //jsd.setObject(root);

  QFile file(dir[0] + "/newWakuSettings.json");
  file.open(QIODevice::WriteOnly);
  QTextStream out(&file);
  out << jsd.toJson();
  file.close();
}

void NewWakuSettingsWindow::loadPresets()
{
  QStringList dir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  if (dir.empty()) {
    mwin->insLog("save directory is not available");
    return;
  }
  QFile file(dir[0] + "/newWakuSettings.json");
  if ( !file.exists() ) {
    file.close();
    mwin->insLog("no preset file");
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QJsonDocument jsd = QJsonDocument::fromJson(file.readAll());

  auto formArray = jsd.array();


  file.close();
}

QJsonObject NewWakuSettingsWindow::makeJsonFromForm()
{
  QJsonObject necessary;
  {
    necessary["title"] = ui->title->text();
    necessary["description"] = ui->description->toPlainText();

    QJsonArray community;
    for (int i = 0; i < ui->community->count(); ++i) {
      QJsonArray item;
      item << ui->community->itemText(i);
      item << ui->community->itemData(i).toString();
      community << item;
    }
    necessary["community"] = community;

    QJsonArray category;
    for (int i = 0; i < ui->category->count(); ++i) {
      QJsonArray item;
      item << ui->category->itemText(i);
      item << ui->category->itemData(i).toString();
      category << item;
    }
    necessary["category"] = category;
  }

  QJsonObject other;
  {
    QJsonArray tags;
    for (int i = 0; i < ui->tags_list->count(); ++i) {
      QListWidgetItem* item = ui->tags_list->item(i);
      QJsonArray jitem;
      jitem << (item->checkState() == Qt::Checked);
      jitem << item->text();
      tags << jitem;
    }
    other["tags"] = tags;

    other["tag_all_lock"] = ui->tag_allLock->isChecked();

    other["add_unmask"] = ui->additional_unmask->isChecked();
    other["add_call_me"] = ui->additional_callMe->isChecked();
    other["add_cruise"] = ui->additional_cruise->isChecked();

    other["community_only"] = ui->communityOnly->isChecked();
    other["timeshift"] = ui->timeshift->isChecked();
    other["twitter"] = ui->twitter->isChecked();
    other["twitter_tag"] = ui->twitterTag->text();
    other["advertising"] = ui->advertising->isChecked();
    other["ichiba"] = ui->ichiba->isChecked();
  }

  QJsonObject root;
  root["necessary"] = necessary;
  root["other"] = other;

  return root;
}

void NewWakuSettingsWindow::setPresetsFromJson(const QJsonObject& jsn)
{
  clearListForm();

  {
    const QJsonObject necessary = jsn["necessary"].toObject();

    ui->title->setText(necessary["title"].toString());
    ui->description->setPlainText(necessary["description"].toString());

    const QJsonArray community = necessary["community"].toArray();
    for (int i = 0; i < community.size(); ++i) {
      ui->community->addItem(community[i].toArray()[0].toString(), community[i].toArray()[1].toString());
    }

    const QJsonArray category = necessary["category"].toArray();
    for (int i = 0; i < category.size(); ++i) {
      ui->category->addItem(category[i].toArray()[0].toString(), category[i].toArray()[1].toString());
    }
  }

  {
    const QJsonObject other = jsn["other"].toObject();

    const QJsonArray tags = other["tags"].toArray();
    for (int i = 0; i < tags.size(); ++i) {
      QListWidgetItem* item = new QListWidgetItem(ui->tags_list);
      const QJsonArray jitem = tags[i].toArray();
      item->setText(jitem[1].toString());
      item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      item->setCheckState(jitem[0].toBool()?Qt::Checked:Qt::Unchecked);
    }

    ui->tag_allLock->setChecked(other["tag_all_lock"].toBool());

    ui->additional_unmask->setChecked(other["add_unmask"].toBool());
    ui->additional_callMe->setChecked(other["add_call_me"].toBool());
    ui->additional_cruise->setChecked(other["add_cruise"].toBool());

    ui->communityOnly->setChecked(other["community_only"].toBool());
    ui->timeshift->setChecked(other["timeshift"].toBool());
    ui->twitter->setChecked(other["twitter"].toBool());
    ui->twitterTag->setText(other["twitter_tag"].toString());
    ui->advertising->setChecked(other["advertising"].toBool());
    ui->ichiba->setChecked(other["ichiba"].toBool());
  }

}

void NewWakuSettingsWindow::on_presets_regist_clicked()
{
  QString text = QInputDialog::getText(this, "プリセット登録", "プリセット名:");
  if (!text.isEmpty()) {
    int indexNew = ui->presetes->findText(text);
    if (indexNew == -1) {
      ui->presetes->addItem(text, makeJsonFromForm());
      ui->presetes->setCurrentText(text);
    } else {
      ui->presetes->setItemData(indexNew, makeJsonFromForm());
    }
  }
}

void NewWakuSettingsWindow::on_presets_delete_clicked()
{
  ui->presetes->removeItem(ui->presetes->currentIndex());
}

void NewWakuSettingsWindow::on_presetes_activated(int index)
{
  setPresetsFromJson(ui->presetes->itemData(index).toJsonObject());
}

void NewWakuSettingsWindow::on_clear_clicked()
{
  ui->title->clear();
  ui->description->clear();
  ui->community->unsetCursor();
  ui->category->unsetCursor();
  ui->tags_list->clear();
  ui->additional_unmask->setChecked(false);
  ui->additional_callMe->setChecked(false);
  ui->additional_cruise->setChecked(false);
  ui->communityOnly->setChecked(false);
  ui->timeshift->setChecked(false);
  ui->twitter->setChecked(false);
  ui->twitterTag->clear();
  ui->advertising->setChecked(false);
  ui->ichiba->setChecked(false);
}

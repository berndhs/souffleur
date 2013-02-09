#include "background-picker.h"


#include <stdlib.h>
#include "deliberate.h"
#include <QDesktopServices>
#include <QProcess>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#include <QTimer>
#include <QDebug>

#include <iostream>

using namespace deliberate;

namespace agenda
{

BackgroundPicker::BackgroundPicker(SystemType sysType, QObject *parent)
  : QObject (parent),
    systemType (sysType)
{
  QString home = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
  switch (systemType) {
  case System_Harmattan:
  default:
    defaultFile = home + "/.wallpapers/wallpaper.png";
    wallpapers << defaultFile << defaultFile;
    break;
  }
  changers[System_Harmattan] = changeHarmattan;
  changers[System_Xfce4] = changeXfce4;
  qDebug () << " change function map " << changers;
}

void
BackgroundPicker::run (RunType runType)
{
  runWhat = runType;
  QTimer::singleShot (1,this,SLOT (doRun()));
}

void
BackgroundPicker::doRun()
{
  switch (runWhat) {
  case Run_Random:
    pickRandom();
    break;
  case Run_Daily:
    pickDaily();
    break;
  case Run_Hourly:
    pickHourly();
    break;
  default:
    break;
  }
}

void
BackgroundPicker::pickRandom()
{
  qDebug () << __PRETTY_FUNCTION__;
  timespec        fineTime;
  unsigned   int  seed (0);

  clock_gettime (CLOCK_REALTIME, &fineTime);
  
  // mix seconds and nanoseconds to get a meaningless number
  seed = abs (fineTime.tv_sec + fineTime.tv_nsec);
  
  srandom (seed);
  int choice (0);

  wallpapers = Settings().value ("wallpapers/files",wallpapers).toStringList();
  qDebug () << __PRETTY_FUNCTION__ << " wallpapers " << wallpapers;
  Settings().setValue("wallpapers/files",wallpapers);
  Settings().sync();
  if (wallpapers.count() > 1) {
    choice = random() % (wallpapers.count() - 1);
    qDebug () << "     random choice " << choice;
  }
  std::cout << __PRETTY_FUNCTION__ << " choice is " << choice << std::endl;
  std::cout.flush();
  if (choice < wallpapers.count()) {
    if (changers.contains(systemType)) {
      (*changers[systemType]) (wallpapers.at(choice));
    }
  }
  emit quit ();
  qDebug () << __PRETTY_FUNCTION__ << " done emitting";
}

void
BackgroundPicker::pickDaily ()
{
  qDebug () << __PRETTY_FUNCTION__;
  QDateTime now (QDateTime::currentDateTime());
  Settings().beginGroup("wallpapers_daily");
  qDebug () << __PRETTY_FUNCTION__ << Settings().childKeys();
  for (int d=0; d<7; d++) {
    QString day = now.addDays(d).toString("ddd");
    QString key (day);
    dailySchedule [day] = Settings().value (key,defaultFile).toString();
    Settings().setValue (key,dailySchedule[day]);
  }
  Settings().endGroup();
  Settings().sync();
  qDebug () << " daily is " << dailySchedule;
  qDebug () << " today is " << now.toString ("ddd");
  //changeHarmattan (dailySchedule[now.toString("ddd")]);
  QString today = now.toString("ddd");
  if (changers.contains(systemType)) {
    (*changers[systemType])(dailySchedule[today]);
  }
  emit quit ();
}

void
BackgroundPicker::pickHourly ()
{
  qDebug () << __PRETTY_FUNCTION__;
  QDateTime now (QDateTime::currentDateTime());
  int theHour = now.toString("hh").toInt();
  QString filename;
  QString defaultKey ("0-23");
  QMap <int, HourRange> keyMap;
  Settings().beginGroup("wallpapers_hourly");
  QStringList keys = Settings().childKeys();
  if (keys.isEmpty()) {
    keys <<  defaultKey;
    Settings().setValue (defaultKey,defaultFile);
  }
  for (auto k=keys.begin(); k!=keys.end(); k++) {\
    HourRange range = rangeParse(*k);
    hourlySchedule[range] = Settings().value(*k,defaultFile).toString();
    keyMap.insertMulti(range.second - range.first,  range);
  }
  qDebug () << " map is " << keyMap;
  for (auto r=keyMap.constBegin(); r!=keyMap.constEnd(); r++) {
    if (r->first <= theHour && theHour <= r->second) {
      qDebug () << " hour matches key " << *r << hourlySchedule[*r];
      filename = hourlySchedule[*r];
      break;
    }
  }
  Settings().endGroup(); 
  Settings().sync();
  if (changers.contains(systemType) && !filename.isEmpty()) {
    (*changers[systemType]) (filename);
  }
  emit quit ();
}

BackgroundPicker::HourRange
BackgroundPicker::rangeParse(const QString range)
{
  QStringList parts = range.split ("-") ;
  if (parts.count() != 2) {
    return HourRange(0,-1);
  }
  int lower = parts.at(0).toInt();
  int upper = parts.at(1).toInt();
  return HourRange (lower,upper);
}

void
BackgroundPicker::changeHarmattan(const QString &filename)
{
  qDebug () << __PRETTY_FUNCTION__ << filename;
  QFileInfo info (filename);
  if (info.isReadable()) {
    QString program ("gconftool-2");
    QStringList options;
    options << "--type=string" 
            << "--set"  
            << "/desktop/meego/background/portrait/picture_filename"
            << info.absoluteFilePath();
    QProcess::startDetached(program,options);
    options << "--type=string" 
            << "--set"  
            << "/desktop/meego/background/landscape/picture_filename"
            << info.absoluteFilePath();
    QProcess::startDetached(program,options);
  }
}

void
BackgroundPicker::changeXfce4(const QString &filename)
{
  QProcess::startDetached("xfdesktop --reload");
}

}

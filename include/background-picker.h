#ifndef BACKGROUND_PICKER_H
#define BACKGROUND_PICKER_H

#include <QStringList>
#include <QString>
#include <QMap>
#include <QPair>

namespace agenda 
{

class BackgroundPicker : public QObject
{
Q_OBJECT
public:
  
  enum SystemType {
    System_Unknown = 0,
    System_Harmattan = 1,
    System_Gnome2 = 2,
    System_Gnome3 = 3,
    System_Xfce4 = 4,
    System_KDE = 5
  };
  enum RunType {
    Run_Random = 0,
    Run_Daily = 1,
    Run_Hourly = 2
  };
  
  BackgroundPicker(SystemType sysType= System_Unknown, QObject *parent=0);
  
  void run (RunType runType);
  
signals:
  
  void quit ();
  
private slots:
  
  void doRun ();
  
private:
  
  typedef QPair<int,int> HourRange;
  typedef void (ChangeFunction) (const QString &);
  typedef QMap<SystemType,ChangeFunction*>  ChangeFunctionMap;
  
  
  static void changeHarmattan (const QString & filename);
  static void changeXfce4 (const QString & filename);
  HourRange rangeParse (const QString range);
  
  void pickRandom ();
  void pickDaily ();
  void pickHourly ();
  
  SystemType   systemType;
  RunType      runWhat;
  
  QStringList               wallpapers;
  QMap <QString, QString>   dailySchedule;
  QMap <HourRange, QString> hourlySchedule;
  
  QString     defaultFile;
  
  ChangeFunctionMap   changers;
   
};

}
#endif // BACKGROUND_PICKER_H

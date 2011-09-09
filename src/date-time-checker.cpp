#include "date-time-checker.h"
#include <QDateTime>
#include <QDebug>

namespace agenda
{

DateTimeChecker::DateTimeChecker (QObject *parent)
  :QObject(parent)
{}

bool
DateTimeChecker::isValid (const QString & candidate)
{
  QDateTime checkLong (QDateTime::fromString(candidate,"yyyy-MM-dd hh:mm:ss"));
  QDateTime checkShort (QDateTime::fromString (candidate, "yyyy-MM-dd hh:mm"));
  qDebug () << "checking " << candidate << " it is " << checkLong.isValid() 
            << " and " << checkShort.isValid();
  return checkLong.isValid() || checkShort.isValid(); 
}

} // namespace

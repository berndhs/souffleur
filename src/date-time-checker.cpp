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
  QDateTime checkLong1 (QDateTime::fromString(candidate,"yyyy-MM-dd hh:mm:ss"));
  if (checkLong1.isValid()) {
    return true;
  }
  QDateTime checkShort1 (QDateTime::fromString (candidate, "yyyy-MM-dd hh:mm"));
  if (checkShort1.isValid()) {
    return true;
  }
  QDateTime checkLong2 (QDateTime::fromString(candidate,"ddd yyyy-MM-dd hh:mm:ss"));
  if (checkLong2.isValid()) {
    return true;
  }
  QDateTime checkShort2 (QDateTime::fromString (candidate, "ddd yyyy-MM-dd hh:mm"));
  if (checkShort2.isValid()) {
    return true;
  }
  qDebug () << __PRETTY_FUNCTION__ << " bad date " << candidate;
  return false;
}

} // namespace

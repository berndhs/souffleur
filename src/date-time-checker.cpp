#include "date-time-checker.h"
#include <QDateTime>
#include <QRegExp>
#include <QDebug>

namespace agenda
{

DateTimeChecker::DateTimeChecker (QObject *parent)
  :QObject(parent)
{}

bool
DateTimeChecker::isValid (const QString & candidate)
{
  QString timeBuf (candidate);
  QRegExp leadingCrap ("\\D+");
  int pos = timeBuf.indexOf (leadingCrap);
  int crapLen = leadingCrap.cap().length();
  qDebug () << "     pos " << pos << " len " << crapLen;
  if (pos == 0) {
    timeBuf.remove (0,crapLen);
  }
  
  QDateTime checkLong1 (QDateTime::fromString(timeBuf,"yyyy-MM-dd hh:mm:ss"));
  if (checkLong1.isValid()) {
    return true;
  }
  QDateTime checkShort1 (QDateTime::fromString (timeBuf, "yyyy-MM-dd hh:mm"));
  if (checkShort1.isValid()) {
    return true;
  }
  qDebug () << __PRETTY_FUNCTION__ << " bad date " << candidate;
  return false;
}

} // namespace

#ifndef DATE_TIME_CHECKER_H
#define DATE_TIME_CHECKER_H

#include <QObject>

namespace agenda
{

class DateTimeChecker: public QObject
{
Q_OBJECT
public:
  
  DateTimeChecker (QObject *parent=0);
  
  Q_INVOKABLE bool isValid (const QString & candidate);
};

} // namespace

#endif // DATECHECKER_H

#ifndef STYLESET_H
#define STYLESET_H


#include <QString>

class styleset
{
public:
    static QString getWindowStyleSheet();
    static QString getLabelStyleSheet();
    static QString getCloseStyleSheet();
    static QString getMaximizeStyleSheet();
    static QString getRestoreStyleSheet();
    static QString getMinimizeStyleSheet();
    static QString getNextStyleSheet();
    static QString getPreviousStyleSheet();
    static QString getStopStyleSheet();
    static QString getPlayStyleSheet();
    static QString getPauseStyleSheet();
    static QString getMenuStyleSheet();
    static QString getTableViewStyleSheet();
};

#endif // STYLESET_H

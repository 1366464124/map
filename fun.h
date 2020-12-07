#ifndef FUN_H
#define FUN_H

#include <QVector>
#include <QString>
#include <QPair>
#include <QFile>
#include <QMultiMap>
#include <cstdio>
#include <cstdlib>
#include <cstring>
QVector<QPair<int, int>> getEdges(void);
QVector<QPair<int, int>> getLocation(void);
QVector<QString> getOrganizations(void);
#endif // FUN_H

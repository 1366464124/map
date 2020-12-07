#include "fun.h"
#include <QDebug>

QVector<QPair<int, int>> getEdges(void) {
    QFile read("E:/map/source/Edge.txt");
    read.open(QIODevice::ReadOnly);
    QVector<QPair<int, int>> edges;
    QString s;
    while((s = read.readLine()).isEmpty() == false) {
        edges.push_back({s.section(",", 0, 0).toInt(), s.section(",", 1, 1).toInt()});
    }
    return edges;
}

QVector<QPair<int, int>> getLocation(void) {
    QFile read("E:/map/source/Location.txt");
    read.open(QIODevice::ReadOnly);
    QVector<QPair<int, int>> locations;
    QString s;
    while((s = read.readLine()).isEmpty() == false) {
        locations.push_back({s.section("，", 0, 0).toInt(), s.section("，", 1, 1).toInt()});
    }
    return locations;
}

QVector<QString> getOrganizations(void) {
    QVector<QString> organizations;
    QFile read("E:/map/source/Organization.txt");
    read.open(QIODevice::ReadOnly);
    QTextStream r(&read);
    r.setCodec("utf-8");
    QString s;

    while((s = r.readLine()).isEmpty() == false) {
        organizations.push_back(s);
    }
    return organizations;
}

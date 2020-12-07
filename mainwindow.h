#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QPushButton>
#include <QtDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QWidgetItem>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QPoint>
#include <QAction>
#include <QString>
#include <QMessageBox>
#include <ctime>
#include <QTimer>
#include <QToolTip>

#include "fun.h"
#include "CompleteLineEdit.h"
#define INF 1e9 + 5

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //开始界面
public slots:
    //行为槽

    void setStart(QString);//设置起点
    void setEnd(QString);//设置终点
    void drawPath(void);//to do
    void clear();//清空path
    void showDialog();
    void showStatus();
    void showOrganizations();

private slots:
    void on_action_triggered();

protected:
    void showMenu(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
    QWidget *widget;//弹窗输入查询信息

    QGraphicsView *background;
    QGraphicsScene * scene;

    QVector<QPair<int, int>> edges;//边
    QVector<QPair<int, int>> locations;//坐标
    QVector<QString> organizations;//地名
    QMultiMap<QPair<int, int>, QString> posToStringMap;//坐标到地名的映射
    QPair<int, int> start = {0, 0}, end = {0, 0};//起始和终止地点坐标
    QTimer *timer;//鼠标悬停计时

    //状态啦信息
    QString s;//起始地点名
    QString e;//终止地点名
    QLabel* from = nullptr;
    QLabel* to = nullptr;

    time_t t;

    void iniTimer(void);
    void iniUI();
    void ontoMap(void);
    void drawPoints(void);
    void dijkstra(void);//to do
    //将经过的节点放到path中
    QVector<int> path;//start到end的路径
    void iniMatrix(void);
    int matrix[25][25];//邻接矩阵
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

QToolTip *tip;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    iniUI();
    iniTimer();
    this->setMouseTracking(true);
    showStatus();
    setFixedSize(this->width(), this->height());
}


void MainWindow::iniUI() {
    edges = getEdges();
    locations = getLocation();
    organizations = getOrganizations();
    ontoMap();
    iniMatrix();

    background = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    QPixmap* pixmap = new QPixmap();
    pixmap->load("E:/map/source/map.bmp");
    scene->addPixmap(*pixmap);

    background->setScene(scene);

    for(auto point : locations) {
       QGraphicsEllipseItem *item = new QGraphicsEllipseItem(point.first - 3, point.second - 3, 5, 5);
       item->setCursor(QCursor(Qt::PointingHandCursor));
       item->setBrush(QBrush(Qt::red));
       item->setPen(QPen(Qt::red));
       scene->addItem(item);
    }

    ui->setupUi(this);
    this->setWindowTitle("map");

    QImage* img = new QImage;
    img->load("E:/map/source/map.bmp");
    this->resize(img->width() + 2, img->height() + 20);

    background->resize(img->width() + 2, img->height() + 2);
}

void MainWindow::iniTimer(void) {
    timer = new QTimer(this);
    timer->start(2000);
    //timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(showOrganizations()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ontoMap(void) {
    QFile read("E:/map/source/Locorg.txt");
    read.open(QIODevice::ReadOnly);
    //posToStringMap
    QString s;
    int i = 0;
    while((s = read.readLine()).isEmpty() == false) {
        int x1 = s.section(",", 0, 0).toInt();
        int x2 = -1;
        if(s.section(",", 1, 1).isEmpty() == false) {
            x2 = s.section(",", 1, 1).toInt();
        }
        posToStringMap.insert(locations[i], organizations[x1]);
        if(x2 != -1) {
            posToStringMap.insert(locations[i], organizations[x2]);
        }
        ++i;
    }
}

void MainWindow::iniMatrix(void) {
    int size = edges.size();
    memset(matrix, INF, sizeof(matrix));
    for(int i = 0; i < size; ++i) {
        QPair<int, int> x = locations[edges[i].first], y = locations[edges[i].second];
        matrix[edges[i].second][edges[i].first] =
                matrix[edges[i].first][edges[i].second] =
                abs(x.first - y.first) + abs(x.second - y.second);
    }
}

void MainWindow::setStart(QString s) {
    for(int i = 0; i < locations.size(); ++i) {
        for(auto it = posToStringMap.find(locations[i]); it != posToStringMap.end(); ++it) {
            if(it.value() == s) {
                this->s = s;
                start = it.key();
                return;
            }
        }
    }
    start = {0, 0};
    this->s = nullptr;
}

void MainWindow::setEnd(QString s) {
    for(int i = 0; i < locations.size(); ++i) {
        for(auto it = posToStringMap.find(locations[i]); it != posToStringMap.end(); ++it) {
            if(it.value() == s) {
                this->e = s;
                end = it.key();
                return;
            }
        }
    }
    end = {0, 0};
    this->e = nullptr;
}

void MainWindow::clear() {
    scene->clear();
    QPixmap* pixmap = new QPixmap();
    pixmap->load("E:/map/source/map.bmp");
    scene->addPixmap(*pixmap);
    for(auto point : locations) {
       QGraphicsEllipseItem *item = new QGraphicsEllipseItem(point.first - 3, point.second - 3, 5, 5);
       item->setCursor(QCursor(Qt::PointingHandCursor));
       item->setBrush(QBrush(Qt::red));
       item->setPen(QPen(Qt::red));
       scene->addItem(item);
    }

}

void MainWindow::drawPath() {
    if(start == QPair<int, int>{0, 0} || end == QPair<int, int>{0, 0}) {
        return;
    }
    dijkstra();
    clear();
    if(path.empty()) return;
    QPen* pen = new QPen(Qt::red);
    pen->setWidth(5);
    for(int i = 1; i < path.size(); ++i) {
        QGraphicsLineItem *line = new QGraphicsLineItem(locations[path[i - 1]].first , locations[path[i - 1]].second , locations[path[i]].first , locations[path[i]].second );
        line->setPen(*pen);
        scene->addItem(line);
    }

}

void MainWindow::showDialog() {
    QStringList sl = QStringList();
    for(auto& s : organizations) {
        sl << s;
       // qDebug() << s;
    }
    QLabel* startLabel = new QLabel("起点");
    QLabel* endLabel = new QLabel("终点");

    CompleteLineEdit* editStart = new CompleteLineEdit(sl);
    CompleteLineEdit* editEnd = new CompleteLineEdit(sl);
    QPushButton* button = new QPushButton("提交");

    QHBoxLayout* layout1 = new QHBoxLayout();
    QHBoxLayout* layout2 = new QHBoxLayout();
    QHBoxLayout* layout3 = new QHBoxLayout();
    layout1->addWidget(startLabel);
    layout1->addWidget(editStart);
    layout2->addWidget(endLabel);
    layout2->addWidget(editEnd);
    layout3->addWidget(button);

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->addLayout(layout1);
    vlayout->addLayout(layout2);
    vlayout->addLayout(layout3);


    connect(editStart, SIGNAL(textChanged(const QString&)), this, SLOT(setStart(const QString&)));
    connect(editEnd, SIGNAL(textChanged(const QString&)), this, SLOT(setEnd(const QString&)));

    connect(editStart, SIGNAL(textChanged(const QString&)), this, SLOT(showStatus()));
    connect(editEnd, SIGNAL(textChanged(const QString&)), this, SLOT(showStatus()));

    connect(button, SIGNAL(clicked()), this, SLOT(drawPath()));

    widget = new QWidget();
    widget->setLayout(vlayout);

    widget->show();
    editStart->show();
    editEnd->show();
}

void MainWindow::on_action_triggered()
{
    showDialog();
}

void MainWindow::showStatus() {
    QStatusBar* status = statusBar();
    if(from != nullptr) delete from;
    if(to != nullptr) delete to;

    if(s == nullptr) {
        from = new QLabel(tr("起点:无"));
    } else {
        from = new QLabel(tr("起点:") + s);
    }
    if(e == nullptr) {
        to = new QLabel(tr(" 终点:无"));
    } else {
        to = new QLabel(tr(" 终点:") + e);
    }

    status->addWidget(from);
    status->addWidget(to);
}
void MainWindow::dijkstra(void) {
    int startIndex = -1, endIndex = -1;
    for(int i = 0; i < locations.size(); ++i) {
        if(locations[i] == start) {
            startIndex = i;
        }
        if(locations[i] == end) {
            endIndex = i;
        }
    }
    path.clear();
    if(startIndex == -1 || endIndex == -1) {
        if(startIndex == -1) {
            start = QPair<int, int>{0, 0};
            s = nullptr;
        }
        if(endIndex == -1) {
            end = QPair<int, int>{0, 0};
            e = nullptr;
        }
        return;
    }

    QVector<QVector<int>> paths(25, QVector<int>(1, startIndex));
    path.push_back(startIndex);
    QVector<int> len(25);
    for(int i = 0; i < 25; ++i) {
        len[i] = matrix[startIndex][i];
        //qDebug() << "length of "<< i << ":" << len[i];
    }

    for(int i = 0; i < 25; ++i) {
        for(int j = 0; j < 25; ++j) {
            for(int k = 0; k < 25; ++k) {
                if(matrix[j][k] + len[j] < len[k]) {
                    len[k] = matrix[j][k] + len[j];
                    paths[k] = paths[j];
                    paths[k].push_back(j);
                }
            }
        }
    }
    Q_ASSERT(endIndex < paths.size());
    path = paths[endIndex];
    path.push_back(endIndex);
    if(startIndex == endIndex || len[endIndex] == INF) {
        if(len[endIndex] == INF)
            QMessageBox::information(nullptr, tr("提醒"), tr("无法到达目标地点"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        path.clear();
    }
}

void MainWindow::showMenu(QMouseEvent *event)
{
    QMenu *pMenu = new QMenu(this);

    QAction *navigate = new QAction(tr("导航"), this);
    pMenu->addAction(navigate);
    connect(navigate, SIGNAL(triggered()), this, SLOT(showDialog()));

    pMenu->move(cursor().pos());
    pMenu->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    QToolTip::hideText();
    timer->start(2000);
    t = time(0);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    time_t nt = time(0);
    if(nt - t >= 2) {
        QPair<int, int> pos = {event->x(), event->y()};
        int p = -1;
        for(int i = 0; i < 25; ++i) {
            //Q_ASSERT(i < locations.size());
            if(abs(locations[i].first - pos.first) + abs(locations[i].second - pos.second) < 10) {
                p = i;
                break;
            }
        }
        if(p == -1 ) {
            if(event->button() == Qt::RightButton)
                showMenu(event);
            return;
        }
        if(event->button() == Qt::LeftButton) {
            if(start != locations[p]) {
                start = locations[p];
                s = *posToStringMap.find(start);
            } else {
                start = {0, 0};
                s = nullptr;
            }
        } else if(event->button() == Qt::RightButton) {
            if(end != locations[p]) {
                end = locations[p];
                e = *posToStringMap.find(end);
            } else {
                end = {0, 0};
                e = nullptr;
            }
        }
        //drawPath();
        clear();
        showStatus();
    } else if(event->button() == Qt::RightButton) {
        showMenu(event);
        return;
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event) {
    if(start != QPair<int, int>{0, 0} && end != QPair<int, int>{0, 0})
        drawPath();
}

void MainWindow::showOrganizations() {
    QPoint pc = QWidget::mapFromGlobal(cursor().pos());
    QPoint gp = QWidget::mapToGlobal(pc);
    QPair<int, int> pos = {pc.x(), pc.y()};
    int p = -1;
    for(int i = 0; i < locations.size(); ++i) {
        if(abs(locations[i].first - pos.first) + abs(locations[i].second - pos.second) < 10) {
            p = i;
            break;
        }
    }
    if(p == -1) return;
    QString s;
    auto it = posToStringMap.find(locations[p]);
    int k = posToStringMap.count(locations[p]);
    for(; k != 0; ++it, --k) {
        s = s + it.value() + (k == 1 ? "" : "\n");
    }
    QToolTip::showText(gp, s);
}

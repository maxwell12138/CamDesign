#ifndef FIGURE_H
#define FIGURE_H
#include <vector>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QAction>
using namespace std;
const int dickLine=3;
const int thinLine=1;
const int dotLine=1;
const int margin=12;
const int tick=5;
const int curveLine=2;
class Figure : public QWidget
{
    Q_OBJECT
public:
    Figure(QWidget *parent = 0);
    ~Figure();
    void clear();

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    void grid();
    void setTitle(QString title);
    void setTracer(bool tracer);
    void autoMatch();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    inline void setTraceIndex(int index){this->traceIndex=index;}
    inline void setInteractive(bool interactive){this->interactive=interactive;repaint();}
    void inputCurve(QPointF *curve, int num);
    void inputPoint(QPointF point);
    void autoScale();
    void initial();
    void contextMenuEvent(QContextMenuEvent *);
private:
    QMenu *popMenu;
    void popMenuInit();
    QString title;
    int traceIndex;
    int timerFlag;

    double minx,maxx,miny,maxy;
    int chart_width,chart_height;
    double x_scale,y_scale,scale;
    QPoint offset;
    int leftMargin,rightMargin,upMargin,downMargin;
    int x_num,y_num;

    QPointF *curve;
    int num;
    vector<QPointF> points;
private:
    QFont font;
    QPoint original;
    QPoint pressPoint;
    bool tracer;
    bool interactive;
private slots:
    void reset();
};

#endif // FIGURE_H

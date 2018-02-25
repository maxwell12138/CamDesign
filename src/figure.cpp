#include "figure.h"
#include <QPainter>
Figure::Figure(QWidget *parent) : QWidget(parent)
{
    curve=0;
    points.clear();
    x_scale=1.0;
    y_scale=1.0;
    scale=1.0;

    offset.setX(0);
    offset.setY(0);

    font.setPixelSize(16);
    leftMargin=2*margin;
    rightMargin=margin;
    upMargin=margin;
    downMargin=std::max(font.pixelSize(),4*margin);

    tracer=false;
    popMenuInit();
}

Figure::~Figure()
{
    curve=0;
    points.clear();
}

void Figure::clear()
{
    title.clear();
    traceIndex=0;
    if(timerFlag) this->killTimer(timerFlag);
    timerFlag=0;

    minx=0;
    maxx=100;
    miny=0;
    maxy=100;

    curve=0;
    points.clear();
    x_scale=1.0;
    y_scale=1.0;
    scale=1.0;

    offset.setX(0);
    offset.setY(0);

    font.setPixelSize(16);
    leftMargin=2*margin;
    rightMargin=margin;
    upMargin=margin;
    downMargin=std::max(font.pixelSize(),4*margin);

    font.setPixelSize(16);
    tracer=false;
}
void Figure::inputPoint(QPointF point){
    points.push_back(point);
}
void Figure::autoScale()
{
    if(curve==0&&points.empty())    return;
    if(curve!=0){
        minx=curve[0].x();
        maxx=curve[0].x();
        miny=curve[0].y();
        maxy=curve[0].y();
        for(int i=0;i<this->num;i++){
            if(minx>curve[i].x())   minx=curve[i].x();
            if(miny>curve[i].y())   miny=curve[i].y();
            if(maxx<curve[i].x())   maxx=curve[i].x();
            if(maxy<curve[i].y())   maxy=curve[i].y();
        }
    }
    if(!points.empty()){
        for(vector<QPointF>::iterator i=points.begin();i!=points.end();i++){
            if(minx>(*i).x())   minx=(*i).x();
            if(miny>(*i).y())   miny=(*i).y();
            if(maxx<(*i).x())   maxx=(*i).x();
            if(maxy<(*i).y())   maxy=(*i).y();
        }
    }
    if(minx==maxx){
        minx=(int)minx;
        maxx=minx+1;
    }
    if(miny==maxy){
        miny=(int)miny;
        maxy=miny+1;
    }
    x_scale=chart_width/(maxx-minx);
    y_scale=chart_height/(maxy-miny);

}
void Figure::inputCurve(QPointF *curve, int num){
    if(this->curve==curve) return;
    this->curve=curve;
    this->num=num;
    this->autoScale();
}

void Figure::paintEvent(QPaintEvent *event)
{
    if(curve==0){
        initial();
        return;
    }
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(curveLine);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing,true);

    painter.save();
    painter.translate(original+offset);
    painter.translate(-minx*(x_scale*scale),miny*(y_scale*scale));
    for(int i=0;i<num-1;i++){
        painter.drawLine(QPointF(curve[i].x()*(x_scale*scale),-curve[i].y()*(y_scale*scale))
                        ,QPointF(curve[i+1].x()*(x_scale*scale),-curve[i+1].y()*(y_scale*scale)));
    }

    if(tracer){
        pen.setWidth(2);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(curve[traceIndex].x()*(x_scale*scale),
                                    -curve[traceIndex].y()*(y_scale*scale)),5.0,5.0);
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawPoint(QPointF(curve[traceIndex].x()*(x_scale*scale),
                                  -curve[traceIndex].y()*(y_scale*scale)));
    }

    for(vector<QPointF>::iterator i=points.begin();i!=points.end();i++){
        painter.save();
        painter.translate((*i).x()*(x_scale*scale),-(*i).y()*(y_scale*scale));
        painter.rotate(45);
        painter.drawLine(-5,0,5,0);
        painter.drawLine(0,5,0,-5);
        painter.restore();
    }

    painter.restore();

    painter.eraseRect(0,0,this->width(),upMargin-dickLine);
    painter.eraseRect(0,0,leftMargin-dickLine,this->height());
    painter.eraseRect(this->width()-rightMargin+dickLine,0,rightMargin,this->height());
    painter.eraseRect(0,this->height()+dickLine-downMargin,this->width(),downMargin);

    double x1,x2,y1,y2;
    x1=offset.x()/(x_scale*scale)+minx;
    x2=(offset.x()+chart_width)/(x_scale*scale)+minx;
    y1=offset.y()/(y_scale*scale)+miny;
    y2=(offset.y()+chart_height)/(y_scale*scale)+miny;
    painter.drawText(leftMargin,this->height()-downMargin/2,QString::number(x1));
    painter.drawText(this->width()-3*rightMargin,this->height()-downMargin/2,QString::number(x2));
    painter.drawText(0,upMargin+chart_height,QString::number(y1));
    painter.drawText(0,2*upMargin,QString::number(y2));
    initial();
}


void Figure::timerEvent(QTimerEvent *)
{
    repaint();
}

void Figure::initial()
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(thinLine);
    painter.setPen(pen);

    painter.drawRect(leftMargin-2*dickLine,upMargin-2*dickLine,
                     chart_width+4*dickLine,chart_height+4*dickLine);
    //x
    if(miny<0&&maxy>0){
        double y=original.y()-scale*y_scale*miny;
        painter.drawLine(original.x(),y,original.x()+chart_width,y);
    }else{
        //painter.drawLine(original.x(),original.y(),original.x()+chart_width,original.y());

    }
    //y
    if(minx<0&&maxx>0){
        double x=original.x()+scale*x_scale*minx;
        painter.drawLine(x,original.y(),x,original.y()-chart_height);
    }else{
        //painter.drawLine(original.x(),original.y(),original.x(),original.y()-chart_height);

    }
    if(title!=0)
    {
        painter.setFont(font);
        double w=painter.fontMetrics().width(title);
        double h=painter.fontMetrics().height();
        painter.drawText(this->width()/2.0-w/2.0,upMargin+chart_height+2*h,title);
    }

}

void Figure::popMenuInit()
{
    popMenu=new QMenu(this);
    QAction *reset=new QAction("reset",popMenu);
    popMenu->addAction(reset);
    connect(reset,SIGNAL(triggered()),this,SLOT(reset()));
}

//show popmenu at cursor
void Figure::contextMenuEvent(QContextMenuEvent *)
{
    popMenu->move(cursor().pos());
    popMenu->show();
}

void Figure::reset()
{
    offset.setX(0);
    offset.setY(0);
    scale=1;
    this->autoScale();
    repaint();
}

void Figure::autoMatch()
{
    chart_width=int(this->width()-leftMargin-rightMargin);
    chart_height=int(this->height()-upMargin-downMargin);

    original.setX(leftMargin);
    original.setY(chart_height+upMargin);//+this->height()-downMargin);
    //repaint();
}

void Figure::mousePressEvent(QMouseEvent *event)
{
    if(interactive){
        pressPoint=event->pos();
    }
}

void Figure::mouseMoveEvent(QMouseEvent *event)
{
    if(interactive){
        this->offset=this->offset+event->pos()-pressPoint;
        pressPoint=event->pos();
        repaint();
    }
}

void Figure::wheelEvent(QWheelEvent *event)
{
    if(interactive){
        double delta=event->delta()/1000.0;
        if(this->scale+delta<=0)    return;
        this->scale+=delta;
        repaint();
    }
}



void Figure::grid()
{

}

void Figure::setTitle(QString title)
{
    this->title=title;
}

void Figure::setTracer(bool tracer)
{
    this->tracer=tracer;
}


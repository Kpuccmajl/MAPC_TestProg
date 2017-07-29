#include "graphics.h"
#include <QtGui>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QtMath>
#include <QLabel>

Graphics::Graphics(QWidget *parent)
    :QWidget(parent){
    //Create buttons
    btnDots = new QPushButton(this);
    btnSegments = new QPushButton(this);
    btnCurve = new QPushButton(this);
    btnClear = new QPushButton(this);

    //Set signals->slots
    connect(btnClear, SIGNAL (clicked()), this, SLOT (btnClearClick()));
    connect(btnDots, SIGNAL (clicked()), this, SLOT (btnDotsClick()));
    connect(btnSegments, SIGNAL (clicked()), this, SLOT (btnSegmentsClick()));
    connect(btnCurve, SIGNAL (clicked()), this, SLOT (btnCurveClick()));

    //Set button text
    btnDots->setText(tr("Dots"));
    btnSegments->setText(tr("Segments"));
    btnCurve->setText(tr("Curve"));
    btnClear->setText(tr("Clear"));

    //Create widget and layout for buttons
    wdgBtn = new QWidget(this);
    vlayBtn = new QVBoxLayout(wdgBtn);

    //Set widget and layout for buttons
    vlayBtn->addWidget(btnClear);
    vlayBtn->addWidget(btnDots);
    vlayBtn->addWidget(btnSegments);
    vlayBtn->addWidget(btnCurve);
    wdgBtn->setLayout(vlayBtn);

    //To draw only grid at first window launch
    isDotsPressed = false;
    isSegmentsPressed =false;
    isCurvePressed = false;
}

Graphics::~Graphics() {}

void Graphics::paintEvent(QPaintEvent *){
    QPainter qp(this);
    sortVectorByY(dots);

    //Draw grid everytime, even if Clear pressed
    drawGrid(qp);

    if(isDotsPressed){
        drawDots(qp);
    }
    else if(isSegmentsPressed){
        drawSegments(qp);
        drawDots(qp);
    }
    else if(isCurvePressed){
        drawCurve(qp);
        drawDots(qp);
    }

    if (dots.count() > 15){
        btnCurve->setEnabled(false);
    }

    //Show buttons
    wdgBtn->move(QPoint(640,0));
    wdgBtn->show();
}

//================
// WORK WITH DOTS
//================

void Graphics::initDots(int dotCount){
    while(dotCount>0){
        dots.append(QPoint(rand()%620+5,rand()%470+5));
        --dotCount;
    }
    sortVectorByY(dots);
}

qreal Graphics::calcDistance(QPoint pt1, QPoint pt2){
    return qSqrt(qPow(pt2.x()-pt1.x(),2) + qPow(pt2.y()-pt1.y(),2));
}

void Graphics::search(int x){
    if ((count==n)&&                //if all dots checked
            (a[x][1]!=0)&&          //if there is a path from girt to last dot
            (s+a[x][1]<min)){      //distance is minimum
        min=s+a[x][1];
        for (int i=1;i<=n;i++){
            minm[i]=m[i];
        }
    }
    else{
        for (int i=1;i<=n;i++){     //check all dots
            if ((i!=x)&&
                    (a[x][i]!=0)&&
                    (m[i]==0)&&
                    (s+a[x][i]<min)){
                s+=a[x][i];                // make sum bigge
                count++;                //current count of dots
                m[i]=count;
                search(i);                //search new dot from this
                m[i]=0;                    //going back
                count--;
                s-=a[x][i];
            }
        }
    }
}

void Graphics::sortVectorByY (QVector<QPoint> &vec){
    bool sortIncomplete = true;
    //Sort Y
    while(sortIncomplete){
        //One iteration
        for(int i = 0; i < vec.count() - 1; i++){
            if(vec.at(i).y() > vec.at(i+1).y()){
                QPoint temp = vec.at(i);
                vec.replace(i,vec.at(i+1));
                vec.replace(i+1,temp);
            }
        }
        //Check
        for(int i = 0; i < vec.count() - 1; i++){
            if(vec.at(i).y() > vec.at(i+1).y()){
                sortIncomplete = true;
                break;
            }
            sortIncomplete = false;
        }
    }
    //Sort with the same Y but not the same X
    sortIncomplete = true;
    while(sortIncomplete){
        //Iteration
        for (int i = 0; i < vec.count() - 1 ; i++){
            if ((vec.at(i).y() == vec.at(i+1).y()) &&
                    (vec.at(i).x() > vec.at(i+1).x())){
                QPoint temp = vec.at(i);
                vec.replace(i,vec.at(i+1));
                vec.replace(i+1,temp);
            }
        }
        //Check
        for(int i = 0; i < vec.count() - 1; i++){
            if((vec.at(i).y() == vec.at(i+1).y()) &&
                    (vec.at(i).x() > vec.at(i+1).x())){
                sortIncomplete = true;
                break;
            }
            sortIncomplete = false;
        }
    }
}


//========
// DRAW
//========

void Graphics::drawDots(QPainter &qp){
    //Set pen
    QPen pen(Qt::red,3, Qt::SolidLine);
    qp.setPen(pen);

    QPoint pt;
    foreach (pt, dots) {
        qp.drawPoint(pt);
    }
}

void Graphics::drawGrid(QPainter &qp){
    //Set pen
    QPen penLightGray(Qt::lightGray,0.3, Qt::SolidLine);
    qp.setPen(penLightGray);

    //Grid
    for (int i = 0; i < 100; i++){
        qp.drawLine(QPoint(0,i*10),QPoint(640,i*10));
    }
    for (int i = 0; i < 65; i++){
        qp.drawLine(QPoint(i*10,0),QPoint(i*10,10000));
    }

    QPen penBlack(Qt::black,1, Qt::SolidLine);
    qp.setPen(penBlack);

    //Arrows
    qp.drawLine(QPoint(0,0),QPoint(10000,0));
    qp.drawLine(QPoint(0,0),QPoint(0,10000));
    qp.drawLine(QPoint(640,0),QPoint(620,10));
    qp.drawLine(QPoint(0,480),QPoint(10,460));

    //Segments
    for (int i = 0; i < 47; i++){
        qp.drawLine(QPoint(0,i*10),QPoint(4,i*10));
    }
    for (int i = 0; i < 63; i++){
        qp.drawLine(QPoint(i*10,0),QPoint(i*10,4));
    }

    //XY text and interval length
    qp.drawText(QPoint(630,15),"x");
    qp.drawText(QPoint(10,475),"y");
    qp.drawLine(QPoint(650,470),QPoint(660,470));
    qp.drawLine(QPoint(650,467.5),QPoint(650,472.5));
    qp.drawLine(QPoint(660,467.5),QPoint(660,472.5));
    qp.drawText(QPoint(640,460),"10px");
}


void Graphics::drawCurve(QPainter &qp){
    //Set pen
    QPen pen(Qt::darkBlue,1, Qt::SolidLine);
    qp.setPen(pen);

    QVector<QPoint> curVec; //Final vector
    qreal DISTANCE = 0.0; //Distance of final vector

    // Comivoyager BEGIN

    //Initialize
    s=0;
    n = dots.count();
    min=32767;
    for (int i=1;i<=n;i++){
        m[i]=0;
    }
    count=1;
    m[1]=count;


    //Set matrix of distances
    for (int i = 1; i <= dots.count(); i++){
        for (int j = 1; j <= dots.count(); j++){
            if (i == j){
                a[i][j] = 0;
            }
            else{
                a[i][j] = calcDistance(dots.at(i-1),dots.at(j-1));
                a[j][i] = a[i][j];
            }
        }
    }

    search(1); //Begin comivoyager from 1 dot

    // Comivoyager END

    //Now, when we have path, convert to vector
    int c=1;   //counter
    for (int i= 1 ;i <= dots.count();i++){  //check all dots
        int j= 1;
        while ((j<dots.count()) && (minm[j]!=c)){
            j++;
        }
            curVec.append(dots.at(j-1));
        c++;
    }
    curVec.append(dots.at(0));

    //Find the most big distance between dots
    qreal maxDist = 0, dist;
    for (int i = 0; i < curVec.count() - 1; i++){
        dist = calcDistance(curVec.at(i),curVec.at(i+1));
        if (dist > maxDist){
            maxDist = dist;
        }
    }

    //Now draw our curve
    for (int i = 0; i < curVec.count() - 1; i++){
        //Draw without the longest distance
        if (calcDistance(curVec.at(i),curVec.at(i+1)) != maxDist){
            qp.drawLine(curVec.at(i),curVec.at(i+1));
            DISTANCE += calcDistance(curVec.at(i),curVec.at(i+1));
        }
        //But if there is only two dots
        else if (curVec.count() == 3){ //3 because we actually have 3 dots (first dot repeats)
            qp.drawLine(curVec.at(i),curVec.at(i+1));
            DISTANCE += calcDistance(curVec.at(i),curVec.at(i+1));
        }
    }

    //Draw distance length
    QString str = "Distance";
    qp.drawText(QPoint(660,200),str);
    str = QString::number(DISTANCE);
    str += "px";
    qp.drawText(QPoint(660,215),str);
}

void Graphics::drawSegments(QPainter &qp){
    //Set Pen
    QPen pen(Qt::blue,0.5, Qt::SolidLine);
    qp.setPen(pen);

    QPoint pt;
    bool isMemerized = false; // to get exactly two points
    QPoint memPt; // to mem prev point
    foreach(pt, dots){
        if(!isMemerized){
            memPt = pt;
            isMemerized = true;
        }
        else{
            qp.drawLine(memPt,pt);
            isMemerized = false;
        }
    }
}

//======
// SLOTS
//======

void Graphics::btnClearClick(){
    isDotsPressed = false;
    isSegmentsPressed = false;
    isCurvePressed = false;
    repaint();
}

void Graphics::btnDotsClick(){
    isDotsPressed = true;
    isSegmentsPressed = false;
    isCurvePressed = false;
    repaint();
}

void Graphics::btnSegmentsClick(){
    isSegmentsPressed = true;
    isDotsPressed = false;
    isCurvePressed = false;
    repaint();
}

void Graphics::btnCurveClick(){
    isDotsPressed = false;
    isSegmentsPressed = false;
    isCurvePressed = true;
    repaint();
}


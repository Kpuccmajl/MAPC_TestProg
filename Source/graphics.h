#ifndef DOTSTEST_H
#define DOTSTEST_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class Graphics:public QWidget{

    Q_OBJECT

public:

    Graphics(QWidget *parent = 0);
    ~Graphics();

    void paintEvent(QPaintEvent *);

    QVector<QPoint> dots; //Vector of dots
    void initDots(int dotCount); //Initializing dots with random xy
    void sortVectorByY(QVector<QPoint> &vec); //Sort dots in vector by Y coordinate

private:

    //Drawings
    void drawDots(QPainter &qp);
    void drawSegments(QPainter &qp);
    void drawCurve(QPainter &qp);
    void drawGrid(QPainter &qp);

    qreal calcDistance(QPoint, QPoint); //Calculates distance between two dots

    // For comivoyager algorythm
    static const int maxn=16; //dots max
    int s; //current distance
    int n; //current dots number
    int min; //min distance
    int count; //counter for dots
    qreal a[maxn][maxn];  //matrix of distances
    int m[maxn];    //current path
    int minm[maxn];   //min path

    void search(int);//Comivoyager main phase of searching path

    QWidget *wdgBtn; //Widget for buttons
    QVBoxLayout *vlayBtn; //Layout for buttons
    QPushButton *btnDots;
    QPushButton *btnSegments;
    QPushButton *btnClear;
    QPushButton *btnCurve;

    //To draw exactly what we need
    bool isDotsPressed;
    bool isSegmentsPressed;
    bool isCurvePressed;

public slots:

    void btnClearClick();
    void btnDotsClick();
    void btnSegmentsClick();
    void btnCurveClick();
};


#endif // DOTSTEST_H

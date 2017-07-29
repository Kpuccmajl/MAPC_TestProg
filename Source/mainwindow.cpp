#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "graphics.h"
#include <QFile>

#include <QMainWindow>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSaveXML_clicked()
{
    //Initializing dots
    Graphics *gr = new Graphics;
    gr->initDots(ui->sbDotsNumber->value());

    //Creating new xml file
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save new xml"), ".",
                                                    tr("Xml files (*.xml)"));

    //Setting the new window
    gr->setWindowTitle(filename);
    gr->setFixedHeight(480);
    gr->setFixedWidth(750);

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    //Set xmlWriter
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    //Serialize dots, initialized before, to XML
    xmlWriter.writeStartDocument();
    QPoint qp;
    xmlWriter.writeStartElement("Dots");    // <Dots>
    foreach (qp, gr->dots)
    {
        xmlWriter.writeStartElement("Dot"); // <Dot>
        xmlWriter.writeStartElement("X");   // <X>
        xmlWriter.writeCharacters(QString::number(qp.x())); // value
        xmlWriter.writeEndElement();        // </X>
        xmlWriter.writeStartElement("Y");   // <Y>
        xmlWriter.writeCharacters(QString::number(qp.y())); // value
        xmlWriter.writeEndElement();        // </Y>
        xmlWriter.writeEndElement();        // </Dot>
    }
    xmlWriter.writeEndElement();            // </Dots>
    xmlWriter.writeEndDocument();

    file.close();

    //Create window with dots
    gr->show();
}

void MainWindow::on_btnLoadXML_clicked()
{
    //Create new widget
    Graphics *gr = new Graphics;

    //Opening xml file
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open xml"), ".",
                                                    tr("Xml files (*.xml)"));
    QFile file(filename);

    //Setting the new window
    gr->setWindowTitle(filename);
    gr->setFixedHeight(480);
    gr->setFixedWidth(750);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,
                             "File error",
                             "Can't open file",
                             QMessageBox::Ok);
    }
    else
    {
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);

        xmlReader.readNext();   // pass ROOT elem
        //Loop till doc ends
        int x,y;
        while(!xmlReader.atEnd())
        {
            if(xmlReader.isStartElement() && xmlReader.name() == "X")
            {

                x = xmlReader.readElementText().toInt();

            }
            if(xmlReader.isStartElement() && xmlReader.name() == "Y")
            {

                y = xmlReader.readElementText().toInt();
                gr->dots.append(QPoint(x,y)); // Add new dot

            }
            xmlReader.readNext(); // Next element
        }
    }
    file.close();
    //Show new window
    gr->show();
}


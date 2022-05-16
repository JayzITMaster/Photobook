/*
 * Name: Jahmur Lopez & Carolee Saunders
 * Student ID: 2015112808
 * Class: Graphical User Interface
 * Title: Photo book
 * Due Date: 10/04/2022
 * Due Time: 11:59 pm
 * Lectures: Thiana Vasquez
 * Class Section: 1
*/
#include "imagedialog.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageDialog w;
    w.show();
    return a.exec();
}

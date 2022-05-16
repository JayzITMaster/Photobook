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
#ifndef IMAGECOLLECTION_H
#define IMAGECOLLECTION_H

#include <QImage>
#include <QList>
#include <QStringList>
#include <QBuffer>
#include <QImageWriter>
#include <QImageReader>
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QListWidgetItem>
#include <QMessageBox>

class ImageCollection
{
public:
    ImageCollection(); // class object created
    QImage getImage(int id); // QImage class accepts an index of a picture
    QList<int>getIds(QStringList tags); // stores the list of ids created
    QStringList getTags(); // returns a list of tags created
    void addTag(int id, QString tag); // adds a tag to QListWidget
    void editTag(int id,QString tag); // changes name of a tag for QListWidget
    void deleteImage(int id); // delete image from selected picture
    void deleteTag(int id); // remove tag from DB and QListWidget
    void addImage(QImage image, QStringList tags); // adds an image to the application

private:
    void populateDatabase(); // populates database based on sql query
};

#endif // IMAGECOLLECTION_H

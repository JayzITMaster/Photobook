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
#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QList>
#include <QInputDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QLineEdit>
#include <QRegularExpression>
#include "imagecollection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ImageDialog; }
QT_END_NAMESPACE

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    ImageDialog(QWidget *parent = nullptr); // QWidget parent class created
    ~ImageDialog();

private slots:
    void nextClicked(); // cycles through each picture forward
    void previousClicked(); // cycles through each picture backwards
    void tagsChanged(); // keeps track of any change for each tag
    void addImageClicked(); // allows button to add an image
    void addTagClicked(); // allows a button to add a tag
    void editTagClicked(); // allows a button to change name of a tag
    void deleteTagClicked(); // allows a button to delete selected tag
    void removeImageClicked(); // allows a button to delete selected picutre
    void on_exitButton_clicked(); // allows exit button to close the application

private:
    QStringList selectedTags(); // helper function to store list of selected tags
    void updateImages(); // helper function to update any change for image
    void updateTags(); // helper function to update any change for tags
    void updateDeletedTags(); // helper function to update any tags deleted
    void updateCurrentImage(); // helper function to update any change to selected image

    Ui::ImageDialog *ui; // Ui class object

    QList<int>imageIds; // stores the list of IDs in form of ints
    int currentImage; // indicates the current image selected
    ImageCollection images; // class object created

};
#endif // IMAGEDIALOG_H

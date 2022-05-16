/*
 * Name: Jahmur Lopez & Carolee Saunders
 * Student ID: 2015112808
 * Class: Graphical User Interface
 * Title: Photo book
 * Due Date: 08/04/2022
 * Due Time: 11:59 pm
 * Lectures: Thiana Vasquez
 * Class Section: 1
*/
#include "imagedialog.h"
#include "ui_imagedialog.h"


ImageDialog::ImageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImageDialog)
{
    ui->setupUi(this);

    currentImage = -1; // sets the current image to be negative one

    updateTags(); // calls function to update tags
    updateImages(); // calls function to update images

    connect(ui->previousButton, SIGNAL(clicked()), this, SLOT(previousClicked()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    connect(ui->addTagButton, SIGNAL(clicked()), this, SLOT(addTagClicked()));
    connect(ui->addImageButton, SIGNAL(clicked()),this, SLOT(addImageClicked()));
    connect(ui->editTagButton,SIGNAL(clicked()),this,SLOT(editTagClicked()));
    connect(ui->removeTagButton,SIGNAL(clicked()),this,SLOT(deleteTagClicked()));
    connect(ui->tagList, SIGNAL(itemSelectionChanged()), this,SLOT(tagsChanged()));
    connect(ui->removeImageButton,SIGNAL(clicked()),this,SLOT(removeImageClicked()));
}

ImageDialog::~ImageDialog()
{
    delete ui;
}

void ImageDialog::nextClicked()
{
  if(!imageIds.isEmpty()) // checks if the list of ids was empty or not
  {
      currentImage = (currentImage + 1) % imageIds.count(); // cycles through each image forwards
      updateCurrentImage(); // updates status for the current image
  }
  else
    updateImages(); // call function updateImages
}

void ImageDialog::previousClicked()
{
  currentImage--; // decrement current image variable
  if(currentImage == -1) // checks if the current image is negative 1
      currentImage = imageIds.count() - 1; // cycles through each images backwards
  else
      updateCurrentImage(); // calls function updateCurrentImage
}

void ImageDialog::tagsChanged()
{
  updateImages(); // updates program of any updated tags
}

void ImageDialog::addImageClicked()
{
    // stores name of PNG files
   QString filename = QFileDialog::getOpenFileName(
               this, tr("Open file"), QString(), tr("PNG Images (*.png)"));

   if(!filename.isNull()) // checks if the file name isnt empty
   {
       QImage image(filename); // stores the filename as image

       if(image.isNull()) // checks if the image is empty
       {
           QMessageBox::warning(this, tr("Image Book"), // warns the user
                                tr("Failed to open the file '1%").arg(filename));
           return;
       }

       images.addImage(image,selectedTags()); // adds image to the selected tag
       updateImages(); // calls function to update images
       nextClicked(); // as an image is added it will show the current image
   }
}

void ImageDialog::addTagClicked()
{
  bool ok; // boolean variable created

  // stores the name of a tag created by the user
  QString tag = QInputDialog::getText(this, tr("Photo Book"),tr("Tag:"), QLineEdit::Normal,QString(),&ok);

  if(ok) // checks if the user clicked ok on the QInputDialog
  {
      tag = tag.toLower(); // converts letters to all lower case
      QRegularExpression re(QRegularExpression::anchoredPattern("[a-z]+")); // checks that each letter is from a to z
      QRegularExpressionMatch match = re.match(tag); // matches the selected tag created by the user

      if(!match.hasMatch()) // checks if there's been a match or not
      {
          // warns the user
          QMessageBox::warning(this, tr("Image Book"), tr("This is ot a valid tag."
          "Tags consists of lower case characters a-z."));
          return;
      }
      images.addTag(imageIds[currentImage],tag); // adds name of tag to the DB
      updateTags(); // calls function to update tags
  }
}

void ImageDialog::editTagClicked()
{
    bool ok; // boolean variable created

    // stores a new name of tag created by the user
    QString tag = QInputDialog::getText(this, tr("Photo Book"),tr("Tag:"), QLineEdit::Normal,QString(),&ok);

    if(ok) // checks if the user presses ok
    {
        if(ui->tagList->currentItem()) // checks if there's a tag selected
        {
            tag = tag.toLower(); // converts letters to all lower case
            QRegularExpression re(QRegularExpression::anchoredPattern("[a-z]+")); // checks that each letter is from a to z
            QRegularExpressionMatch match = re.match(tag); // matches the selected tag created by the user

            if(!match.hasMatch()) // checks if there's been a match or not
            {
                // warns the user
                QMessageBox::warning(this, tr("Image Book"), tr("This is ot a valid tag."
                "Tags consists of lower case characters a-z."));
                return;
            }


            ui->tagList->currentItem()->setText(tag); // changes old name of tag to the new one created
            images.editTag(imageIds[currentImage],tag); // edits tag from the database
        }
    }
}

void ImageDialog::deleteTagClicked()
{

    if(ui->tagList->currentItem()) // checks if a current tag is selected
    {
        ui->tagList->takeItem(ui->tagList->currentRow()); // removes tag from the list
        images.deleteTag(imageIds[currentImage]); // deletes tag from the database
    }
    else
    {
        // warns the user
        QMessageBox::warning(this,tr("Warning"),"You'll need to select a tag to remove first");
        return;
    }


}

void ImageDialog::removeImageClicked()
{
   if(ui->tagList->currentItem()) // checks if an image is selected
   {
       if(imageIds[currentImage]) // checks if an image id is selected
       {
           images.deleteImage(imageIds[currentImage]); // removes image from the database
           imageIds.erase(imageIds.begin()+currentImage); // removes image from QListWidget
           nextClicked(); // after current image cycle to the following image
           ui->imagesLabel->setText(QString::number(imageIds.count())); // keeps track of amount of images
       }
       else if(imageIds.isEmpty()) // checks if the list of image IDs are empty
       {
           updateCurrentImage(); // call function updateCurrentImage
       }
       else
           updateCurrentImage(); // call function updateCurrentImage
   }
   else if(imageIds[currentImage]) // checks if there's a current image selected
   {

       if(!imageIds.isEmpty())
       {          
               imageIds.erase(imageIds.begin()+currentImage); // removes image from the database
               images.deleteImage(imageIds[currentImage]);  // removes image from QListWidget
               nextClicked(); // after current image cycle to the following image
               ui->imagesLabel->setText(QString::number(imageIds.count())); // keeps track of amount of images
       }
       else
       {
           updateCurrentImage(); // call function updateCurrentImage
       }

   }
   else
   {
      // warns the user
      QMessageBox::warning(this,tr("Warning"),"Please select a tag name along with an image added");
      return;
   }
}

QStringList ImageDialog::selectedTags()
{
   QStringList result; // QStringList object created

   foreach(QListWidgetItem * item, ui->tagList->selectedItems()) // a foreach loop to loop each tags
       result << item->text(); // adds each selected tag to the list

   return result; // returns the list of selected tags
}

void ImageDialog::updateImages()
{
  int id; // variable created for storing single image IDs

  if(currentImage != -1) // checks if the current Image is not equal -1
      id = imageIds[currentImage]; // stores the current image to the Image Id
  else
      id = -1; // stores id as -1

  imageIds = images.getIds(selectedTags()); // stores id into imageId function
  currentImage = imageIds.indexOf(id); // stores the current index of image iD

  if(currentImage == -1 && !imageIds.isEmpty()) // checks if both current image is -1 and not empty
      currentImage = 0; // stores the current image as 0


  ui->imagesLabel->setText(QString::number(imageIds.count())); // keeps track of the amount of images

  updateCurrentImage(); // calls function updateCurrentImage
}

void ImageDialog::updateTags()
{
  QStringList selection = selectedTags(); // stores selected tags

  QStringList tags = images.getTags(); // stores selected tags for images
  qDebug() << "Tag names: " << tags; // displays list of tags to the console
  ui->tagList->clear(); // clear the list of QListWiget
  ui->tagList->addItems(tags); // adds tag to the QListWidget

  for(int i = 0; i < ui->tagList->count(); ++i) // loops through each item in QListWidget
      if(selection.contains(ui->tagList->item(i)->text())) // checks if the selection had the tag selected
          ui->tagList->item(i)->setSelected(true); // selects the item
}

void ImageDialog::updateDeletedTags()
{
  QStringList removedTags =  images.getTags();
}

void ImageDialog::updateCurrentImage()
{
  if(currentImage == -1) // checks if the current image is empty
  {
      ui->imageLabel->setPixmap(QPixmap()); // preps label to display a blank image
      ui->imageLabel->setText(tr("No Image")); // display label as 'No Image'

      ui->addTagButton->setEnabled(false); // add Tag button is disabled
      ui->nextButton->setEnabled(false); // next button is disabled
      ui->previousButton->setEnabled(false); // previous button is disabled
      ui->removeImageButton->setEnabled(false); // remove image button is disabled
  }
  else
  {
      ui->imageLabel->setPixmap(QPixmap::fromImage(images.getImage(imageIds[currentImage]))); // sets image to current image selected
      ui->addTagButton->setEnabled(true); // enables add tag button
      ui->nextButton->setEnabled(true); // enables next button
      ui->previousButton->setEnabled(true); // enables previous button
      ui->removeImageButton->setEnabled(true); // enables remove image button
  }
}



void ImageDialog::on_exitButton_clicked()
{
    QCoreApplication::quit(); // allows button to close the application
}


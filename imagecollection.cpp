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
#include "imagecollection.h"

ImageCollection::ImageCollection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // QSqlDatabase object to add a database

    //setting connection credentials
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("photobook");

    // attempting to open database connection
    if (!db.open())
    {
           qDebug() << "Failed to connect to database: " << db.lastError().text();
           qFatal("Applicaction Closed!");
    }
    else
    {
        qDebug() << "Connection was successful" << "\n"; // indicates a successful connection
    }

  populateDatabase(); // call function to populate the database
}

QImage ImageCollection::getImage(int id)
{
    QSqlQuery query; // QSqlQuery object created

    query.prepare( "SELECT data FROM images WHERE id = :id" ); // Preparing the sql query select images
    query.bindValue( ":id", id ); // binds valued created by the programmer

    if(query.exec()) // executes the query
        qDebug() << "The get image query is functioning";

    if(query.next()) // query is performing its task
        qDebug() << "Its getting another image";

    QByteArray byteArray = query.value(0).toByteArray(); // saves the each result in an array
    QBuffer buffer(&byteArray); // QBuffer object created to expect a QByteArray object
    buffer.open( QIODevice::ReadOnly ); // opens buffer to handle huge files

    QImageReader reader(&buffer, "PNG"); // reads each image in the buffer
    QImage image = reader.read(); // stores each read image inside of a QImage object

    return image; // returns the image
}

QList<int> ImageCollection::getIds(QStringList tags)
{
  QSqlQuery qry; // QSqlQuery object created
  QList<int> counter; // QList object created to store each Ids

  if(tags.count() == 0) // checks if the number of tags is 0
  {
      qry.prepare("SELECT images.id FROM images"); // preparing sql query to select id from image table
  }
  else
  {
      // prepares an sql query to select ids from tag table
      qry.prepare("SELECT id FROM tags WHERE tag IN ('" + tags.join("','") + "') GROUP BY id");
  }

  if(qry.exec()) // executes the query
      qDebug() << "The get ID function works properly";

  while(qry.next()) //executes the following query
      counter << qry.value(0).toInt();

  return counter; // return list of  ids
}

QStringList ImageCollection::getTags()
{
  QSqlQuery tagsQry; // QSqlQuery object created
  QStringList queryResults; // QStringList object created

  tagsQry.prepare("SELECT DISTINCT tag FROM tags"); // preparing query to select certain tags

  if(!tagsQry.exec()) // checks if the query was executed
      qFatal("Tags query wasn't successful");

  while(tagsQry.next()) // keeps executing following query to store each tag to the list
  {
      queryResults << tagsQry.value(0).toStringList();
  }

  qDebug() << "get tags Query ran successfully"; // indicates to the user that the query was working
  return queryResults; // returns a list of tags
}

void ImageCollection::addTag(int id, QString tag)
{
  QSqlQuery query; // QSqlQuery object query is created

  query.prepare("INSERT INTO tags (id, tag) VALUES (:Id, :Tag)"); // preparing query to insert id and name of tags
  query.bindValue(":Id",id); // binding value for id
  query.bindValue(":Tag",tag); // binding value for tag
  query.exec(); // executes the query

  if(!query.exec()) // checks if the sql query ran properly
      qFatal("nothing was added to the query"); // display error message

}

void ImageCollection::editTag(int id, QString tag)
{
  QSqlQuery query; //QSqlQuery object created

  query.prepare("UPDATE tags set tag = :tag where id= :id"); // preparing sql query to update tag table
  query.bindValue(":tag",tag); // binds value for variable tag
  query.bindValue(":id",id); // binds value for variable id
  query.exec(); // executes the query

  if(query.exec()) // checks if the query was executed or not
      qDebug() << "Selected Tag was changed to: " << tag;
  else
      qDebug() << "Not tag was changed from the database";

}

void ImageCollection::deleteTag(int id)
{
  QSqlQuery deleteQuery; //QSqlQuery object created

  deleteQuery.prepare("DELETE FROM tags WHERE id = :id"); // preparing sql query to delete tags
  deleteQuery.bindValue(":id",id); // binds value for variable id
  deleteQuery.exec(); // executes the query

  if(!deleteQuery.exec()) // checks if the query was executed or not
      qDebug() << "Nothing was deleted from the tag table";
}

void ImageCollection::deleteImage(int id)
{
  QSqlQuery deleteImageQuery; //QSqlQuery object created

   deleteImageQuery.prepare("DELETE FROM image WHERE id = :id"); // preparing sql query to delete ids
   deleteImageQuery.bindValue(":id",id); // binds value for variable id
   deleteImageQuery.exec(); // executes the query

   if(!deleteImageQuery.exec()) // checks if the query was executed or not
          qDebug() << "No image was deleted";


}

void ImageCollection::addImage(QImage image, QStringList tags)
{
  QBuffer buffer; // QBuffer object created
  QImageWriter writer(&buffer,"PNG"); // QImageWriter created to handle png file sizes

  writer.write(image); // writes the image

  QSqlQuery qry; // QSqlQuery object created
  int id; // variable to store single id from image table

  qry.prepare("SELECT id FROM images ORDER BY id DESC LIMIT 0,1"); // prepares query for selecting ids from images table
  qry.exec(); // exececutes query
  qry.next(); // executes the following query
  id = qry.value(0).toInt() + 1; // stores the id from the database

  qry.prepare("INSERT INTO images(id,data) VALUES (:id, :data)"); // prepares another query to insert data
  qry.bindValue(":id",id); // binds value for id
  qry.bindValue(":data",buffer.data()); // binds value for data buffer
  qry.exec(); // executes the query

  foreach(QString tag, tags) // looping through each tags in QStringList
      addTag(id,tag); // calls function to add tag with respective arguments
}

void ImageCollection::populateDatabase()
{
   QSqlQuery imagesQry; // first QSqlQuery object created for image table
   QSqlQuery tagsQry; // second QSqlQuery object created for tags table

   // prepares query to create images table
   imagesQry.prepare("CREATE TABLE IF NOT EXISTS images(id INTEGER UNIQUE PRIMARY KEY, data longblob);");

   // prepares query to create tags table
   tagsQry.prepare("CREATE TABLE IF NOT EXISTS tags(id INTEGER NOT NULL, tag VARCHAR(30));");

   if(!imagesQry.exec()) // checks if the query was executed for images table or not
       qDebug() << "Failed to create table 'images'";
   else
       qDebug() << "Table 'images' has been created";

   if(!tagsQry.exec()) // checks if the query was executed for tags table or not
       qDebug() << "Failed to create table'tags'";
   else
       qDebug() << "Table 'tags' has been created";

}

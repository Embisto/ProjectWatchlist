#ifndef XMLPICTURELOADER_H
#define XMLPICTURELOADER_H
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>

#include <QObject>

/*
 *  XmlPictureLoader Class for parsing xml-file downloaded from TvRage API:
 *  Loads http://services.tvrage.com/feeds/full_show_info.php?sid=SHOW_ID and extracts ImageUrl
 */

class XmlPictureLoader : public QObject
{
    Q_OBJECT

public:

    explicit XmlPictureLoader(QObject *parent = 0);
    ~XmlPictureLoader();
    QNetworkAccessManager* nam;
    void createConnection(QString showId);
    QString imageUrl;
    QDomNodeList list;

signals:

    void imageUrlLoaded(const QString& imageUrl);
    
public slots:

    void finishedSlot(QNetworkReply*);
    
};

#endif // XMLPICTURELOADER_H

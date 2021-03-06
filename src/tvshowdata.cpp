#include "tvshowdata.h"
#include "database.h"
#include <QtDebug>



TvShowData::TvShowData(QObject *parent)
    : QAbstractListModel(parent)
{
    showIndex=0;
    QHash<int, QByteArray> roles;
    roles[TitleRole]   = "title";
    roles[SeasonRole]  = "season";
    roles[EpisodeRole] = "episode";
    roles[NewEpisodeAvailableRole] = "newEpisodeAvailable";
    roles[StartedRole] = "started";
    roles[StatusRole]  = "status";
    roles[AirtimeRole] = "airtime";
    roles[NetworkRole] = "network";
    roles[LatestEpisodeRole] = "latestEpisode";
    roles[NextEpisodeRole]   = "nextEpisode";
    roles[ImageUrlRole] = "imageUrl";
    roles[SortRole] = "sortRole";
    setRoleNames(roles);

    QObject::connect(this, SIGNAL(allDataLoaded(TvShow*)), this, SLOT(checkForNewEpisodes(TvShow*)));
    QObject::connect(this, SIGNAL(allDataLoaded(TvShow*)), this, SLOT(updateLoadedCount(TvShow*)));
}

TvShowData::~TvShowData()
{
    qDeleteAll(shows.begin(), shows.end());
    shows.clear();
}


// First creates new TvShow out of name, then inserts it
int TvShowData::addShow(const QString &name)
{
    TvShow* insert = new TvShow(name);
    return addShow(*insert);
}


// Tries to find given Show in the vector, and adds it if not found.
// Returns the Index of the Show if it is already present (useful for later highlighting?)
int TvShowData::addShow(TvShow& show)
{
    int index = findShowIndex(show.getTitle());
    // -1 means the Show wasn�t found
    if (index == -1)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        shows.append(&show);
        endInsertRows();
        emit newShowAdded();
        checkForNewEpisodes(show.getTitle());
    }

    return index;
}


// Finds Shows position in Vector through its name
int TvShowData::findShowIndex(const QString& name)
{
    for(int i = 0; i < shows.size(); i++)
    {
        if (shows.at(i)->getTitle() == name)
            return i;
    }
    return -1;
}

/*
// Finds Shows position in QList through whole Show-Object
int TvShowData::findShowIndex(const TvShow &show)
{
    return findShowIndex(show.getTitle());
}
*/


// Removes show from vector via its name. Returns -1 if show wasn�t found.
int TvShowData::removeShow(const QString &name)
{
    int index = findShowIndex(name);

    if (index != -1)
    {

        beginRemoveRows(QModelIndex(), index, index);
        delete shows.at(index);
        shows.erase(shows.begin() + index);
        endRemoveRows();
    }
    return index;
}

/*
// Removes show via object
int TvShowData::removeShow(const TvShow &show)
{

    return removeShow(show.getTitle());
}
*/

// Updates Season
void TvShowData::setSeason(const QString &name, int delta)
{
    int index = findShowIndex(name);

    if (index != -1)
    {

        // Changes in Model
        shows.at(index)->setSeason(delta);

        // Visualize changes
        QModelIndex modindex = createIndex(index, 1);
        emit dataChanged(modindex, modindex);

    }
}


// Updates Episode
void TvShowData::setEpisode(const QString &name, int delta)
{
    int index = findShowIndex(name);

    if (index != -1)
    {

        // Changes in Model
        shows.at(index)->setEpisode(delta);

        // Visualize changes
        QModelIndex modindex = createIndex(index, 1);
        emit dataChanged(modindex, modindex);

    }
}

// Updates showName
void TvShowData::alterShowName(const QString &oldName, const QString &newName)
{
    int index = findShowIndex(oldName);

    if (index != -1)
    {

        // Changes in Model
        shows.at(index)->setTitle(newName);

        // Visualize changes
        QModelIndex modindex = createIndex(index, 1);
        emit dataChanged(modindex, modindex);

    }
}

QString TvShowData::getStarted(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getStarted();
    else
    {
        qDebug() << "ERROR when getting Started from Show "+name;
        return "ERROR when getting Started from Show "+name;
    }
}

QString TvShowData::getStatus(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getStatus();
    else
    {
        qDebug() << "ERROR when getting Status from Show "+ name;
        return "ERROR when getting Status from Show "+ name;
    }
}

QString TvShowData::getAirtime(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getAirtime();
    else
    {
        qDebug() << "ERROR when getting Airtime from Show "+ name;
        return "ERROR when getting Airtime from Show "+ name;
    }
}

QString TvShowData::getNetwork(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getNetwork();
    else
    {
        qDebug() << "ERROR when getting Network from Show "+ name;
        return "ERROR when getting Network from Show "+ name;
    }
}

QString TvShowData::getGenre(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getGenre();
    else
    {
        qDebug() << "ERROR when getting Genre from Show "+ name;
        return "ERROR when getting Genre from Show "+ name;
    }
}

QString TvShowData::getLatestEp(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getLatestEpisode();
    else
    {
        qDebug() << "ERROR when getting Latest Ep from Show "+ name;
        return "ERROR when getting Latest Ep from Show "+ name;
    }
}

QString TvShowData::getNextEp(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getNextEpisode();
    else
    {
        qDebug() << "ERROR when getting next ep from Show "+ name;
        return "ERROR when getting next ep from Show "+ name;
    }
}

QString TvShowData::getImageUrl(const QString &name)
{
    int index = findShowIndex(name);
    if (index != -1)
        return shows.at(index)->getImageUrl();
    else
    {
        qDebug() << "ERROR when getting imageurl from Show "+ name;
        return "ERROR when getting imageurl from Show "+ name;
    }
}

void TvShowData::editShow(const QString &oldName, const QString &newName)
{
    int index = findShowIndex(oldName);

    if (index != -1)
    {
        TvShow* edit = shows.at(index);

        QObject::connect(edit, SIGNAL(showEdited(bool,const QString&)), this, SLOT(onShowEdited(bool,const QString&)));
        edit->editShow(newName);
    }
    else
    {
        qDebug() << "ERROR when trying to edit Show " + oldName + ". It was not found in Database";
    }
}

void TvShowData::editComplete(const QString &oldName, const QString &newName)
{
    int index = findShowIndex(newName);
    if (index != -1)
    {
        TvShow* editted = shows.at(findShowIndex(newName));
        emit showEditedDBUpdate(editted, oldName);
        QObject::disconnect(editted, SIGNAL(showEdited(bool,const QString &)),this, SLOT(onShowEdited(bool, const QString &)));
    }
    else
    {
        qDebug() << "ERROR when trying to disconnect Edit Signal after editting";
    }
}



// Returns Debug-String
QString TvShowData::toString() const
{
    QString result("");

    for (int i = 0; i < shows.size(); i++)
    {
        result.append(shows.at(i)->toString());
        result.append("\n");
    }

    return result;
}

void TvShowData::updateShowAtCurrentIndex()
{
    TvShow* show = shows.at(showIndex);
    QObject::connect(show, SIGNAL(allDataLoaded(TvShow*)), this, SIGNAL(allDataLoaded(TvShow*)));
    QObject::connect(show, SIGNAL(htmlErrorOccured()), this, SIGNAL(htmlErrorOccured()));
    show->getExtraInformation();
}

void TvShowData::getExtraInformation()
{
    showIndex = 0;
    if (shows.size() > 0)
    {
        updateShowAtCurrentIndex();
    }

/*

    for(int i=0; i < shows.size(); i++)
    {
        TvShow* show = shows.at(i);
        QObject::connect(show, SIGNAL(allDataLoaded(TvShow*)), this, SIGNAL(allDataLoaded(TvShow*)));
        QObject::connect(show, SIGNAL(htmlErrorOccured()), this, SIGNAL(htmlErrorOccured()));
        show->getExtraInformation();
    }
    */
}


// Required by QAbstractListModel - Interface
int TvShowData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return shows.size();
}



// Required by QAbstractListModel - Interface
QVariant TvShowData::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > shows.size())
        return QVariant();

   const TvShow &show = *(shows.at(index.row()));

   switch (role)
   {
   case TitleRole:
       return show.getTitle();

   case SeasonRole:
       return show.getSeason();

   case EpisodeRole:
       return show.getEpisode();

   case NewEpisodeAvailableRole:
       return show.getNewEpisodesAvailable();

   case StartedRole:
       return show.getStarted();

   case StatusRole:
       return show.getStatus();

   case AirtimeRole:
       return show.getAirtime();

   case NetworkRole:
       return show.getNetwork();

   case LatestEpisodeRole:
       return show.getLatestEpisode();

   case NextEpisodeRole:
       return show.getNextEpisode();

   case ImageUrlRole:
       return show.getImageUrl();

   case SortRole:
       return show.getNextEpisodeDate();

   default:
       return QVariant();
   }
}


// When Database is loaded, API-Information is retrieved/updated
void TvShowData::onDbLoaded()
{
    if (this->shows.size() > 0)
    {
        getExtraInformation();
    }
}

void TvShowData::checkForNewEpisodes(TvShow *show)
{
    checkForNewEpisodes(show->getTitle());
}

void TvShowData::updateLoadedCount(TvShow* show)
{
    QObject::disconnect(show, SIGNAL(allDataLoaded(TvShow*)), this, SIGNAL(allDataLoaded(TvShow*)));
    QObject::disconnect(show, SIGNAL(htmlErrorOccured()), this, SIGNAL(htmlErrorOccured()));

    if(++showIndex == shows.size())
    {
        emit everyShowLoaded();
        qDebug() << "Network Update finished without errors.";
    }
    else
    {
        updateShowAtCurrentIndex();
    }
}

void TvShowData::onShowEdited(bool success, const QString& name)
{
    if (success)
    {
        emit showEditSuccess();
    }
    else
    {
        emit showEditFailure();
    }

}


void TvShowData::checkForNewEpisodes(const QString &show)
{
    int index = findShowIndex(show);
    shows.at(index)->checkForNewEpisodes();

    // Visualize Changes
    QModelIndex modIndex = createIndex(index, 1);
    emit dataChanged(modIndex, modIndex);
}

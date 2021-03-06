#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QSettings>
#include "quickinfo.h"
#include "database.h"
#include "logger.h"
#include "settingscontroller.h"


class MainView;
class QDeclarativeView;
class QVBoxLayout;
class NcFramelessHelper;
class QApplication;
class UIController;
class TvShowProxyModel;

/*
 * Interface Class between GUI and Data. Handles most of the User Requests regarding
 * the underlying data model.
 */

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QApplication *app, QObject *parent = 0);
    ~Controller();

    //database functions
    Q_INVOKABLE void add(const QString& name);
    Q_INVOKABLE void remove(const QString& name);
    Q_INVOKABLE bool alterShowName(const QString& oldName,const QString& newName);
    Q_INVOKABLE void setSeason(const QString& name, int delta);
    Q_INVOKABLE void setEpisode(const QString& name, int delta);
    Q_INVOKABLE void loadDB();
    Q_INVOKABLE void checkForNewEpisodes(const QString &title);
    Q_INVOKABLE void manualNetworkUpdate();

    //quickInfo functions
    Q_INVOKABLE QString getStarted(const QString& name) {return db->data->getStarted(name);}
    Q_INVOKABLE QString getStatus(const QString& name) {return db->data->getStatus(name);}
    Q_INVOKABLE QString getAirtime(const QString& name) {return db->data->getAirtime(name);}
    Q_INVOKABLE QString getNetwork(const QString& name) {return db->data->getNetwork(name);}
    Q_INVOKABLE QString getGenre(const QString& name) {return  db->data->getGenre(name);}
    Q_INVOKABLE QString getLatestEpisode(const QString& name) {return db->data->getLatestEp(name);}
    Q_INVOKABLE QString getNextEpisode(const QString& name) {return db->data->getNextEp(name);}
    Q_INVOKABLE QString getImageUrl(const QString& name) {return db->data->getImageUrl(name);}

    Q_INVOKABLE void editShowName(const QString& oldName, const QString& newName);
    Q_INVOKABLE void editComplete(const QString& oldName, const QString &newName);


    Q_INVOKABLE int find(const QString& name) { return db->data->findShowIndex(name); }

    //window functions
    Q_INVOKABLE void windowMaximize();
    Q_INVOKABLE void windowMinimize();
    Q_INVOKABLE void windowHide();

    void initMsgHandler();
    void initializeProxyModel();

    QWidget *mainWidget;
    Database *db;
    QDeclarativeView  *qmlView;
    NcFramelessHelper *framelessHelper;
    QVBoxLayout *layout;
    UIController *uicontroller;
    TvShowProxyModel *proxyModel;
    SettingsController *settingsController;

};

#endif // CONTROLLER_H

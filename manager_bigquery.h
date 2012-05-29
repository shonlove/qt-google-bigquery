#ifndef MANAGER_BIGQUERY_H
#define MANAGER_BIGQUERY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

class ManagerBigQuery : public QObject
{
    Q_OBJECT
public:
    explicit ManagerBigQuery(QObject *parent = 0);

    void setAPIKey(QString str)   {m_API_Key = str;};
    void setAccessToken(QString str)   {m_Access_Token = str;};

//functions for Google Big Query API
public:
    void getDatasetsList(QString strProjectID);
    void getTablesListForDataset(QString strProjectID, QString strDataset);
    void getColumnsForTable(QString strProjectID, QString strDataset, QString strTable);
    void geTableData(QString strProjectID, QString strDataset, QString strTable);
    void getQuery(QString strProjectID, QString strQuery);

signals:
    void replyText(const QString& text);
    void errorOccured(const QString& error);
    void recvDatasetsList(const QVariantList& list);
    void recvTablesList(const QVariantList& list);
    void recvColumnsList(const QVariantList& list);
    void recvTableData(const QVariantList& list);

public slots:
    void replyFinished(QNetworkReply* reply);
private:
    QNetworkAccessManager* m_pNetworkAccessManager;
    QString m_API_Key;
    QString m_Access_Token;
};

#endif // MANAGER_BIGQUERY_H

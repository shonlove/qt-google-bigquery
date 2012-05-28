#include "manager_bigquery.h"
#include <QDebug>
#include <QJson/Parser>
#include <QApplication>


ManagerBigQuery::ManagerBigQuery(QObject *parent) :
    QObject(parent)
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void ManagerBigQuery::getDatasetsList(QString strProjectID)
{
    QString url = QString("https://www.googleapis.com/bigquery/v2/projects/%1/datasets").arg(strProjectID);
    //QString url = QString("https://www.googleapis.com/bigquery/v2/projects");

    QNetworkRequest request;
    request.setUrl( QUrl(url) );
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", (QString("Bearer %1").arg(m_Access_Token)).toLatin1());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(request);

    qDebug() << "getPredict, Send URL === " << url;
}

void ManagerBigQuery::getTablesListForDataset(QString strProjectID, QString strDataset)
{
    QString url = QString("https://www.googleapis.com/bigquery/v2/projects/%1/datasets/%2/tables").arg(strProjectID).arg(strDataset);

    QNetworkRequest request;
    request.setUrl( QUrl(url) );
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", (QString("Bearer %1").arg(m_Access_Token)).toLatin1());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(request);

    qDebug() << "getPredict, Send URL === " << url;
}

void ManagerBigQuery::getColumnsForTable(QString strProjectID, QString strDataset, QString strTable)
{
    QString url = QString("https://www.googleapis.com/bigquery/v2/projects/%1/datasets/%2/tables/%3")
            .arg(strProjectID).arg(strDataset).arg(strTable);

    QNetworkRequest request;
    request.setUrl( QUrl(url) );
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", (QString("Bearer %1").arg(m_Access_Token)).toLatin1());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(request);

    qDebug() << "getPredict, Send URL === " << url;
}

void ManagerBigQuery::geTableData(QString strProjectID, QString strDataset, QString strTable)
{
    QString url = QString("https://www.googleapis.com/bigquery/v2/projects/%1/datasets/%2/tables/%3/data?maxResults=10")
            .arg(strProjectID).arg(strDataset).arg(strTable);

    QNetworkRequest request;
    request.setUrl( QUrl(url) );
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", (QString("Bearer %1").arg(m_Access_Token)).toLatin1());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(request);

    qDebug() << "getPredict, Send URL === " << url;
}

void ManagerBigQuery::replyFinished(QNetworkReply *reply)
{
    QApplication::restoreOverrideCursor();
    QString json = reply->readAll();
    qDebug() << "Reply = " << json;
    qDebug() << "URL = " << reply->url();
    QString strUrl = reply->url().toString();

    emit replyText(json);

    QJson::Parser parser;

    bool ok;

    // json is a QString containing the data to convert
    QVariant result = parser.parse (json.toLatin1(), &ok);
    if(!ok)
    {
        emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
        return;
    }

    QString queryKind = result.toMap()["kind"].toString();
    qDebug() << "!!!!!!!!!!!" << queryKind;
    if (queryKind == "bigquery#datasetList")
    {
        QVariantList _varList = result.toMap()["datasets"].toList();
        qDebug() << _varList;
        emit recvDatasetsList(_varList);
        return;
    }

    if (queryKind == "bigquery#tableList")
    {
        QVariantList _varList = result.toMap()["tables"].toList();
        qDebug() << _varList;
        emit recvTablesList(_varList);
        return;
    }
    if (queryKind == "bigquery#table")
    {
        QVariant var = result.toMap()["schema"];
//        qDebug() << var;
        QVariantList _varList = var.toMap()["fields"].toList();
        qDebug() << _varList;
        emit recvColumnsList(_varList);
        return;
    }
    if (queryKind == "bigquery#tableDataList")
    {
        QVariantList _varList = result.toMap()["rows"].toList();
        qDebug() << _varList.size();
        qDebug() << _varList;

        emit recvTableData(_varList);
        return;
    }



}

#ifndef TESTDATA_H
#define TESTDATA_H

#include <QObject>
#include <QVector>


class TestData : public QObject
{
    Q_OBJECT
public:
    explicit TestData(QString filename, QObject *parent = 0);

    QString getFileName();


public slots:
    void setFileName(QString filename);
    void addData(QVector<float> datapoint);
    void addData(int time, float mm, float newtons);
    bool writeToFile();

signals:
    void wroteToFile();
    void errorInFileWrite();



private:
    QVector< QVector<float> > data_;
    QString filename_;
};

#endif // TESTDATA_H

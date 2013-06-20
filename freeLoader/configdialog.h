#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QStringList configs, QWidget *parent = 0);
    ~ConfigDialog();

public slots:
    void selectionMade();

signals:
    void selected(QString);

private:
    Ui::ConfigDialog *ui;
    QStringList configs_;
};

#endif // CONFIGDIALOG_H

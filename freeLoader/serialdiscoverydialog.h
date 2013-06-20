#ifndef SERIALDISCOVERYDIALOG_H
#define SERIALDISCOVERYDIALOG_H

#include <QDialog>

namespace Ui {
class SerialDiscoveryDialog;
}

class SerialDiscoveryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SerialDiscoveryDialog(QWidget *parent = 0);
    ~SerialDiscoveryDialog();
    
private:
    Ui::SerialDiscoveryDialog *ui;
};

#endif // SERIALDISCOVERYDIALOG_H

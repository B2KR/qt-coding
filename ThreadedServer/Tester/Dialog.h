#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit Dialog(QWidget *parent = 0);
        ~Dialog();
        QString getUser() const { return nif; }
        
    private:
        Ui::Dialog *ui;
        QString nif;

    private slots:
        void slotUserLogged();
};

#endif // DIALOG_H

#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(slotUserLogged()));
    connect(this, SIGNAL(rejected()), this, SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotUserLogged()
{
    nif = ui->nif->text();
}

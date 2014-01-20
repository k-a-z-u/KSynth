#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
	QDialog(parent), ui(new Ui::ProgressDialog) {

	ui->setupUi(this);

}

ProgressDialog::~ProgressDialog() {
	delete ui;
}

void ProgressDialog::setText(const std::string& msg) {
	ui->lblMsg->setText(msg.c_str());
}

void ProgressDialog::setProgress(float percent) {
	ui->bar->setValue( int(percent * 100.0f) );
}

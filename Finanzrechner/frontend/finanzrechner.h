#pragma once

#include "stdafx.h"
#include "ui_finanzrechner.h"

class Finanzrechner : public QMainWindow {
	Q_OBJECT

public:
	Finanzrechner(QWidget *parent = Q_NULLPTR);

private:
	Ui::FinanzrechnerClass ui;
};

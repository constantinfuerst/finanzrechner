#pragma once

#include "stdafx.h"
#include "ui_finanzrechner.h"

class financecalc final : public QMainWindow {
	Q_OBJECT

public:
	financecalc(QWidget *parent = Q_NULLPTR);

private:
	Ui::FinanzrechnerClass m_ui;
};

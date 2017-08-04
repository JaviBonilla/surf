/*
*    This file is part of Surf
*
*    Surf - Simulation of FMUs
*    Copyright (C) 2017 - Javier Bonilla
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
}

void cursorBegining(QTextEdit *qte)
{
    QTextCursor cursor = qte->textCursor();
    cursor.setPosition(0);
    qte->setTextCursor(cursor);
}

void about::setDefaultGUI()
{
   ui->tabWidget->setCurrentIndex(0);
   ui->tabSoftware->setCurrentIndex(0);
   cursorBegining(ui->tDescription);
   cursorBegining(ui->tLicense_2);
   cursorBegining(ui->tVersion_2);
}

about::~about()
{
    delete ui;
}

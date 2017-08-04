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
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (!w.getInitialized())
        return 0;
    return a.exec();
}

// ---------------------------------------------------------//
// WARNING: This declarations must be deleted,              //
// they are dependencies from opt_problem.cpp             //
// ---------------------------------------------------------//

class opt_method;

void getParameterEstimationList(QList<opt_method> &lmethods)
{
    Q_UNUSED(lmethods);
}
// ---------------------------------------------------------//

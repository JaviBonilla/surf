/*
*    This file is part of simint
*
*    simint - Simulation Interface
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
#ifndef SVGVIEWER_H
#define SVGVIEWER_H

#include <QMainWindow>
#include <QString>
#include <QDomDocument>
#include <QMenu>
#include <QSvgWidget>

namespace Ui {
class SVGViewer;
}

class SVGViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit SVGViewer(QWidget *parent = 0);
    ~SVGViewer();
    void loadFile(QString filename);
    void setIcon(QIcon i);
    void setSize();
    QDomDocument getDoc() const;
    void setDoc(const QDomDocument &value);
    void refresh();

private:
    Ui::SVGViewer *ui;
    //QIcon svg_icon;
    bool set_svg_icon;
    double aspect_ratio;
    QDomDocument doc;
    QMenu cmSVG;
    QSvgWidget *view;

private slots:
    bool eventFilter(QObject *obj, QEvent *event);
    void on_actionClipboard_triggered();
    void on_actionSave_triggered();
    void on_SVGViewer_customContextMenuRequested(const QPoint &pos);
};

#endif // SVGVIEWER_H

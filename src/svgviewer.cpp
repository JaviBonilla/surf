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
#include "svgviewer.h"
#include "ui_svgviewer.h"

#include <QFile>
#include <QClipboard>
#include <QFileDialog>
#include <QSvgRenderer>
#include <QLayout>
#include <QXmlInputSource>

#include "commonapp.h"
#include "common.h"
#include "commonsimint.h"

const QString sbWhite = "background-color: white;";

SVGViewer::SVGViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SVGViewer)
{
    ui->setupUi(this);

    view = new QSvgWidget(this);

    setStyleSheet(sbWhite);
    view->setStyleSheet(sbWhite);
    set_svg_icon = false;

    setCentralWidget(view);

    cmSVG.addAction(ui->actionClipboard);
    cmSVG.addAction(ui->actionSave);
}

SVGViewer::~SVGViewer()
{
    delete view;
    delete ui;
}

void SVGViewer::loadFile(QString filename)
{
    if (!set_svg_icon)
    {
        // Load XML file
        XML_to_QDom(filename,doc);

        // Aspect ratio
        aspect_ratio = view->width()/view->height();

        // Icon set
        set_svg_icon = true;

        // Instal filter
        installEventFilter(this);

        // Set size
        SVG_setSize(view,aspect_ratio,width(),height());
    }
}

QDomDocument SVGViewer::getDoc() const
{
    return doc;
}

void SVGViewer::setDoc(const QDomDocument &value)
{
    doc = value;
}

void SVGViewer::refresh()
{
    SVG_refresh(doc,view,width(),height(),aspect_ratio);
}

bool SVGViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == objectName())
    {
            if (event->type() == QEvent::Resize)
                SVG_setSize(view,aspect_ratio,width(),height());
    }

    // To process other events
    return false;
}

void SVGViewer::on_actionClipboard_triggered()
{
    imageToImg(getImage(view),true,sEmpty);
}

void SVGViewer::on_actionSave_triggered()
{
    saveSVGimage(this,doc,view);

//    QFileDialog dlg(this, mImagFile, sEmpty, PDFFilter + ";;" + SVGFilter + ";;" + PNGFilter + ";;" + BMPFilter + ";;" + JPGFilter + ";;" + JPEGFilter);

//    dlg.setAcceptMode(QFileDialog::AcceptSave);
//    dlg.setOption(QFileDialog::DontConfirmOverwrite,false);

//    if (dlg.exec())
//    {
//        if (dlg.selectedNameFilter() == PDFFilter)       domDocToPDF(doc,dlg.selectedFiles()[0],view->widthMM(),view->heightMM());
//        else if (dlg.selectedNameFilter() == SVGFilter)  domDocToSVG(doc,dlg.selectedFiles()[0]);
//        else                                             imageToImg(getImage(view),false,dlg.selectedFiles()[0]);
//    }
}

void SVGViewer::on_SVGViewer_customContextMenuRequested(const QPoint &pos)
{
    cmSVG.exec(mapToGlobal(pos));
}

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
#include "ui_mainwindow.h"

#include <QErrorMessage>
#include <QApplication>
#include <QSvgRenderer>
#include <QDebug>

#include "commonapp.h"
#include "graphconfig.h"
#include "seriesconfig.h"
#include "integrator.h"
#include "outputs.h"

const QString sVer = " - ver. ";
const QString sExp = "Experiment - ";

const QString simSep   = " / ";
const QString simUnits = " (s)";
const QString simSeconds = " seconds.";

const QString mSimError    = "Simulation error: ";
const QString mSimFinished = "Simulation finished, execution time: ";
const QString mSimStopped  = "Simulation stopped";

const QString ssColorGreen  = "color: green;";
const QString ssColorRed    = "color: red;";
const QString ssColorOrange = "color: orange;";

const QString cTitle_def      = "#2b3443";
const QString cBackground_def = "#edf2f6";
const QString cHeader_def     = "#6f7e95";
const QString cFontHeader_def = "#ffffff";
const QString cFontTitle_def  = "#ffffff";

const QString PRO_DIAGRAM = "PRO_DIAGRAM";

const QString ID_MAIN_WIN   = "MAIN_WINDOW";
const QString ID_SVG_WIN    = "SVG_WINDOW";
const QString ID_PAR_TREE   = "PAR_TREE";
const QString ID_RES_TABLE  = "RES_TABLE";
const QString ID_SPLITTER   = "SPLITTER";
const QString ID_SPLITTER1  = "SPLITTER1";
const QString ID_SPLITTER2  = "SPLITTER2";
const QString ID_SPLITTER3  = "SPLITTER3";
const QString ID_SPLITTERD  = "SPLITTER_";
const QString ID_CASE       = "CASE";
const QString ID_GRAPHS_TAB = "GRAPHS_TAB";
const QString ID_DOC_TAB    = "DOC_TAB";
const QString ID_EXP        = "EXP";

const QString DOC_REF       = "DOC_REF";
const QString DOC_DT        = "DOC_DT";

const int HANDLE_WIDTH     = 7;
const int SVG_RIGHT_MARGIN = 10;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //qDebug() << QStyleFactory::keys();
    //qApp->setStyle("Windows"); // Breeze, Windows, Fusion

    QString msg;

    // --------------------------- //
    // Initialize global variables //
    // --------------------------- //
    initialized = true;
    thrSim      = NULL;
    gau         = NULL;
    rl          = new RenderLinkDelegate;
    antPos      = 0;
    plot        = new plotgraphs();
    newResults  = false;

    ui->setupUi(this);

    // ------------------------------------ //
    // Register meta types for signal/slots //
    // ------------------------------------ //
    qRegisterMetaType<opt_exp>("opt_exp");
    qRegisterMetaType<opt_problem>("opt_problem");

    // ------------------- //
    // Configure GUI style //
    // ------------------- //
    ui->fTitle->setStyleSheet(sEmpty);
    ui->hParameters->setStyleSheet(sEmpty);
    ui->hGraphs->setStyleSheet(sEmpty);
    ui->hSimulation->setStyleSheet(sEmpty);
    ui->hDocumentation->setStyleSheet(sEmpty);
    ui->tParameters->header()->setStretchLastSection(true);
    ui->tRes->horizontalHeader()->setStretchLastSection(true);

    // ----------------------- //
    // Configure context menus //
    // ----------------------- //
    configureConMenus();

    // ------------- //
    // Read XML data //
    // ------------- //
    if (!xmlSimIntParser(msg,dir.path(),xmlData))
    {
        QErrorMessage *error = new QErrorMessage(this);

        error->showMessage(msg);
        error->exec();
        initialized = false;
        delete error;
        return;
    }

    // ----------------------------------- //
    // Set GUI configuration from XML data //
    // ----------------------------------- //

    // Apply style
    setAppStyleSheet();

    // Configure app
    configureApp();

    // Geometry
    configureGeometry();

    // Only one selected action in the group
    act_exps = new QActionGroup(ui->menuExperiment);
    act_exps->setExclusive(true);

    // Menu for all experiments & descriptions
    for(int i=0;i<xmlData.getExps().count();i++)
    {
        experiment e = xmlData.getExps()[i];
        QMenu *menu = NULL;

        // If there is group, first find it otherwise create a new one
        if (!e.getGroup().trimmed().isEmpty())
        {
            foreach (QAction *action, ui->menuExperiment->actions())
            {
                if (action->menu() && action->text() == e.getGroup())
                {
                    menu = action->menu();
                    break;
                }
            }
            if (menu==NULL)
                menu = ui->menuExperiment->addMenu(e.getGroup());
        }

        // New experiment action
        QAction *act_e = act_exps->addAction(e.getDescription());
        // Set postion
        act_e->setData(i);
        // Action is checkable
        act_e->setCheckable(true);

        // Add to menu
        if (menu==NULL)
            ui->menuExperiment->addAction(act_e);
        else
            menu->addAction(act_e);
    }

    // Assign slot for experiment menu
    connect(ui->menuExperiment, SIGNAL( triggered( QAction * ) ), this, SLOT( experimentMenuActionTriggered( QAction * ) ) );

    // If there is only one experiment then remove experiment menu
    if (xmlData.getExps().count()<=1)
        ui->menuBar->removeAction(ui->menuExperiment->menuAction());

    // Documentation tab
    configureDocumentation();


    if (xmlData.getIapp().getReadStoredGeo())
    {
        // Read stored geometry configuration
        readGeoConf();
    }
    else
    {
        // No reading geometry
        setPosExperiment(0);
    }

    // Set current experiment
    bool found = false;
    for(int i=0;!found && i<act_exps->actions().count();i++)
    {
        found = act_exps->actions()[i]->isChecked();
        if (found) act_exps->actions()[i]->trigger();
    }
}

void MainWindow::setAppStyleSheet()
{
    app a       = xmlData.getIapp();
    cTitle      = a.getTitleColor().isEmpty() ? cTitle_def : a.getTitleColor();
    cBackground = a.getBackColor().isEmpty() ? cBackground_def : a.getBackColor();
    cHeader     = a.getSectionColor().isEmpty() ? cHeader_def : a.getSectionColor();
    cFontHeader = a.getFontSectionColor().isEmpty() ? cFontHeader_def : a.getFontSectionColor();
    cFontTitle  = a.getFontTitleColor().isEmpty() ? cFontTitle_def : a.getFontTitleColor();

    qApp->setStyleSheet(
        "#fTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
        "#lTitle {color: " + cFontTitle + ";}"
        "#lExperiment {color: " + cFontTitle + ";}"
        "#lLogo {border-radius: 10px;}"
        "#hParameters {background-color: " + cHeader + "; color: white; border-top-left-radius: 10px; border-top-right-radius: 10px;}"
        "#hGraphs {background-color: " + cHeader + "; color: white; border-top-left-radius: 10px; border-top-right-radius: 10px;}"
        "#hSimulation {background-color: " + cHeader + "; color: white; border-top-left-radius: 10px; border-top-right-radius: 10px;}"
        "#hDocumentation {background-color: " + cHeader + "; color: white; border-top-left-radius: 10px; border-top-right-radius: 10px;}"
        "#lParameters {color: "+ cFontHeader + ";}"
        "#lGraphs {color: "+ cFontHeader + ";}"
        "#lSimulation {color: "+ cFontHeader + ";}"
        "#lDocumentation {color: "+ cFontHeader + ";}"
        "QMainWindow {background-color: " + cBackground + ";}"
        "QSplitter::handle:vertical   {image: url(:icons/hsplitter.png);}"
        "QSplitter::handle:horizontal {image: url(:icons/vsplitter.png);}"
        "QHeaderView::section {background-color: white;}"
        "QTabWidget {background-color: white; }"
        "QTabBar::tear{width: 0px;}"
        "QTabBar QToolButton { border: 1px solid rgb(201, 203, 205); background-color: white;}"
        "QTextBrowser {background-color: white;}"
        "QTableWidget {border-style: none; background-color: white;}"
        "QHeaderView::up-arrow { width: 20px; height:12px; subcontrol-position: center right;}"
        "QHeaderView::down-arrow { width: 20px; height:12px; subcontrol-position: center right;}"
        "QHeaderView::section { background-color: " + cBackground + "; border-style: none; padding: 4px; border: 1px solid rgb(201, 203, 205);}"
        );

    ui->tGraphs->setTabShape(QTabWidget::Rounded);
    ui->tDocumentation->setTabShape(QTabWidget::Rounded);
    ui->tGraphs->setUsesScrollButtons(true);
}

void MainWindow::configureApp()
{
    app a = xmlData.getIapp();

    // Window title
    setWindowTitle(a.getWindowName());

    // Icon - If there is an error, reduce icon size.
    if (!a.getIcon().isEmpty())
    {
        setWindowIcon(QIcon(a.getIcon()));
        svgview.setWindowIcon(QIcon(a.getIcon()));
    }
    // Title
    ui->lTitle->setText(a.getTitleName() + sVer + xmlData.getIapp().getVersion());

    // Logo
    if (!a.getLogo().isEmpty())
    {
        QPixmap  logo(a.getLogo());
        QRect    rec    = ui->lLogo->rect();
        QString  cTitle = a.getTitleColor().isEmpty() ? cTitle_def : a.getTitleColor();
        QString  sStyle = "background-color: " + cTitle + "; border-radius: 10px;";

        logo = logo.scaled(QSize(rec.height(),rec.width()),  Qt::KeepAspectRatioByExpanding);
        ui->lLogo->setStyleSheet(sStyle);
        ui->lLogo->setPixmap(logo);
        ui->lLogo->setAlignment(Qt::AlignCenter);
    }

    // Configuration options visible status
    ui->actionNumerical_integrator->setVisible(a.getConf_integrator());
    ui->actionInputs->setVisible(a.getConf_inputs());
    ui->actionOutputs->setVisible(a.getConf_outputs());
    if (!ui->actionNumerical_integrator->isVisible() && !ui->actionInputs->isVisible() && !ui->actionOutputs->isVisible())
        ui->menuBar->removeAction(ui->menu_Configuration->menuAction());
    ui->bResults->setVisible(a.getResults());

    // Simulation bar
    if (a.getSteady())
    {
        ui->fSimulation->layout()->removeWidget(ui->lTsim);
        ui->fSimulation->layout()->removeWidget(ui->sSim);
        ui->lTsim->setVisible(false);
        ui->sSim->setVisible(false);
    }

    // Experiment text
    QString expText = a.getExperimentMenu().trimmed();
    if (!expText.isEmpty())
        ui->menuExperiment->setTitle(expText);

    // Resources
    ui->tRes->setItemDelegateForColumn(1,rl);
    ui->tRes->verticalHeader()->hide();
    ui->tRes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tRes->setMouseTracking(true);
    ui->tRes->viewport()->setMouseTracking(true);
    ui->tRes->installEventFilter(this);
    ui->tRes->viewport()->installEventFilter(this);
}

int findCol(QList<column> l, QString name)
{
    bool found = false;
    int  i     = 0;

    while(!found && i<l.count())
    {
        found = l[i].getName().toLower() == name.toLower();
        if (!found) i++;
    }
    return found ? i : -1;
}

void setSplitterSize(QSplitter *s, double per, int width)
{
    QList<int> sizes;

    if(per>=0)
    {
        sizes.clear();
        sizes.append(width*per);
        sizes.append(width-sizes[0]);
        s->setSizes(sizes);
    }
}

void MainWindow::configureGeometry()
{
    QRect      rect  = geometry();
    int        w     = xmlData.getGeo().getWidth();
    int        h     = xmlData.getGeo().getHeight();
    double     hs    = xmlData.getGeo().getPos_hslider();
    double     vs    = xmlData.getGeo().getPos_vslider();

    // Width, height, splitter positions
    if (w>0) rect.setWidth(w);
    if (h>0) rect.setHeight(h);
    setGeometry(rect);
    w = rect.width();
    h = rect.height();
    setSplitterSize(ui->splitter_1,hs,w);
    setSplitterSize(ui->splitter_2,vs,h);

    // Parameter columns
    QList<column> cols = xmlData.getCols();

    if (cols.count()>0)
    {
        for(int i=0;i<ui->tParameters->columnCount();i++)
        {
            int index = findCol(cols,ui->tParameters->headerItem()->text(i));

            if (index>=0)
            {
                int size = cols[index].getSize();
                if (size>0) ui->tParameters->setColumnWidth(i,size);
            }
            else
                ui->tParameters->setColumnHidden(i,true);
        }
    }

    // Clear minimum values
    ui->fGraphs->setMinimumWidth(0);
}

void MainWindow::configureConMenus()
{
    const QString iStyle     = ":icons/format-border-style.svg";

    ui->actionConfigureGraph->setText("Configure graph");
    ui->actionConfigureSerie->setText("Configure series");
    ui->actionZoomOut->setText("Zoom out");
    ui->actionZoomReset->setText("Zoom reset");

    ui->actionConfigureGraph->setIcon(QIcon(":icons/office-chart-area-black.svg"));
    ui->actionConfigureSerie->setIcon(QIcon(":icons/office-chart-area-black.svg"));
    ui->actionZoomOut->setIcon(QIcon(":icons/zoom-out.svg"));
    ui->actionZoomReset->setIcon(QIcon(":icons/zoom.svg"));

    ui->actionLight->setIcon(QIcon(iStyle));
    ui->actionBlue_cerulean->setIcon(QIcon(iStyle));
    ui->actionDark->setIcon(QIcon(iStyle));
    ui->actionBorwn_sand->setIcon(QIcon(iStyle));
    ui->actionBlue_NCS->setIcon(QIcon(iStyle));
    ui->actionHigh_contrast->setIcon(QIcon(iStyle));
    ui->actionBlue_icy->setIcon(QIcon(iStyle));
    ui->actionQt->setIcon(QIcon(iStyle));

    ui->actionLight->setText("Light");
    ui->actionBlue_cerulean->setText("Blue cerulean");
    ui->actionDark->setText("Dark");
    ui->actionBorwn_sand->setText("Brown sand");
    ui->actionBlue_NCS->setText("Blue NCS");
    ui->actionHigh_contrast->setText("High constrast");
    ui->actionBlue_icy->setText("Blue icy");
    ui->actionQt->setText("Qt");

    cmStyle.setIcon(QIcon(iStyle));
    cmStyle.setTitle("Configure style");
    cmStyle.addSection("Style overrides current configuration");
    cmStyle.addAction(ui->actionLight);
    cmStyle.addAction(ui->actionBlue_cerulean);
    cmStyle.addAction(ui->actionDark);
    cmStyle.addAction(ui->actionBorwn_sand);
    cmStyle.addAction(ui->actionBlue_NCS);
    cmStyle.addAction(ui->actionHigh_contrast);
    cmStyle.addAction(ui->actionBlue_icy);
    cmStyle.addAction(ui->actionQt);

    cmSerie.addSection("Line series");
    cmSerie.addAction(ui->actionConfigureSerie);
    cmSerie.addSeparator();
    cmSerie.addAction(ui->actionSerieClipboardText);
    cmSerie.addAction(ui->actionSerieFileText);

    cmClipboardGraph.setIcon(QIcon(iClipboard));
    cmClipboardGraph.setTitle("Clipboard");
    cmClipboardGraph.addAction(ui->actionChartClipboardImage);
    cmClipboardGraph.addAction(ui->actionChartClipboardText);

    cmSaveGraph.setIcon(QIcon(iSave));
    cmSaveGraph.setTitle("Save");
    cmSaveGraph.addAction(ui->actionChartFileImage);
    cmSaveGraph.addAction(ui->actionChartFileText);

    cmGraph.addSection("Graphs");
    cmGraph.addAction(ui->actionConfigureGraph);
    cmGraph.addMenu(&cmStyle);
    cmGraph.addSeparator();
    cmGraph.addAction(ui->actionZoomReset);
    cmGraph.addAction(ui->actionZoomOut);
    cmGraph.addSeparator();
    cmGraph.addMenu(&cmClipboardGraph);
    cmGraph.addMenu(&cmSaveGraph);

}

void MainWindow::setDefaultChart(QChartView *qcv)
{
    QBrush bWhite(Qt::white);

    qcv->setBackgroundBrush(bWhite);
    qcv->setFrameShape(QFrame::Shape::NoFrame);
    qcv->chart()->legend()->hide();
    qcv->setRenderHint(QPainter::Antialiasing);
    setEmptyAxes(qcv);
    qcv->installEventFilter(this);
    qcv->chart()->setAcceptHoverEvents(true);
    qcv->setMouseTracking(true);
    qcv->setContextMenuPolicy(Qt::CustomContextMenu);
    qcv->setRubberBand(QChartView::RectangleRubberBand);
    connect(qcv, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}

void MainWindow::onCustomContextMenu(const QPoint &p)
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QChartView  *qcv = ((QChartView *)w);

        if (selectedSerie())
            cmSerie.exec(qcv->mapToGlobal(p));
        else
            cmGraph.exec(qcv->mapToGlobal(p));
    }
}

void MainWindow::parameterMenuActionTriggered(QAction *action)
{
    int pos = action->data().toInt();

    ui->tParameters->setColumnHidden(pos,!ui->tParameters->isColumnHidden(pos));
}

void MainWindow::setState()
{
    ui->bSimulate->setEnabled(isNotExecuting());
    ui->bStop->setEnabled(isExecuting());
    ui->bResults->setEnabled(isNotExecuting() && isSimulated());
    ui->sSim->setEnabled(isNotExecuting() && isSimulated());

    ui->actionChartClipboardImage->setEnabled(isNotExecuting() && isSimulated());
    ui->actionChartClipboardText->setEnabled(isNotExecuting() && isSimulated());
    ui->actionChartFileImage->setEnabled(isNotExecuting() && isSimulated());
    ui->actionChartFileText->setEnabled(isNotExecuting() && isSimulated());

    ui->actionSerieFileText->setEnabled(isNotExecuting() && isSimulated());
    ui->actionSerieClipboardText->setEnabled(isNotExecuting() && isSimulated());
}

bool MainWindow::isNotExecuting()
{
    return !isExecuting();
}

bool MainWindow::isExecuting()
{
    return thrSim!=NULL;
}

bool MainWindow::isSimulated()
{
    return values_output.count()>0;
}

void MainWindow::experimentMenuActionTriggered(QAction *action)
{
    // ------------------- //
    // Set current project //
    // ------------------- //
    indexPrj = action->data().toInt();

    // ---------------- //
    // Get project data //
    // ---------------- //
    experiment  exp = xmlData.getExps()[indexPrj];
    opt_project prj = xmlData.getPrjs()[indexPrj];

    // ----------------- //
    // Title description //
    // ----------------- //
    QString txtExp = xmlData.getIapp().getExperimentTitle().isEmpty() ? sExp : xmlData.getIapp().getExperimentTitle();
    ui->lExperiment->setText(txtExp + exp.getDescription());

    // ------ //
    // Graphs //
    // ------ //

    // Delete previous tabs
    while (ui->tGraphs->count()>0)
        ui->tGraphs->removeTab(0);

    // Free dynamic memory: axis, series, charts and tabs.
    qDeleteAll(lsvg);
    qDeleteAll(lwidget);
    //qDeleteAll(lspliter); -- Deleted in ltabs
    qDeleteAll(ltabs);
    qDeleteAll(axis);
    qDeleteAll(qlss);
    qDeleteAll(qlas);
    qDeleteAll(qlps);
    qDeleteAll(qcvs);
    qDeleteAll(tabs);
    axis.clear();
    qlas.clear();
    qlps.clear();
    qlss.clear();
    qcvs.clear();
    tabs.clear();
    ltabs.clear();
    ldoc.clear();
    lsvg.clear();
    lwidget.clear();
    lspliter.clear();
    qlss_points.clear();
    qll_points.clear();
    qlu_points.clear();
    qlss_ignored.clear();
    qlss_ignored_val.clear();
    lar.clear();

    // ----------------------- //
    // And new diagram - table //
    // ----------------------- //
    addDiagramTables(exp);

    // -------------- //
    // Add new graphs //
    // -------------- //
    addGraphs(exp);

    // ---------- //
    // Parameters //
    // ---------- //
    populateTree(prj);

    // ---------- //
    // Simulation //
    // ---------- //

    // Configure simulation: integration, experiment & outputs
    restoreSim(prj);

    // ------- //
    // Diagram //
    // ------- //
    configureDiagram();

    // --------- //
    // Resources //
    // --------- //
    configureResource();

    // Windows and buttons state
    setState();
}

void MainWindow::showEvent(QShowEvent *)
{
    // NOTE: in order to adjust sizes of diagrams in tabs
    // it seem that it has no effect if called in the constructor.
    if (!ajustedDia)
    {
        experiment  exp = xmlData.getExps()[indexPrj];
        QList<diagramTable> dt = exp.getDiaTabs();
        int j = 0;

        // Adjust splitters
        for(int i=0;i<dt.count() && j<lspliter.count();i++)
        {
            if (!dt[i].getPicture().trimmed().isEmpty() &&
                 dt[i].getTLinks().count()>0)
            {
                setSplitterSize(lspliter[j],dt[i].getPos_hslider(),
                                dt[i].getHorizontal() ? tabs.last()->geometry().width() :
                                                        tabs.last()->geometry().height());
                j++;
            }
        }

        // Adjust SVG
        splitterMoved();

        ajustedDia = true;
    }
}

void MainWindow::addDiagramTables(experiment exp)
{
    QList<diagramTable> dt = exp.getDiaTabs();

    for(int i=0;i<dt.count();i++)
    {
        QDomDocument doc;
        QWidget      *vLay  = NULL;
        QWidget      *tLay  = NULL;
        QSvgWidget   *view  = NULL;
        QTableWidget *table = NULL;

        tabs.append(new QWidget(ui->tGraphs));
        tabs.last()->setLayout(new QHBoxLayout());

        // Diagram
        if (!dt[i].getPicture().trimmed().isEmpty())
        {
            // Create layout
            vLay = new QWidget;

            // Create view
            view = new QSvgWidget(vLay);

            // Configure
            vLay->setContextMenuPolicy(Qt::CustomContextMenu);
            vLay->connect(vLay,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequestedDiagram(QPoint)));

            // Actions context menu
            cmSVG.addAction(ui->actionClipboard);
            cmSVG.addAction(ui->actionSave);           

            // XML to QDomDocument
            if (XML_to_QDom(dt[i].getPicture().trimmed(),doc))
            {
                int width,height;

                // Read width and height for aspect ratio
                if (SVG_size(doc,width,height))
                    lar.append(((double)width)/height);
                else
                    lar.append(1);

                // Add to list
                ldoc.append(doc);
                lsvg.append(view);
                lwidget.append(vLay);

                // Link svg with doc
                view->setProperty(DOC_REF.toStdString().c_str(),ldoc.count()-1);
                // Link svg with diagram-table
                view->setProperty(DOC_DT.toStdString().c_str(),i);
            }
            else
            {
                delete view;
                delete vLay;
            }
        }

        // Table
        if (!dt[i].getTLinks().isEmpty())
        {
            // Widget for layout
            tLay = new QWidget;

            // Create table
            table = new QTableWidget;

            // Link table with diagram-table
            table->setProperty(DOC_DT.toStdString().c_str(),i);

            // Add to delete list
            ltabs.append(table);

            // Actions context menu
            cmTable.addAction(ui->actionClipboardTable);
            cmTable.addAction(ui->actionClipboardTableHeader);

            // Configure table
            QStringList headerLabels;

            headerLabels.push_back("Name");
            headerLabels.push_back("Description");
            headerLabels.push_back("Value");
            headerLabels.push_back("Units");

            table->setColumnCount(headerLabels.count());
            table->setHorizontalHeaderLabels(headerLabels);
            table->horizontalHeader()->setStretchLastSection(true);
            table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
            table->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
            table->verticalHeader()->hide();

            table->setContextMenuPolicy(Qt::CustomContextMenu);
            table->connect(table,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequestedTable(QPoint)));

            int width_name  = dt[i].getWidth_name();
            int width_desc  = dt[i].getWidth_desc();
            int width_value = dt[i].getWidth_value();
            int width_units = dt[i].getWidth_units();

            if (width_name >0) table->setColumnWidth(0,width_name);
            if (width_desc >0) table->setColumnWidth(1,width_desc);
            if (width_value>0) table->setColumnWidth(2,width_value);
            if (width_units>0) table->setColumnWidth(3,width_units);

            // Fill table
            QList<link> tLinks = dt[i].getTLinks();

            for(int j=0;j<tLinks.count();j++)
            {
                table->insertRow(j);
                table->setItem(j,0,new QTableWidgetItem(tLinks[j].getVariable()));
                table->setItem(j,1,new QTableWidgetItem(tLinks[j].getDesc()));
                table->setItem(j,2,new QTableWidgetItem(sEmpty));
                table->setItem(j,3,new QTableWidgetItem(tLinks[j].getUnits()));
                table->item(j,0)->setFlags(table->item(j,0)->flags() ^ Qt::ItemIsEditable);
                table->item(j,1)->setFlags(table->item(j,1)->flags() ^ Qt::ItemIsEditable);
                table->item(j,2)->setFlags(table->item(j,2)->flags() ^ Qt::ItemIsEditable);
                table->item(j,3)->setFlags(table->item(j,3)->flags() ^ Qt::ItemIsEditable);
                table->item(j,0)->setTextColor(Qt::darkRed);
                table->item(j,1)->setTextColor(Qt::darkBlue);
                table->item(j,2)->setTextColor(Qt::black);
                table->item(j,3)->setTextColor(Qt::darkGreen);
            }

            // Enable sorting
            table->setSortingEnabled(true);

            // Select first row
            if (table->columnCount()>0 && table->rowCount()>0)
                table->setCurrentCell(0,0);

            // Insert in layout
            //table->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
            tLay->setLayout(new QVBoxLayout);
            tLay->layout()->addWidget(table);
            //tLay->layout()->setContentsMargins(0,0,0,0);
            //tLay->layout()->setAlignment(table,Qt::AlignVCenter);

        }

        // Splliter if there are both
        if (vLay!=NULL && tLay!=NULL)
        {
            QSplitter *splitter = new QSplitter(tabs.last());

            splitter->setOrientation(dt[i].getHorizontal() ? Qt::Horizontal : Qt::Vertical);
            splitter->setHandleWidth(HANDLE_WIDTH);
            splitter->setStyleSheet("QTableWidget { margin-left: 10px }");
            lspliter.append(splitter);
            splitter->addWidget(vLay);
            splitter->addWidget(tLay);
            tabs.last()->layout()->addWidget(splitter);
            splitter->connect(splitter,SIGNAL(splitterMoved(int,int)),this,SLOT(splitterMoved()));
        }
        else if (vLay!=NULL)
            tabs.last()->layout()->addWidget(vLay);
        else if (tLay!=NULL)
            tabs.last()->layout()->addWidget(tLay);

        // Tab
        ui->tGraphs->setCurrentIndex(ui->tGraphs->count()-1);
        ui->tGraphs->insertTab(ui->tGraphs->currentIndex()+1,tabs.last(),QIcon(":icons/office-chart-area-black.svg"),dt[i].getName());
        ui->tGraphs->show();

        // Load SVG and evaluate links
        evaluateLinks(-1);

        // Splitter size -- It has to be also done after show for the first time!!
        // This code is for when selecting a different experiment
        if (vLay!=NULL && tLay!=NULL)
        {
            setSplitterSize(lspliter.last(),dt[i].getPos_hslider(),
                            dt[i].getHorizontal() ? tabs.last()->geometry().width() :
                                                    tabs.last()->geometry().height());
            // Adjust SVG
            splitterMoved();
        }

    }
}

void MainWindow::customContextMenuRequestedTable(QPoint pos)
{
    // Temporarl reference to table
    tableTemp = qobject_cast<QTableWidget *>(sender());

    if (tableTemp!=NULL)
        cmTable.exec(tableTemp->viewport()->mapToGlobal(pos));
}

void MainWindow::customContextMenuRequestedDiagram(QPoint pos)
{
    QWidget *vLay = qobject_cast<QWidget *>(sender());

    if (vLay!=NULL)
    {
        if (vLay->children().count()>0)
        {
            if (vLay->children()[0]->inherits("QSvgWidget"))
            {
                // Temporal reference to svg
                svgTemp = qobject_cast<QSvgWidget *>(vLay->children()[0]);
                // Call menu
                cmSVG.exec(vLay->mapToGlobal(pos));
            }
        }
    }
}

void MainWindow::splitterMoved()
{
    for(int i=0;i<lsvg.count();i++)
    {
        SVG_setSize(lsvg[i],lar[i],lwidget[i]->width()-SVG_RIGHT_MARGIN,lwidget[i]->height());
        //qDebug() << lwidget[i]->width() << lwidget[i]->height();
    }
}

void MainWindow::addGraphs(experiment exp)
{
    QList<graph> graphs = exp.getGraphs();

    for(int i=0;i<graphs.count();i++)
    {
        tabs.append(new QWidget(ui->tGraphs));
        tabs.last()->setLayout(new QHBoxLayout());
        qcvs.append(new QChartViewExt);
        setDefaultChart(qcvs.last());
        tabs.last()->layout()->addWidget(qcvs.last());
        ui->tGraphs->setCurrentIndex(ui->tGraphs->count()-1);
        ui->tGraphs->insertTab(ui->tGraphs->currentIndex()+1,tabs.last(),QIcon(":icons/office-chart-area-black.svg"),graphs[i].getName());
    }
    ui->tGraphs->setCurrentIndex(0);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    // Diagram
    if (obj->objectName() == ui->meDiagram->objectName())
    {
        if (event->type() == QEvent::Resize)
            setDiagramSize();
        else if (event->type() == QEvent::MouseButtonPress)
        {
            svgview.show();
            svgview.raise();
            svgview.activateWindow();
            svgview.loadFile(ui->meDiagram->property(PRO_DIAGRAM.toStdString().c_str()).toString());
            evaluateLinks(ui->sSim->value());
        }
    }
    // Resources - Pointing hand cursor
    if (obj->objectName() == ui->tRes->viewport()->objectName())
    {
        if (event->type() == QEvent::MouseMove)
        {
            QPoint pos = ((QMouseEvent *)event)->pos();
            QModelIndex index = ui->tRes->indexAt(pos);

            if (index.column() == 1)
            {
                if (QApplication::overrideCursor()==NULL ||
                    QApplication::overrideCursor()->shape()!=Qt::PointingHandCursor)
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
            }
            else
                QApplication::restoreOverrideCursor();
        }
        if (event->type() == QEvent::Leave)
        {
            QApplication::restoreOverrideCursor();
        }
    }
    // To process other events
    return false;
}

void MainWindow::evaluateLinks(int position)
{
    experiment exp = xmlData.getExps()[indexPrj];

    // Evaluate links external diagram
    QDomDocument docd = svgview.getDoc();

    for(int i=0;i<exp.getLinks().count();i++)
    {
        link l = exp.getLinks()[i];

        if (l.getType() == LINK_ASSIGN)
            evaluateLinkAssign(docd,l,position);
        else if (l.getType() == LINK_TEXT)
            evaluateLinkText(docd,l,position);
    }
    svgview.setDoc(docd);
    svgview.refresh();

    // For each diagram
    for(int i=0;i<lsvg.count();i++)
    {
        // Locate diagram-table
        int pos_dt = lsvg[i]->property(DOC_DT.toStdString().c_str()).toInt();

        // Locate xml document
        int pos_doc = lsvg[i]->property(DOC_REF.toStdString().c_str()).toInt();

        // If there is diagram table and xml document
        if (pos_dt >=0 && pos_dt <exp.getDiaTabs().count() &&
            pos_doc>=0 && pos_doc<ldoc.count())
        {
            diagramTable dt   = exp.getDiaTabs()[i];
            QDomDocument doc  = ldoc[pos_doc];
            QList<link> links = dt.getDLinks();

            // For each link
            for(int j=0;j<links.count();j++)
            {
                link l = links[j];

                if (l.getType() == LINK_ASSIGN)
                    evaluateLinkAssign(doc,l,position);
                else if (l.getType() == LINK_TEXT)
                    evaluateLinkText(doc,l,position);
            }

            // Referesh svg
            SVG_refresh(doc,lsvg[i],lwidget[i]->width()-SVG_RIGHT_MARGIN,lwidget[i]->height(),lar[i]);
        }
    }

    // For each table
    for(int i=0;i<ltabs.count();i++)
    {
        // Locate diagram-table
        int pos_dt = ltabs[i]->property(DOC_DT.toStdString().c_str()).toInt();


        // If there is diagram table and xml document
        if (pos_dt >=0 && pos_dt <exp.getDiaTabs().count())
        {
            diagramTable dt   = exp.getDiaTabs()[i];
            QList<link> links = dt.getTLinks();
            QStringList names;

            // Variable list
            for(int j=0;j<ltabs[i]->rowCount();j++)
                names.append(ltabs[i]->item(j,0)->text());

            // For each link
            for(int j=0;j<links.count();j++)
            {
                // Find variable name in table
                int index = names.indexOf(links[j].getVariable());

                if (index>=0 && index<ltabs[i]->rowCount())
                {
                    // Get value
                    QString value = position>=0 ? QString::number(getVariableValue(links[j].getVariable(),position),'G',4) : sEmpty;

                    // Set value in table
                    ltabs[i]->item(index,2)->setText(value);
                }
            }
        }
    }
}

double MainWindow::getVariableValue(QString var, int position)
{
    opt_model mo = xmlData.getPrjs()[indexPrj].getModel();

    int var_pos = findVar(var,mo.outputs);

    // If variable found
    if (var_pos>=0)
    {
        return values_output.count() > var_pos && values_output[var_pos].count()>position
               ? values_output[var_pos][position] : 0;
    }
    // Otherwise search in parameteers
    else
    {
        // Search in new values
        int index = mo.p_name_new.indexOf(var);

        if (index>=0) return mo.p_value_new[index].toDouble();

        // Search in model parameters
        var_pos = findVar(var,mo.params);

        return var_pos>=0 ? mo.params[var_pos].value.toDouble() : 0;
    }
}

void MainWindow::evaluateLinkAssign(QDomDocument doc, link l, int position)
{
    if (position>=0)
    {
        double var_val = getVariableValue(l.getVariable(),position);

        assignSvgPro(doc,l.getId(),l.getProperty(),var_val);
    }
}

void MainWindow::evaluateLinkText(QDomDocument doc, link l, int position)
{
    if(position>=0)
    {
        double var_val = getVariableValue(l.getVariable(),position);

        assignSvgText(doc,l.getId(),QString::number(var_val,'G',4) + " " + l.getUnits());
    }
    else
    {
        assignSvgText(doc,l.getId(),sEmpty);
    }
}

void MainWindow::assignSvgPro(QDomDocument doc, QString id, QString property, double val)
{
    changeSvgAttr(doc,LINK_ASSIGN,id,property,QString::number(val));
}

void MainWindow::assignSvgText(QDomDocument &doc, QString id, QString value)
{
    changeSvgAttr(doc,LINK_TEXT,id,sEmpty,value);
}

void MainWindow::configureDiagram()
{
    experiment exp = xmlData.getExps()[indexPrj];


    if (!exp.getPicture().isEmpty())
    {
        ui->meDiagram->clear();
        ui->meDiagram->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->meDiagram->installEventFilter(this);
        ui->meDiagram->setCursor(Qt::PointingHandCursor);
        setSplitterSize(ui->splitter_3,exp.getPos_diagram(),ui->splitter_3->geometry().width());
        setDiagramSize();
        evaluateLinks(0);
    }
    else
    {
        ui->tME->layout()->removeWidget(ui->splitter_3);
        ui->tME->layout()->addWidget(ui->meDescription);
    }
    ui->meDescription->setText(exp.getPicture_desc());
}

void MainWindow::setDiagramSize()
{
    experiment exp   = xmlData.getExps()[indexPrj];
    QString    thumb = exp.getThumb().isEmpty() ? exp.getPicture() : exp.getThumb();

    if (!thumb.isEmpty())
    {
        ui->meDiagram->setProperty(PRO_DIAGRAM.toStdString().c_str(),exp.getPicture());
        QPixmap pixmap = QIcon(thumb).pixmap(ui->meDiagram->size()*0.95);
        ui->meDiagram->setPixmap(pixmap);
    }
}

void MainWindow::configureDocumentation()
{
    // Icons
    ui->tDocumentation->setTabIcon(0,QIcon(":/icons/applications-science.svg"));
    ui->tDocumentation->setTabIcon(1,QIcon(":/icons/view-categories-expenditures.svg"));
    ui->tDocumentation->setTabIcon(2,QIcon(":/icons/user-group-new.svg"));

    // Select first tab
    ui->tDocumentation->setCurrentIndex(0);

    // Configure authors
    configureAuthors();
}

void MainWindow::configureResource()
{
    experiment      e   = xmlData.getExps()[indexPrj];
    double          hsd = e.getPos_documents();
    double          wn  = e.getWidth_name();
    double          wd  = e.getWidth_document();
    QList<resource> res = e.getResources();
    QPalette p;

    // Resources geometry
    if (wn>0) ui->tRes->setColumnWidth(0,wn);
    if (wd>0) ui->tRes->setColumnWidth(1,wd);
    setSplitterSize(ui->splitter,hsd,ui->splitter->geometry().width());

    while (ui->tRes->rowCount()>0) ui->tRes->removeRow(0);
    if (res.count()>0)
    {
        // Insert tab if removed
        if (pRes!=NULL)
        {
            ui->tDocumentation->insertTab(1,pRes,"Resources");
            pRes = NULL;
        }

        // Disable sorting when writting elevemnts
        ui->tRes->setSortingEnabled(false);

        // Write elements
        for(int i=0;i<res.count();i++)
        {
            ui->tRes->insertRow(i);
            ui->tRes->setItem(i,0,new QTableWidgetItem(res[i].getName()));
            ui->tRes->setItem(i,1,new QTableWidgetItem(res[i].getDocument()));
            ui->tRes->item(i,0)->setFlags(ui->tRes->item(i,0)->flags() ^ Qt::ItemIsEditable);
            ui->tRes->item(i,1)->setFlags(ui->tRes->item(i,1)->flags() ^ Qt::ItemIsEditable);
            ui->tRes->item(i,1)->setTextColor(p.link().color());
            ui->tRes->item(i,1)->setToolTip(res[i].getLink());
            ui->tRes->item(i,0)->setData(Qt::UserRole,i);
        }

        // Enable sorting
        ui->tRes->setSortingEnabled(true);

        // Select first row
        if (ui->tRes->columnCount()>0 && ui->tRes->rowCount()>0)
        {
            ui->tRes->setCurrentCell(0,0);
            ui->rDescription->setText(res[ui->tRes->currentItem()->data(Qt::UserRole).toInt()].getDescription());
        }
    }
    else
    {
        // Remove tab if there are no resources
        if (pRes==NULL)
        {
            pRes = ui->tResources;
            ui->tDocumentation->removeTab(1);
        }
    }
}

void MainWindow::configureAuthors()
{
    const int MIN_WIDTH    = 24;
    const int MIN_HEIGHT   = 24;
    const int FRAME_HEIGHT = 110;

    // Author list
    QList<author> lau = xmlData.getAuthors();

    // Scroll area grid layout
    gau = new QVBoxLayout;

    for(int i=0;i<lau.count();i++)
    {
        QPixmap picon;
        QFont   f;

        // New frame
        lfau.append(new QFrame);

        // New frame widgets
        llau.append(new QGridLayout);
        licon.append(new QLabel("Icon"));
        lemail.append(new QLabel);
        lname.append(new QLabel);
        lcompany.append(new QLabel);
        lweb.append(new QLabel);
        lrole.append(new QLabel);

        // Icon
        picon.load(":/icons/im-user.svg");
        licon.last()->setPixmap(picon);
        licon.last()->setMinimumHeight(MIN_WIDTH);
        licon.last()->setMaximumWidth(MIN_HEIGHT);

        // Name
        lname.last()->setText(lau[i].getName());
        lname.last()->setMinimumHeight(MIN_HEIGHT);
        f = lname.last()->font();
        f.setBold(true);
        f.setPointSize(f.pointSize()+1);
        lname.last()->setFont(f);

        // Company
        lcompany.last()->setText(lau[i].getCompany());
        lcompany.last()->setMinimumHeight(MIN_HEIGHT);

        // Web
        lweb.last()->setText("<a href=\""+lau[i].getWebsite()+"\">"+lau[i].getWebsite()+"</a>");
        lweb.last()->setOpenExternalLinks(true);
        lweb.last()->setMinimumHeight(MIN_HEIGHT);

        // Email
        lemail.last()->setText("<a href='mailto:"+lau[i].getEmail()+"'>"+lau[i].getEmail()+"</a>");
        lemail.last()->setOpenExternalLinks(true);
        lemail.last()->setMinimumHeight(MIN_HEIGHT);

        // Role
        lrole.last()->setText(lau[i].getRole());
        lrole.last()->setMinimumHeight(MIN_WIDTH);
        f = lrole.last()->font();
        f.setPointSize(f.pointSize()+1);
        lrole.last()->setFont(f);

        // Frame
        lfau.last()->setMinimumHeight(FRAME_HEIGHT);
        lfau.last()->setMaximumHeight(FRAME_HEIGHT);

        // Add widgets to frame
        llau.last()->addWidget(licon.last(),0,0);
        llau.last()->addWidget(lname.last(),0,1);
        llau.last()->addWidget(lcompany.last(),1,0,1,2);
        llau.last()->addWidget(lweb.last(),2,0,1,2);
        llau.last()->addWidget(lemail.last(),3,0,1,2);
        llau.last()->addWidget(lrole.last(),4,0,1,2);
        lfau.last()->setLayout(llau.last());

        // Add frame to scroll area
        gau->addWidget(lfau.last());
    }

    // Set layout
    gau->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    fmain.setStyleSheet("background-color: white;");
    fmain.setLayout(gau);
    ui->scAuthors->setWidget(&fmain);
}

void MainWindow::restoreSim(opt_project prj)
{
    ui->lTsim->setText(QString::number(prj.getExp().startTime) + simSep + QString::number(prj.getExp().stopTime) + simUnits);
    ui->sSim->setMinimum(0);
    ui->sSim->setMaximum(100);
    ui->sSim->setValue(0);

    values_output.clear();
    time_output.clear();
}

void MainWindow::populateTree(opt_project prj)
{
    // Clear previous parameters
    changed.clear();
    new_value.clear();

    // Clear tree
    ui->tParameters->clear();

    // Delete previous widgets
    qDeleteAll(lpar_widget);
    lpar_widget.clear();

    // No changes yet in params
    for(int i=0;i<prj.getModel().params.count();i++)
    {
        changed.append(false);
        new_value.append(sEmpty);
    }
    ui->tParameters->setSortingEnabled(false);
    populateTreeGeneric(ui->tParameters,prj.getModel().params,prj,PAR_COL_NAME,sEmpty,setItemTreeParams,lpar_widget);
    ui->tParameters->setSortingEnabled(true);
    ui->tParameters->sortByColumn(-1);
    ui->tParameters->setContextMenuPolicy(Qt::CustomContextMenu);

    // Order parameters by name
    if (ui->tParameters->columnCount()>0)
        ui->tParameters->sortByColumn(0,Qt::AscendingOrder);
}

void MainWindow::on_le_textChanged(const QString &arg1)
{
    QLineEdit *qle = (QLineEdit *)sender();
    int pos;
    QString value;

    pos   = qle->property(PAR_PRO_POS.toLatin1().data()).toInt();
    value = qle->property(PAR_PRO_VALUE.toLatin1().data()).toString();

    if (arg1 == value)
    {
        qle->setStyleSheet("color: black; font: normal");
        changed[pos]   = false;
        new_value[pos] = arg1;
    }
    else
    {
        qle->setStyleSheet("color: green; font: bold");
        changed[pos]   = true;
        new_value[pos] = arg1;
    }
}

void MainWindow::on_sb_valueChanged(int arg1)
{
    QSpinBox *qsb = (QSpinBox *)sender();
    int pos;
    int value;

    pos   = qsb->property(PAR_PRO_POS.toLatin1().data()).toInt();
    value = qsb->property(PAR_PRO_VALUE.toLatin1().data()).toInt();

    if (arg1 == value)
    {
        qsb->setStyleSheet("color: black; font: normal");
        changed[pos]   = false;
        new_value[pos] = QString::number(arg1,NUM_FORMAT,NUM_DEC);
    }
    else
    {
        qsb->setStyleSheet("color: green; font: bold");
        changed[pos]   = true;
        new_value[pos] = QString::number(arg1,NUM_FORMAT,NUM_DEC);
    }
}

void MainWindow::on_cb_textChanged(const QString &arg1)
{
    QComboBox *qcb = (QComboBox *)sender();
    int pos;
    QString value;

    pos   = qcb->property(PAR_PRO_POS.toLatin1().data()).toInt();
    value = qcb->property(PAR_PRO_VALUE.toLatin1().data()).toString();

    if (arg1 == value)
    {
        qcb->setStyleSheet("color: black; font: normal");
        changed[pos]   = false;
        new_value[pos] = arg1;
    }
    else
    {
        qcb->setStyleSheet("color: green; font: bold");
        changed[pos]   = true;
        new_value[pos] = arg1;
    }
}

MainWindow::~MainWindow()
{
    // Delete dynamic objects
    delete act_exps;
    delete gau;
    delete rl;

    // Delete lists
    qDeleteAll(axis);
    qDeleteAll(qcvs);
    qDeleteAll(lsvg);
    qDeleteAll(lwidget);
    //qDeleteAll(lspliter); -- Deleted in ltabs
    qDeleteAll(ltabs);
    qDeleteAll(tabs);
    qDeleteAll(qlss);
    qDeleteAll(qlas);
    qDeleteAll(qlps);
    qDeleteAll(licon);
    qDeleteAll(lemail);
    qDeleteAll(lname);
    qDeleteAll(lcompany);
    qDeleteAll(lweb);
    qDeleteAll(lrole);
    qDeleteAll(llau);
    qDeleteAll(lfau);
    qDeleteAll(lpar_widget);

    // Delete plotting tool
    delete plot;

    // Delete GUI
    delete ui;
}

bool MainWindow::getInitialized() const
{
    return initialized;
}

void MainWindow::on_tbClear_clicked()
{
    ui->lineFilter->clear();
    on_lineFilter_returnPressed();
}

void MainWindow::on_lineFilter_returnPressed()
{
    filterTree(ui->tParameters,0,ui->lineFilter->text().trimmed(),ui->cbCaseSentitive->checkState() == Qt::Checked);
}

void MainWindow::on_tParameters_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;

    menu.addSection("Columns");
    for(int i=0;i<ui->tParameters->columnCount();i++)
    {
        QAction act;

        menu.addAction(ui->tParameters->headerItem()->text(i));
        menu.actions().last()->setCheckable(true);
        menu.actions().last()->setChecked(!ui->tParameters->isColumnHidden(i));
        menu.actions().last()->setData(i);
    }
    connect(&menu, SIGNAL( triggered( QAction * ) ), this, SLOT( parameterMenuActionTriggered( QAction * ) ) );
    menu.exec(ui->tParameters->mapToGlobal(pos));
}

void MainWindow::simThread()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);

    // Current project
    opt_project prj = xmlData.getPrjs()[indexPrj];

    // Clear simulation results and GUI
    restoreSim(prj);

    // Simulation thread
    thrSim = new QThread;
    sim    = new simulation(&prj);

    setState();
    sim->moveToThread(thrSim);
    connect(thrSim, SIGNAL(started()),   sim,    SLOT(run()));
    connect(sim,    SIGNAL(end()),       thrSim, SLOT(quit()));
    connect(sim,    SIGNAL(end()),       sim,    SLOT(deleteLater()));
    connect(thrSim, SIGNAL(finished()),  thrSim, SLOT(deleteLater()));

    connect(sim, SIGNAL(simUpdate(double,double,double)),                  this, SLOT(simUpdate(double,double,double)));
    connect(sim, SIGNAL(simError(unsigned, QString)),                      this, SLOT(simError(unsigned, QString)));
    connect(sim, SIGNAL(finished(double, opt_exp, opt_problem, unsigned)), this, SLOT(simFinished(double, opt_exp, opt_problem, unsigned)));

    thrSim->start();
}

void MainWindow::simUpdate(double t, double startTime, double stopTime)
{
    // GUI info
    ui->lTsim->setText(QString::number(t) + simSep + QString::number(stopTime)+ simUnits);
    ui->sSim->setValue(100 * ((t-startTime)/(stopTime-startTime)));
}

void MainWindow::sendToGUI(QString msg, QString styleSheet)
{
    ui->statusBar->showMessage(msg);
    ui->statusBar->setStyleSheet(styleSheet);
}

void MainWindow::simError(unsigned simStatus, QString msg)
{
    Q_UNUSED(simStatus);

    // Error message
    sendToGUI(mSimError + msg, ssColorRed);

    // Result cursor
    QApplication::restoreOverrideCursor();

    // GUI state
    setState();
}

void MainWindow::simFinished(double t, opt_exp exp, opt_problem prb, unsigned status)
{
    Q_UNUSED(prb);
    Q_UNUSED(status);

    // Current project
    opt_project prj = xmlData.getPrjs()[indexPrj];

    // Write project results
    prj.setExp(exp);

    // Get current results
    values_output = exp.values_output;
    time_output   = exp.time_output;

    // Simulation status
    unsigned simStatus = prj.getExp().simStatus;
    if (simStatus == SIM_FMU_VALID)     sendToGUI(mSimFinished + msToTime(t) + space + simSeconds, ssColorGreen);
    else if (simStatus == SIM_FMU_STOP) sendToGUI(mSimStopped, ssColorOrange);

    // Wait thread to finish
    while (thrSim->isRunning())
        QApplication::processEvents();
    thrSim = NULL;

    // Adjust simulation bar
    adjustsSim();

    // GUI state
    setState();

    // Update plots
    updatePlots();

    // Update diagram links
    evaluateLinks(ui->sSim->value());

    // New simulation results
    newResults = true;

    // Restore cursor
    QApplication::restoreOverrideCursor();
}

void MainWindow::adjustsSim()
{
    ui->sSim->setMinimum(0);
    ui->sSim->setMaximum(time_output.count()-1);
    ui->sSim->setValue(time_output.count()-1);
}

bool MainWindow::checkInputs()
{
    bool ok = true;
    int  i  = 0;

    while(ok && i<lpar_widget.count())
    {
        QLineEdit *le = dynamic_cast<QLineEdit *>(lpar_widget[i]);
        //QSpinBox  *sb = dynamic_cast<QSpinBox  *>(lpar_widget[i]);
        //QComboBox *cb = dynamic_cast<QComboBox *>(lpar_widget[i]);

        if (le!=NULL && le->validator()!=NULL)
        {
            QString  cad   = le->text();
            int      pos   = 0;
            variable param = xmlData.getPrjs()[indexPrj].getModel().params[le->property(PAR_PRO_POS.toLatin1().data()).toInt()];


            ok = le->validator()->validate(cad,pos) == QValidator::Acceptable;

            if (!ok)
            {
                QMessageBox msgBox;
                QString     text;
                const QDoubleValidator *dv = dynamic_cast<const QDoubleValidator *>(le->validator());

                if(cad.trimmed().isEmpty())
                    text = "Parameter <b>" + param.name + "</b> has no value.";
                else if(dv!=NULL)
                {
                    text = "Parameter " + param.name + " = <b>" + le->text() + "</b> is outside of ";

                    if (dv->bottom() > std::numeric_limits<double>::lowest())
                        text += "[ " + QString::number(dv->bottom()) + " , ";
                    else
                        text += "( -Inf , ";

                    if (dv->top() < std::numeric_limits<double>::max())
                        text += QString::number(dv->top()) + " ]";
                    else
                        text += "Inf )";

                    text += " range.";
                }
                else
                    text = "Wrong value for parameter " + param.name;
                msgBox.setText(text);
                msgBox.exec();
            }

        }
        //else if (sb!=NULL)
        //{
        //}
        //else if (cb!=NULL)
        //{
        //}
        i++;
    }
    return ok;
}

void MainWindow::on_bSimulate_clicked()
{
    // Check inputs are correct
    if (checkInputs())
    {

        // Apply parameter values to project model
        opt_project prj = xmlData.getPrjs()[indexPrj];
        opt_model   mo  = prj.getModel();
        mo.p_name_new.clear();
        mo.p_value_new.clear();
        for(int i=0;i<changed.count();i++)
        {
            if (changed[i])
            {
                mo.p_name_new.append(mo.params[i].name);
                mo.p_value_new.append(new_value[i]);
            }
        }

        // Set model & project
        prj.setModel(mo);
        xmlData.setPrj(prj,indexPrj);

        // Simulation thread
        simThread();
    }
}

void MainWindow::updatePlots(int count)
{
    QList<variable> mvars = xmlData.getPrjs()[indexPrj].getModel().outputs;
    QList<graph>    graphs = xmlData.getExps()[indexPrj].getGraphs();

    // Delete previous series, axes and data
    qDeleteAll(axis);
    qDeleteAll(qlss);
    qDeleteAll(qlas);
    qDeleteAll(qlps);
    qlss.clear();
    qlas.clear();
    qlps.clear();
    axis.clear();
    qlss_points.clear();
    qll_points.clear();
    qlu_points.clear();

    // Set previous position
    antPos = ui->sSim->value();

    // For each graph in the present experiment
    for(int i=0;i<graphs.count();i++)
    {
        QList<var> vars = graphs[i].getVars();

        // Remove previous series in chart
        qcvs[i]->chart()->removeAllSeries();

        // Add x axis label from graph
        for(int j=0;j<qcvs[i]->chart()->axes(Qt::Horizontal).count();j++)
            qcvs[i]->chart()->axes(Qt::Horizontal)[j]->setTitleText(graphs[i].getXaxis());

        // For each var in each graph
        for(int j=0;j<vars.count();j++)
        {
            // Get variable index
            int indexy = findVar(vars[j].getName(),mvars);
            int indexx = !vars[j].getX().isEmpty() ? findVar(vars[j].getX(),mvars) : -1;


            if (indexy>=0)
            {
                // Series name
                QString name = vars[j].getDescription();

                // Independent variable
                QList<double> values_x = indexx>=0 ? values_output[indexx] : time_output;

                // Plot the graph in the time interval
                if (indexy<values_output.count())
                {
                    // Vertical alignment flag
                    Qt::AlignmentFlag af = vars[j].getYalign().toLower() == algRight.toLower() ? Qt::AlignRight : Qt::AlignLeft;

                    if (graphs[i].getType() == PRO_AREA)
                    {
                        // Plot area
                        addArea(qcvs[i],name,indexy,values_x,values_output[indexy],count,vars[j].getLineStyle(),vars[j].getLineColor(),
                                vars[j].getBrushStyle(),vars[j].getBrushColor(),af,vars[j].getIgnored(),vars[j].getIgnored_val().toDouble());
                    }
                    else if (graphs[i].getType() == PRO_PIE)
                    {
                        // Plot pie
                        addPie(qcvs[i],name,indexy,values_x,values_output[indexy],count,vars[j].getLineStyle(),vars[j].getLineColor(),
                               vars[j].getBrushStyle(),vars[j].getBrushColor(),af,vars[j].getIgnored(),vars[j].getIgnored_val().toDouble());
                    }
                    else
                        // Plot series
                        addSeries(qcvs[i],name,indexy,values_x,values_output[indexy],count,vars[j].getLineStyle(),vars[j].getLineColor(),
                                  af,vars[j].getIgnored(),vars[j].getIgnored_val().toDouble());

                    // Add y axis label, if there is one
                    if (!vars[j].getYaxis().isEmpty())
                    {
                        for(int k=0;k<qcvs[i]->chart()->axes(Qt::Vertical).count();k++)
                        {
                            if (qcvs[i]->chart()->axes(Qt::Vertical)[k]->alignment() == af)
                                qcvs[i]->chart()->axes(Qt::Vertical)[k]->setTitleText(vars[j].getYaxis());
                        }
                    }
                }
            }
        }

        // Add missing y axis labels from graph
        for(int j=0;j<qcvs[i]->chart()->axes(Qt::Vertical).count();j++)
        {
            if (qcvs[i]->chart()->axes(Qt::Vertical)[j]->titleText().isEmpty())
                qcvs[i]->chart()->axes(Qt::Vertical)[j]->setTitleText(graphs[i].getYaxis());
        }

        // Add missing x axis labels from graph
        for(int j=0;j<qcvs[i]->chart()->axes(Qt::Horizontal).count();j++)
        {
            if (qcvs[i]->chart()->axes(Qt::Horizontal)[j]->titleText().isEmpty())
                qcvs[i]->chart()->axes(Qt::Horizontal)[j]->setTitleText(graphs[i].getXaxis());
        }

        // Show legend
        qcvs[i]->chart()->legend()->show();
    }
}

QString sliceLabel(QString name, double val)
{
    return name + " ( " + QString::number(val,'G',4) + " % ) ";
}

void MainWindow::addPie(QChartView *qcv, QString name, int varIndex, QList<double> xvals, QList<double> yvals, int count,
                         unsigned lineStyle, QString lineColor, int brushStyle, QString brushColor,
                         Qt::AlignmentFlag af, bool ignored, double ignored_val)
{
    QPieSeries *qps = NULL;
    QPieSlice  *qs  = new QPieSlice();
    QPen        p;
    QBrush      b;
    QFont       f;
    int         i   = 0;

    Q_UNUSED(ignored);
    Q_UNUSED(ignored_val);
    Q_UNUSED(af);
    Q_UNUSED(count);
    Q_UNUSED(xvals);

    // Delete axes
    while (qcv->chart()->axes().count()>0)
        qcv->chart()->removeAxis(qcv->chart()->axes()[0]);

    // Set time in properties
    qcv->chart()->setProperty(VAR_TIME_VAL.toStdString().c_str(),xvals.last());

    // Find a pie serie in chart
    while(qps == NULL && i<qcv->chart()->series().count())
    {
        qps = dynamic_cast<QPieSeries *>(qcv->chart()->series()[i]);
        i++;
    }

    // If there is no one, create a new one
    if (qps == NULL)
    {
        qps = new QPieSeries(qcv);
        qlps.append(qps);
        qps->setUseOpenGL(false);
        qps->installEventFilter(this);
        qcv->chart()->addSeries(qps);
    }

    // New slice properties
    qs->setLabel(sliceLabel(name,yvals.last()));
    qs->setValue(yvals.last());
    if (!lineColor.isEmpty())  qs->setBorderColor(QColor(lineColor));
    if (!brushColor.isEmpty()) qs->setColor(QColor(brushColor));
    p = qs->pen();
    p.setStyle(static_cast<Qt::PenStyle>(lineStyle));
    qs->setPen(p);
    b = qs->brush();
    if (brushStyle>=0) b.setStyle(static_cast<Qt::BrushStyle>(brushStyle));
    qs->setBrush(b);
    f = qs->labelFont();
    f.setBold(true);
    qs->setLabelFont(f);
    qs->setLabelPosition(QPieSlice::LabelPosition::LabelOutside);
    qs->setProperty(VAR_INDEX.toStdString().c_str(),varIndex);
    qs->setProperty(VAR_NAME.toStdString().c_str(),name);
    connect(qs, SIGNAL(hovered(bool)), this, SLOT(slice_hover(bool)));

    // Add slice to pie series
    qps->append(qs);

}

void MainWindow::addArea(QChartView *qcv, QString name, int varIndex, QList<double> xvals, QList<double> yvals, int count,
                         unsigned lineStyle, QString lineColor, int brushStyle, QString brushColor,
                         Qt::AlignmentFlag af, bool ignored, double ignored_val)
{
    QAreaSeries *qas = new QAreaSeries(qcv);
    QLineSeries *qus = new QLineSeries(qas); // Owned and delete with qas
    QLineSeries *qls = new QLineSeries(qas); // Owned and delete with qas
    int last = count>=0 ? count : xvals.count();

    Q_UNUSED(ignored);
    Q_UNUSED(ignored_val);

    // Add to list for deleting when necessary
    qlas.append(qas);

    // Min & max values
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    // Get min and max values for current series
    for(int i=0;i<last;i++)
    {
        long double yvalLower = qlu_points.count()>0 ? qlu_points.last()[i].y() : 0;
        long double yvalUpper = yvals[i]+yvalLower;

        qls->append(xvals[i],yvalLower);
        qus->append(xvals[i],yvalUpper);
        if (xvals[i]  < minX) minX = xvals[i];
        if (xvals[i]  > maxX) maxX = xvals[i];
        if (yvalUpper < minY) minY = yvalUpper;
        if (yvalUpper > maxY) maxY = yvalUpper;
    }

    // Add series to area
    qas->setUpperSeries(qus);
    qas->setLowerSeries(qlas.count()>1 ? qlas[qlas.count()-2]->upperSeries() : qls);

    // Cache for fast plotting
    qll_points.append(qas->lowerSeries()->points());
    qlu_points.append(qas->upperSeries()->points());

    // Add current series to chart
    qcv->chart()->addSeries(qas);

    // Use OpenGL
    qas->setUseOpenGL(false);

    // Set properties in series
    qas->setName(name);
    qas->setProperty(MIN_T.toStdString().c_str(),minX);
    qas->setProperty(MIN_Y.toStdString().c_str(),minY);
    qas->setProperty(MAX_T.toStdString().c_str(),maxX);
    qas->setProperty(MAX_Y.toStdString().c_str(),maxY);
    qas->setProperty(VAR_INDEX.toStdString().c_str(),varIndex);
    qas->installEventFilter(this);

    // Signals and slots
    connect(qas, SIGNAL(hovered(QPointF, bool)), this, SLOT(serie_hover(QPointF,bool)));

    // Area brush style
    QBrush b = qas->brush();
    if (brushStyle>=0) b.setStyle(static_cast<Qt::BrushStyle>(brushStyle));
    if (!brushColor.isEmpty()) b.setColor(QColor(brushColor));
    qas->setBrush(b);

    // Area line style
    QPen p = qas->pen();
    p.setStyle(static_cast<Qt::PenStyle>(lineStyle));
    if (!lineColor.isEmpty())  p.setColor(QColor(lineColor));
    else                       p.setColor(b.color());
    qas->setPen(p);

    // Set axes range
    setAxesRange(qcv,af);
}

void MainWindow::addSeries(QChartView *qcv, QString name, int varIndex, QList<double> xvals, QList<double> yvals, int count,
                           unsigned lineStyle, QString lineColor, Qt::AlignmentFlag af, bool ignored, double ignored_val)
{
    QLineSeries *qls = new QLineSeries(qcv);
    QList<QPointF> sp;
    int last = count>=0 ? count : xvals.count();

    // Add to list for deleting when necessary
    qlss.append(qls);

    // Min & max values
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    // Get min and max values for current series
    for(int i=0;i<last;i++)
    {
        if (!ignored  || yvals[i]!=ignored_val)
        {
            qls->append(xvals[i],yvals[i]);
            if (xvals[i] < minX) minX = xvals[i];
            if (xvals[i] > maxX) maxX = xvals[i];
            if (yvals[i] < minY) minY = yvals[i];
            if (yvals[i] > maxY) maxY = yvals[i];
        }
        sp.append(QPointF(xvals[i],yvals[i]));
    }

    // Cache for fast plotting
    qlss_points.append(sp);
    qlss_ignored.append(ignored);
    qlss_ignored_val.append(ignored_val);

    // Add current series to chart
    qcv->chart()->addSeries(qls);

    // Set properties in series
    qls->setName(name);
    qls->setProperty(MIN_T.toStdString().c_str(),minX);
    qls->setProperty(MIN_Y.toStdString().c_str(),minY);
    qls->setProperty(MAX_T.toStdString().c_str(),maxX);
    qls->setProperty(MAX_Y.toStdString().c_str(),maxY);
    qls->setProperty(VAR_INDEX.toStdString().c_str(),varIndex);
    qls->installEventFilter(this);

    // Use OpenGL
    qls->setUseOpenGL(false);

    // Signals and slots
    connect(qls, SIGNAL(hovered(QPointF, bool)), this, SLOT(serie_hover(QPointF,bool)));

    // Series style
    QPen p = qls->pen();
    p.setStyle(static_cast<Qt::PenStyle>(lineStyle));
    if (!lineColor.isEmpty()) p.setColor(QColor(lineColor));
    qls->setPen(p);

    // Set axes range
    setAxesRange(qcv,af);
}

void MainWindow::adjustSeries(int position)
{
    // For each line series
    for(int i=0;i<qlss.count();i++)
    {
        // Add points
        if (position>antPos)
        {
            if (!qlss_ignored[i])
            {
                // NOTE: Without considering ignored values
                // ----------------------------------------
                int count =  qlss[i]->count();
                qlss[i]->append(qlss_points[i].mid(count,position-(count-1)));
            }
            else
            {
                QList<QPointF> p;

                for(int j=antPos+1;j<=position;j++)
                {
                   if (qlss_ignored_val[i]!=qlss_points[i][j].y())
                        p.append(qlss_points[i][j]);
                }

                qlss[i]->append(p);
            }
        }

        // Remove points
        if (position<antPos)
        {
            if (!qlss_ignored[i])
            {
                // NOTE: Without considering ignored values
                // ----------------------------------------
                int count =  qlss[i]->count();
                qlss[i]->removePoints(position+1,count-position-1);
            }
            else
            {
                for(int j=antPos;j>position;j--)
                {
                    if (qlss_ignored_val[i]!=qlss_points[i][j].y())
                        qlss[i]->remove(qlss[i]->points().last());
                }
            }
        }
    }

    // WARNING: line series are shared between area series,
    // therefore they must be deleted only once.

    // Previous parent
    QObject *prev_parent = NULL;

    // For each area series
    for(int i=0;i<qlas.count();i++)
    {
        // To determine if they are in the same chart
        QObject *parent = qlas[i]->parent();

        // Add points
        if (position>antPos)
        {
            int count_l = qlas[i]->lowerSeries()->count();
            int count_u = qlas[i]->upperSeries()->count();
            qlas[i]->lowerSeries()->append(qll_points[i].mid(count_l,position-(count_l-1)));
            qlas[i]->upperSeries()->append(qlu_points[i].mid(count_u,position-(count_u-1)));
        }
        // Remove points
        if (position<antPos)
        {

            for(int j=antPos;j>position;j--)
            {
                if (prev_parent != parent) qlas[i]->lowerSeries()->remove(qlas[i]->lowerSeries()->points().last());
                qlas[i]->upperSeries()->remove(qlas[i]->upperSeries()->points().last());
            }
        }

        // Set pervious parent
        prev_parent = parent;
    }

    // For each pie series
    for(int i=0;i<qlps.count();i++)
    {
        QPieSeries *pie = qlps[i];

        // Set property for time value
        pie->chart()->setProperty(VAR_TIME_VAL.toStdString().c_str(), values_output[0][position]);

        // Set value for each slice
        for(int j=0;j<pie->count();j++)
        {
            QPieSlice *slice = pie->slices()[j];
            QString    name  = slice->property(VAR_NAME.toStdString().c_str()).toString();
            int        index = slice->property(VAR_INDEX.toStdString().c_str()).toInt();
            double     value = index>=0 && index<values_output.count() && position>=0 && position<values_output[index].count() ?
                               values_output[index][position] : 0;

            slice->setValue(value);
            slice->setLabel(sliceLabel(name,value));
        }
    }

    // Previous position
    antPos = position;
}

void MainWindow::on_sSim_sliderMoved(int position)
{
    if (isNotExecuting())
    {
        ui->lTsim->setText(QString::number(time_output[position]) + simSep + QString::number(xmlData.getPrjs()[indexPrj].getExp().stopTime) + simUnits);
        adjustSeries(position);
        evaluateLinks(position);
    }
}

void MainWindow::on_bStop_clicked()
{
    if (sim!=NULL) sim->stopSim();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::serie_hover(QPointF point, bool state)
{
    serieHover(point,state,sender(),&cmSerie);
}

void MainWindow::slice_hover(bool state)
{
    sliceHover(state,sender());
}

void MainWindow::on_actionZoomReset_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
        ((QChartView *)w)->chart()->zoomReset();
}

void MainWindow::on_actionZoomOut_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
        ((QChartView *)w)->chart()->zoomOut();
}

void MainWindow::setChartTheme(QChart::ChartTheme t)
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
        ((QChartView *)w)->chart()->setTheme(t);
}

void MainWindow::on_actionLight_triggered()
{
    setChartTheme(QChart::ChartThemeLight);
}

void MainWindow::on_actionBlue_cerulean_triggered()
{
    setChartTheme(QChart::ChartThemeBlueCerulean);
}

void MainWindow::on_actionDark_triggered()
{
    setChartTheme(QChart::ChartThemeDark);
}

void MainWindow::on_actionBorwn_sand_triggered()
{
    setChartTheme(QChart::ChartThemeBrownSand);
}

void MainWindow::on_actionBlue_NCS_triggered()
{
    setChartTheme(QChart::ChartThemeBlueNcs);
}

void MainWindow::on_actionHigh_contrast_triggered()
{
    setChartTheme(QChart::ChartThemeHighContrast);
}

void MainWindow::on_actionBlue_icy_triggered()
{
    setChartTheme(QChart::ChartThemeBlueIcy);
}

void MainWindow::on_actionQt_triggered()
{
    setChartTheme(QChart::ChartThemeQt);
}

void MainWindow::on_actionConfigureGraph_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QChartView *cv    = (QChartView *)w;
        QChart     *chart = cv->chart();
        graphConfig gc;

        // Title & icon
        gc.setWindowIcon(QIcon(xmlData.getIapp().getIcon()));
        // Stylesheet
        gc.deleteStyleSheet();
        gc.setStyleSheet(
                    "#lTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
                    "QGroupBox{background-color: white; border: none;}"
                    "QFrame{background-color: white; border: none;}"
                    "QTextBrowserborder: 1px solid rgb(201, 203, 205);}"
        );
        // Title
        gc.setTitleBrush(chart->titleBrush());
        gc.setTitleFont(chart->titleFont());
        gc.setTitleText(chart->title());
        // Legend
        gc.setLegendVisible(chart->legend()->isVisible());
        gc.setBackgroundVisible(chart->legend()->isBackgroundVisible());
        gc.setLabelFontColor(chart->legend()->labelColor());
        gc.setBackgroundPen(chart->legend()->pen());
        gc.setLegendFont(chart->legend()->font());
        gc.setBackgroundBrush(chart->legend()->brush());
        gc.setAlignment(chart->legend()->alignment());
        gc.setAttached(chart->legend()->isAttachedToChart());
        gc.setX(chart->legend()->geometry().top(),chart->legend()->geometry().height(),chart->geometry().height());
        gc.setY(chart->legend()->geometry().left(),chart->legend()->geometry().width(),chart->geometry().width());
        if (gc.exec())
        {
            // Title
            chart->setTitle(gc.getTitleText());
            chart->setTitleFont(gc.getTitleFont());
            chart->setTitleBrush(gc.getTitleBrush());
            // Legend
            chart->legend()->setVisible(gc.getLegedVisible());
            chart->legend()->setBackgroundVisible(gc.getBackgroundVisible());
            chart->legend()->setLabelColor(gc.getLabelFontColor());
            chart->legend()->setPen(gc.getBackgroundPen());
            chart->legend()->setFont(gc.getLegendFont());
            chart->legend()->setBrush(gc.getBackgroundBrush());
            if (gc.getAttached())
            {
                chart->legend()->attachToChart();
                chart->legend()->setAlignment(gc.getAlignment());
            }
            else
            {
                QRectF geo = chart->legend()->geometry();

                chart->legend()->detachFromChart();
                geo.setTop(gc.getX1());
                geo.setLeft(gc.getY1());
                geo.setHeight(gc.getX2());
                geo.setWidth(gc.getY2());
                chart->legend()->setGeometry(geo);
            }
            // Axis captions
            QValueAxis *ax = chart->axes(Qt::Horizontal).count()>0 ? (QValueAxis *)chart->axes(Qt::Horizontal)[0] : NULL;
            QValueAxis *ay = chart->axes(Qt::Vertical).count()>0   ? (QValueAxis *)chart->axes(Qt::Vertical)[0]   : NULL;
            if (ax!=NULL) ax->setTitleText(gc.getXcaption());
            if (ay!=NULL) ay->setTitleText(gc.getYcaption());
        }
    }
}

void MainWindow::on_actionConfigureSerie_triggered()
{
    seriesConfig *sc = new seriesConfig;

    // Get serie
    QLineSeries *pSerie = (QLineSeries *)cmSerie.property(REF_SER.toStdString().c_str()).value<void *>();

    //"QDialog {background-color: white;}"

    // Window icon
    sc->setWindowIcon(QIcon(xmlData.getIapp().getIcon()));
    // Style sheet
    sc->deleteStyleSheet();
    sc->setStyleSheet(
                "#lTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
                "QGroupBox{background-color: white; border: none;}"
                "QFrame{background-color: white; border: none;}"
                "QTextBrowserborder: 1px solid rgb(201, 203, 205);}"
    );

    sc->setPen(pSerie->pen());
    sc->setOpacity(pSerie->opacity());
    sc->setName(pSerie->name());
    sc->setPoints(pSerie->pointsVisible());
    sc->setPointsLabel(pSerie->pointLabelsVisible());
    sc->setPointsColor(pSerie->pointLabelsColor());
    sc->setPointsFont(pSerie->pointLabelsFont());
    sc->setPointsFormat(pSerie->pointLabelsFormat());
    sc->setPointsClippping(pSerie->pointLabelsClipping());
    if (sc->exec())
    {
        pSerie->setOpacity(sc->getOpacity());
        pSerie->setPen(sc->getPen());
        pSerie->setName(sc->getName());
        pSerie->setPointsVisible(sc->getPoints());
        pSerie->setPointLabelsVisible(sc->getPointsLabel());
        pSerie->setPointLabelsColor(sc->getPointsColor());
        pSerie->setPointLabelsFont(sc->getPointsFont());
        pSerie->setPointLabelsFormat(sc->getPointsFormat());
        pSerie->setPointLabelsClipping(sc->getPointsClipping());
    }
    delete sc;
}

void MainWindow::on_tRes_clicked(const QModelIndex &index)
{
    int      pos = ui->tRes->item(index.row(),0)->data(Qt::UserRole).toInt();
    resource res = xmlData.getExps()[indexPrj].getResources()[pos];

    ui->rDescription->setText(res.getDescription());

    if (index.column() == 1)
    {
        if (res.getType().toLower() == RT_FILE.toLower())
        {
            QFileInfo fi(res.getLink());
            QString   file = dir.path() + QDir::separator() + "resource" + QString::number(indexPrj+1) + "-" + QString::number(pos+1) + "." + fi.suffix();

            QFile::copy(res.getLink(),file);
            QDesktopServices::openUrl(QUrl::fromLocalFile(file));
        }
        else
            QDesktopServices::openUrl(res.getLink());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    if (xmlData.getIapp().getReadStoredGeo()) saveGeoConf();
    svgview.close();
}

int MainWindow::getPosExperiment()
{
    for(int i=0;i<act_exps->actions().count();i++)
        if (act_exps->actions()[i]->isChecked()) return i;
    return 0;
}

void MainWindow::setPosExperiment(int i)
{
    if (i>=0 && i<act_exps->actions().count())
        act_exps->actions()[i]->setChecked(true);
}

void MainWindow::saveGeoConf()
{
    // Main window
    writeGeometry(this,ID_MAIN_WIN);
    writeState(ui->tParameters->header(),ID_PAR_TREE);
    writeState(ui->tRes,ID_RES_TABLE);
    writeState(ui->splitter,ID_SPLITTER);
    writeState(ui->splitter_1,ID_SPLITTER1);
    writeState(ui->splitter_2,ID_SPLITTER2);
    writeState(ui->splitter_3,ID_SPLITTER3);
    writeOption(ID_CASE,ui->cbCaseSentitive->isChecked());
    writeOption(ID_GRAPHS_TAB,QString::number(ui->tGraphs->currentIndex()));
    writeOption(ID_DOC_TAB,QString::number(ui->tDocumentation->currentIndex()));
    writeOption(ID_EXP,getPosExperiment());

    //for(int i=0;i<lspliter.count();i++)
    //    writeState(lspliter[i],ID_SPLITTERD + QString::number(i));

    // SVG viewer
    writeGeometry(&svgview,ID_SVG_WIN);
}

void MainWindow::readGeoConf()
{
    // Main window
    readGeometry(this,ID_MAIN_WIN);
    readState(ui->tParameters->header(),ID_PAR_TREE);
    readState(ui->tRes,ID_RES_TABLE);
    readState(ui->splitter,ID_SPLITTER);
    readState(ui->splitter_1,ID_SPLITTER1);
    readState(ui->splitter_2,ID_SPLITTER2);
    readState(ui->splitter_3,ID_SPLITTER3);
    ui->cbCaseSentitive->setChecked(readOption(ID_CASE,false));
    ui->tGraphs->setCurrentIndex(readOption(ID_GRAPHS_TAB,QString("0")).toInt());
    ui->tDocumentation->setCurrentIndex(readOption(ID_DOC_TAB,QString("0")).toInt());
    setPosExperiment(readOption(ID_EXP,0));

    // SVG viewer
    readGeometry(&svgview,ID_SVG_WIN);
}

void MainWindow::on_actionChartFileImage_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QChartView  *qcv = ((QChartView *)w);
        chartToImageFile(this,qcv);
    }
}

void MainWindow::on_actionChartClipboardImage_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QChartView  *qcv = ((QChartView *)w);
        chartToImage(qcv);
    }
}

void MainWindow::on_actionChartClipboardText_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QList<QString>       names;
        QList<double>        time;
        QList<QList<double>> values;

        QChartView  *qcv = ((QChartView *)w);
        chartToData(qcv,time,values,names);
        copyVarsClipboard(time,values,names);
    }
}

void MainWindow::on_actionChartFileText_triggered()
{
    QWidget *w = NULL;

    if (focusItemGraph(w))
    {
        QList<QString>       names;
        QList<double>        time;
        QList<QList<double>> values;

        QChartView  *qcv = ((QChartView *)w);
        chartToData(qcv,time,values,names);
        copyVarsFile(this,time,values,names);
    }
}

void MainWindow::on_actionSerieClipboardText_triggered()
{
    // Get serie
    QList<double>        time;
    QList<QList<double>> values;
    QStringList          names;
    QList<double>        v;
    QString              n;
    QLineSeries *pSerie = (QLineSeries *)cmSerie.property(REF_SER.toStdString().c_str()).value<void *>();

    serieToData(pSerie,time,v,n);
    values.append(v);
    names.append(axisName(pSerie->attachedAxes(),Qt::AlignBottom));
    names.append(n);
    copyVarsClipboard(time,values,names);
}

void MainWindow::on_actionSerieFileText_triggered()
{
    // Get serie
    QList<double>        time;
    QList<QList<double>> values;
    QStringList          names;
    QList<double>        v;
    QString              n;
    QLineSeries *pSerie = (QLineSeries *)cmSerie.property(REF_SER.toStdString().c_str()).value<void *>();

    serieToData(pSerie,time,v,n);
    values.append(v);
    names.append(axisName(pSerie->attachedAxes(),Qt::AlignBottom));
    names.append(n);
    copyVarsFile(this,time,values,names);
}

void MainWindow::on_actionNumerical_integrator_triggered()
{
    integrator *inte = new integrator();
    opt_project  prj = xmlData.getPrjs()[indexPrj];

    inte->setWindowIcon(windowIcon());
    inte->deleteStyleSheet();
    inte->hideDefaultsButtons();
    inte->setStyleSheet(
                "#lTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
                "QDialog{background-color: white;}"
                "QGroupBox{background-color: white; border: none;}"
                "QFrame{background-color: white; border: none;}"
                "QTextBrowser{border: 1px solid rgb(201, 203, 205);}"
    );
    inte->setIntInfo(prj.getSim());
    if (inte->exec() == QDialog::Accepted)
    {
        prj.setSim(inte->getIntInfo());
        xmlData.setPrj(prj,indexPrj);
    }
    delete inte;
}

void MainWindow::on_actionOutputs_triggered()
{
    outputs     *out = new outputs();
    opt_project  prj = xmlData.getPrjs()[indexPrj];

    out->setWindowIcon(windowIcon());
    out->deleteStyleSheet();
    out->setStyleSheet(
                "#lTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
                "QDialog{background-color: white;}"
                "QGroupBox{background-color: white; border: none;}"
                "QFrame{background-color: white; border: none;}"
                "QTextBrowserborder: 1px solid rgb(201, 203, 205);}"
    );
    out->setModelExp(prj);
    if (out->exec())
    {
        prj.setExp(out->getExp());
        xmlData.setPrj(prj,indexPrj);
    }
    delete out;
}

void MainWindow::on_action_About_triggered()
{
    ab.setDefaultGUI();
    ab.setWindowIcon(windowIcon());
    ab.exec();
}

void MainWindow::on_bResults_clicked()
{
    QList<variable>      vars;
    QList<QList<double>> values;
    opt_project          prj = xmlData.getPrjs()[indexPrj];

    // If there is a new simulation, add it
    if (newResults)
    {

        // Add time variable (indepedent variable) for plotting
        vars.append(varTimeDef());
        values.append(time_output);

        // --------
        // WARNING: Ignore inputs
        // --------
        // Add inputs
        //if (prj.getModel().inputs.count()>0)
        //{
        //    vars.append(prj.getModel().inputs);
        //    values.append(prj.getExp().values_input);
        //}

        // Add outputs
        if (prj.getModel().outputs.count()>0)
        {
            vars.append(prj.getModel().outputs);
            values.append(values_output);
        }

        // Flag for new results
        newResults = false;

        // Load data in plotting tool
        plot->loadData(vars,values,xmlData.getExps()[indexPrj].getDescription());
    }

    // Style sheet
    plot->setWindowIcon(QIcon(xmlData.getIapp().getIcon()));
    plot->setStyleDialogs(
                "#lTitle {background-color: " + cTitle + "; color: white; border-radius: 10px;}"
                "QGroupBox{background-color: white; border: none;}"
                "QFrame{background-color: white; border: none;}"
                "QTextBrowserborder: 1px solid rgb(201, 203, 205);}"
                );
    plot->setStyleSheet(
                "QComboBox{background-color: white;}"
                "QGroupBox{background-color: white; border-style: none;}"
                "QSplitter::handle:vertical   {image: url(:icons/hsplitter.png);}"
                "QSplitter::handle:horizontal {image: url(:icons/vsplitter.png);}"
                );
    // Show window
    plot->setWindowModality(Qt::ApplicationModal);
    plot->show();
}

void MainWindow::on_splitter_1_splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);

    splitterMoved();
}

void MainWindow::on_splitter_2_splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);

    splitterMoved();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   splitterMoved();
}

void MainWindow::on_actionClipboard_triggered()
{
    imageToImg(getImage(svgTemp),true,sEmpty);
}

void MainWindow::on_actionSave_triggered()
{
    int pos = svgTemp->property(DOC_REF.toStdString().c_str()).toInt();

    if (pos>=0 && pos<ldoc.count())
        saveSVGimage(this,ldoc[pos],svgTemp);
}

void MainWindow::on_actionClipboardTable_triggered()
{
    if (tableTemp->selectedRanges().count()>0)
    {
        QTableWidgetSelectionRange range = tableTemp->selectedRanges()[0];
        copyClipboard(tableTemp,range.topRow(),range.leftColumn(),range.rowCount(),range.columnCount(),false);
    }
}

void MainWindow::on_actionClipboardTableHeader_triggered()
{
    if (tableTemp->selectedRanges().count()>0)
    {
        QTableWidgetSelectionRange range = tableTemp->selectedRanges()[0];
        copyClipboard(tableTemp,range.topRow(),range.leftColumn(),range.rowCount(),range.columnCount(),true);
    }
}

void MainWindow::on_tbRestore_clicked()
{
    opt_project prj = xmlData.getPrjs()[indexPrj];

    // Delete changed values
    prj.getModel().p_name_new.clear();
    prj.getModel().p_value_new.clear();

    // Set original values
    for(int i=0;i<lpar_widget.count();i++)
    {
        QLineEdit *le = dynamic_cast<QLineEdit *>(lpar_widget[i]);
        QSpinBox  *sb = dynamic_cast<QSpinBox  *>(lpar_widget[i]);
        QComboBox *cb = dynamic_cast<QComboBox *>(lpar_widget[i]);

        if      (le!=NULL) le->setText(le->property(PAR_PRO_VALUE.toLatin1().data()).toString());
        else if (sb!=NULL) sb->setValue(sb->property(PAR_PRO_VALUE.toLatin1().data()).toDouble());
        else if (cb!=NULL) cb->setCurrentText(cb->property(PAR_PRO_VALUE.toLatin1().data()).toString());
    }

    // Save current project
    xmlData.setPrj(prj,indexPrj);
}

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryDir>
#include <QtCharts/QtCharts>

#include "commonsimint.h"
#include "simulation.h"
#include "svgviewer.h"
#include "about.h"
#include "plotgraphs.h"

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool getInitialized() const;

private:
    Ui::MainWindow *ui;
    bool initialized;
    bool ajustedDia = false;
    QTemporaryDir dir;
    XmlContentHandler xmlData;
    QMenu cmGraph;
    QMenu cmSerie;
    QMenu cmStyle;
    QMenu cmClipboardGraph;
    QMenu cmSaveGraph;
    QMenu cmSVG;
    QMenu cmTable;
    QFrame fmain;
    SVGViewer svgview;   
    about     ab;
    QSvgWidget *svgTemp;
    QTableWidget *tableTemp;

    // Current project
    int indexPrj;

    // Dynamic objects
    QActionGroup *      act_exps = NULL;
    QList<QWidget *>    tabs;
    QList<QChartView *> qcvs;
    QList<QValueAxis *> axis;
    QList<QLineSeries*> qlss;
    QList<QAreaSeries*> qlas;
    simulation         *sim;
    QThread            *thrSim;
    QList<QLayout *>   llayouts;
    QList<QFrame *>    lauthors;
    QVBoxLayout         *gau;
    QList<QFrame *>      lfau;
    QList<QGridLayout *> llau;
    QList<QLabel *>      licon;
    QList<QLabel *>      lemail;
    QList<QLabel *>      lname;
    QList<QLabel *>      lcompany;
    QList<QLabel *>      lweb;
    QList<QLabel *>      lrole;
    RenderLinkDelegate  *rl;
    QWidget             *pRes = NULL;
    QList<QTableWidget *>ltabs;
    QList<QSvgWidget *>  lsvg;
    QList<QSplitter *>   lspliter;
    QList<QWidget *>     lwidget;
    QList<double>        lar;
    QList<QDomDocument>  ldoc;

    // Plotting
    QList<QList<QPointF>> qlss_points;
    QList<QList<QPointF>> qll_points;
    QList<QList<QPointF>> qlu_points;
    QList<bool>           qlss_ignored;
    QList<double>         qlss_ignored_val;
    int                   antPos;

    // Methods
    void setDefaultChart(QChartView *qcv);
    void populateTree(opt_project prj);
    void restoreSim(opt_project prj);
    void configureGeometry();
    void setState();
    void simThread();
    bool isNotExecuting();
    bool isExecuting();
    bool isSimulated();
    void sendToGUI(QString msg, QString styleSheet = "");
    void updatePlots(int count = -1);
    void addSeries(QChartView *qcv, QString name, int varIndex, QList<double> xvals, QList<double> yvals, int count, unsigned lineStyle, QString lineColor, Qt::AlignmentFlag af, bool ignored, double ignored_val);
    void addArea(QChartView *qcv, QString name, int varIndex, QList<double> xvals, QList<double> yvals, int count, unsigned lineStyle, QString lineColor, unsigned brushStyle, QString brushColor, Qt::AlignmentFlag af, bool ignored, double ignored_val);
    void adjustsSim();
    void adjustSeries(int position);
    void configureConMenus();
    void setChartTheme(QChart::ChartTheme t);
    void configureDocumentation();
    void configureResource();
    void configureAuthors();
    void configureDiagram();
    void setDiagramSize();
    void evaluateLinks(int position);
    void evaluateLinkAssign(QDomDocument doc, link l, int position);
    void evaluateLinkText(QDomDocument doc, link l, int position);
    double getVariableValue(QString var, int position);
    void assignSvgPro(QDomDocument doc, QString id, QString property, double val);
    void assignSvgText(QDomDocument &doc, QString id, QString value);
    void setAppStyleSheet();
    void configureApp();
    void saveGeoConf();
    void readGeoConf();
    int getPosExperiment();
    void setPosExperiment(int i);
    void addGraphs(experiment exp);
    void addDiagramTables(experiment exp);
    bool checkInputs();

    // Values for parameters
    QList<bool>      changed;
    QList<QString>   new_value;
    QList<QWidget *> lpar_widget;

    // Results
    QList<QList<double> > values_output;
    QList<double>         time_output;

    // Colors
    QString cTitle;
    QString cBackground;
    QString cHeader;
    QString cFontHeader;
    QString cFontTitle;

    // Plotting tool
    plotgraphs  *plot;

    // New simulation results
    bool newResults;

private slots:
    // Event filters
    bool eventFilter(QObject *, QEvent *);
    // Menu
    void experimentMenuActionTriggered(QAction * action);
    // Parameter columns
    void parameterMenuActionTriggered(QAction *action);
    // Parameters
    void on_le_textChanged(const QString &arg1);
    void on_sb_valueChanged(int arg1);
    void on_cb_textChanged(const QString &arg1);
    void on_tbClear_clicked();
    void on_lineFilter_returnPressed();
    void on_tParameters_customContextMenuRequested(const QPoint &pos);
    // Simulation
    void simUpdate(double t, double startTime, double stopTime);
    void simError(unsigned simStatus, QString msg);
    void simFinished(double t, opt_exp exp, opt_problem prb, unsigned status);
    void on_bSimulate_clicked();
    void on_sSim_sliderMoved(int position);
    void on_bStop_clicked();
    void on_actionExit_triggered();
    // Chart / series
    void onCustomContextMenu(const QPoint &p);
    void serie_hover(QPointF point, bool state);
    void on_actionZoomReset_triggered();
    void on_actionZoomOut_triggered();
    void on_actionLight_triggered();
    void on_actionBlue_cerulean_triggered();
    void on_actionDark_triggered();
    void on_actionBorwn_sand_triggered();
    void on_actionBlue_NCS_triggered();
    void on_actionHigh_contrast_triggered();
    void on_actionBlue_icy_triggered();
    void on_actionQt_triggered();
    void on_actionConfigureGraph_triggered();
    void on_actionConfigureSerie_triggered();
    void on_tRes_clicked(const QModelIndex &index);
    void closeEvent(QCloseEvent *event);
    void on_actionChartFileImage_triggered();
    void on_actionChartClipboardImage_triggered();
    void on_actionChartClipboardText_triggered();
    void on_actionChartFileText_triggered();
    void on_actionSerieClipboardText_triggered();
    void on_actionSerieFileText_triggered();
    void on_actionNumerical_integrator_triggered();
    void on_actionOutputs_triggered();
    void on_action_About_triggered();
    void on_bResults_clicked();
    void splitterMoved();
    void on_splitter_1_splitterMoved(int pos, int index);
    void on_splitter_2_splitterMoved(int pos, int index);
    void resizeEvent(QResizeEvent* event);
    void on_actionClipboard_triggered();
    void customContextMenuRequestedDiagram(QPoint pos);
    void customContextMenuRequestedTable(QPoint pos);
    void on_actionSave_triggered();
    void showEvent(QShowEvent *);
    void on_actionClipboardTable_triggered();
    void on_actionClipboardTableHeader_triggered();
    void on_tbRestore_clicked();
};

#endif // MAINWINDOW_H

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
#ifndef COMMONSIMINT_H
#define COMMONSIMINT_H

#include <QString>
#include <QXmlSimpleReader>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QDomDocument>
#include <QSvgWidget>

#include "opt_project.h"

const QString algLeft  = "left";
const QString algRight = "right";

const QString RT_FILE = "file";
const QString RT_URL  = "url";

const QString LINK_ASSIGN = "assign";
const QString LINK_TEXT   = "text";

class RenderLinkDelegate : public QStyledItemDelegate
{
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class author
{
public:
    author(QString n, QString e, QString w, QString c, QString r);

    QString getName() const;
    void setName(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    QString getWebsite() const;
    void setWebsite(const QString &value);

    QString getCompany() const;
    void setCompany(const QString &value);

    QString getRole() const;
    void setRole(const QString &value);

private:

    QString name;
    QString email;
    QString website;
    QString company;
    QString role;
};

class app
{

public:
    QString getWindowName() const;
    void setWindowName(const QString &value);
    QString getTitleName() const;
    void setTitleName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QString getVersion() const;
    void setVersion(const QString &value);
    QString getCompany() const;
    void setCompany(const QString &value);
    QString getLogo() const;
    void setLogo(const QString &value);
    QString getBackColor() const;
    void setBackColor(const QString &value);
    QString getTitleColor() const;
    void setTitleColor(const QString &value);
    QString getSectionColor() const;
    void setSectionColor(const QString &value);
    QString getFontTitleColor() const;
    void setFontTitleColor(const QString &value);
    QString getFontSectionColor() const;
    void setFontSectionColor(const QString &value);
    QString getIcon() const;
    void setIcon(const QString &value);
    bool getConf_integrator() const;
    void setConf_integrator(bool value);
    bool getConf_inputs() const;
    void setConf_inputs(bool value);
    bool getConf_outputs() const;
    void setConf_outputs(bool value);
    bool getSteady() const;
    void setSteady(bool value);
    QString getExperimentMenu() const;
    void setExperimentMenu(const QString &value);
    QString getExperimentTitle() const;
    void setExperimentTitle(const QString &value);
    bool getResults() const;
    void setResults(bool value);

    bool getReadStoredGeo() const;
    void setReadStoredGeo(bool value);

private:

    QString windowName;
    QString titleName;
    QString description;
    QString version;
    QString company;
    QString logo;
    QString backColor;
    QString titleColor;
    QString sectionColor;
    QString fontTitleColor;
    QString fontSectionColor;
    QString experimentMenu;
    QString experimentTitle;
    QString icon;
    bool    conf_integrator;
    bool    conf_inputs;
    bool    conf_outputs;
    bool    steady;
    bool    results;
    bool    readStoredGeo;
};

class geometry
{
public:
    geometry()
    {setHeight(0); setWidth(0); setPos_hslider(0); setPos_vslider(0);}
    geometry(int h, int w, double vs, double hs)
    {setHeight(h); setWidth(w); setPos_hslider(hs); setPos_vslider(vs);}
    int getHeight() const;
    int getWidth() const;
    void setWidth(int value);
    double getPos_vslider() const;
    double getPos_hslider() const;
    void setPos_hslider(double value);
    void setHeight(int value);
    void setPos_vslider(double value);
    //double getPos_documents() const;
    //void setPos_documents(double value);
    //int getWidth_name() const;
    //void setWidth_name(int value);
    //int getWidth_document() const;
    //void setWidth_document(int value);

private:
    int     height;
    int     width;
    double  pos_vslider;
    double  pos_hslider;
};

class column
{
public:
    column(){setSize(0);}
    column(QString n, int s){setName(n);setSize(s);}
    QString getName() const;
    void setName(const QString &value);
    int getSize() const;
    void setSize(const int &value);

private:
    QString name;
    int     size;
};

class var
{

public:
    var(QString n, QString d, unsigned l, QString c, QString y, QString ya, bool i = false, QString iv=0, QString x="")
    {setName(n); setDescription(d); setLineStyle(l); setLineColor(c); setYalign(y); setYaxis(ya); setIgnored(i); setIgnored_val(iv); setX(x);}
    QString getName() const;
    void setName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    unsigned getLineStyle() const;
    void setLineStyle(const unsigned &value);
    QString getYalign() const;
    void setYalign(const QString &value);
    QString getYaxis() const;
    void setYaxis(const QString &value);
    QString getLineColor() const;
    void setLineColor(const QString &value);
    bool getIgnored() const;
    void setIgnored(bool value);
    QString getIgnored_val() const;
    void setIgnored_val(const QString &value);
    QString getX() const;
    void setX(const QString &value);

private:
    QString  name;
    QString  x;
    QString  description;
    unsigned lineStyle;
    QString  lineColor;
    QString  yalign;
    QString  yaxis;
    bool     ignored;
    QString  ignored_val;
};

class graph
{
public:
    graph(QString n, QString xa, QString ya){setName(n); setXaxis(xa); setYaxis(ya);}
    QString getName() const;
    void setName(const QString &value);
    QString getXaxis() const;
    void setXaxis(const QString &value);
    QString getYaxis() const;
    void setYaxis(const QString &value);
    QList<var> getVars() const;
    void setVars(const QList<var> &value);
    void addVar(const var value);

private:
    QString name;
    QString xaxis;
    QString yaxis;
    QList<var> vars;
};

class link
{

public:
    link(QString t, QString i, QString p, QString v, double pmi, double pma, double vmi, double vma)
    {setType(t); setId(i); setProperty(p); setVariable(v); setPro_min(pmi); setPro_max(pma); setVar_min(vmi); setVar_max(vma);}
    link(QString t, QString i, QString v, QString u, QString d="")
    {setType(t); setId(i); setVariable(v); setUnits(u); setDesc(d);}
    QString getType() const;
    void setType(const QString &value);
    QString getId() const;
    void setId(const QString &value);
    QString getProperty() const;
    void setProperty(const QString &value);
    QString getVariable() const;
    void setVariable(const QString &value);
    double getPro_min() const;
    void setPro_min(double value);
    double getPro_max() const;
    void setPro_max(double value);
    double getVar_min() const;
    void setVar_min(double value);
    double getVar_max() const;
    void setVar_max(double value);
    QString getUnits() const;
    void setUnits(const QString &value);
    QString getDesc() const;
    void setDesc(const QString &value);

private:
    QString type;
    QString  id;
    QString  property;
    QString  variable;
    QString  units;
    QString  desc;
    double   pro_min;
    double   pro_max;
    double   var_min;
    double   var_max;
};

class diagramTable
{
public:
    diagramTable(QString name, double p, bool h){setName(name); setPos_hslider(p); setHorizontal(h);}
    QString getName() const;
    void setName(const QString &value);
    QString getPicture() const;
    void setPicture(const QString &value);
    QList<link> getDLinks() const;
    void setDLinks(const QList<link> &value);
    QList<link> getTLinks() const;
    void setTLinks(const QList<link> &value);
    void addDLink(const link value);
    void addTLink(const link value);
    double getPos_hslider() const;
    void setPos_hslider(double value);
    int getWidth_name() const;
    void setWidth_name(int value);
    int getWidth_desc() const;
    void setWidth_desc(int value);
    int getWidth_value() const;
    void setWidth_value(int value);
    int getWidth_units() const;
    void setWidth_units(int value);
    bool getHorizontal() const;
    void setHorizontal(bool value);

private:
    QString name;
    double pos_hslider;
    QString picture;
    QList<link> dLinks;
    QList<link> tLinks;
    int width_name  = 0;
    int width_desc  = 0;
    int width_value = 0;
    int width_units = 0;
    bool horizontal;
};

class resource
{
public:
    resource(QString t, QString n, QString d, QString l, QString des){setType(t); setName(n); setDocument(d); setLink(l); setDescription(des);}
    QString getType() const;
    void setType(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getDocument() const;
    void setDocument(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QString getLink() const;
    void setLink(const QString &value);

private:
    QString type;
    QString name;
    QString document;
    QString link;
    QString description;
};

class experiment
{

public:
    experiment(QString n, QString d, QString g){setName(n); setDescription(d); setGroup(g);}
    QString getName() const;
    void setName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QList<graph> getGraphs() const;
    void setGraphs(const QList<graph> &value);
    void addGraph(const graph value);
    void addDiaTab(const diagramTable value);
    double getPos_diagram() const;
    void setPos_diagram(double value);
    QString getThumb() const;
    void setThumb(const QString &value);
    QString getPicture() const;
    void setPicture(const QString &value);
    QString getPicture_desc() const;
    void setPicture_desc(const QString &value);
    QList<link> getLinks() const;
    void setLinks(const QList<link> &value);
    void addLink(const link value);
    QString getGroup() const;
    void setGroup(const QString &value);
    QList<resource> getResources() const;
    void setResources(const QList<resource> &value);
    void addResource(const resource r);
    double getPos_documents() const;
    void setPos_documents(double value);
    int getWidth_name() const;
    void setWidth_name(int value);
    int getWidth_document() const;
    void setWidth_document(int value);
    QList<diagramTable> getDiaTabs() const;
    void setDiaTabs(const QList<diagramTable> &value);

private:
    QString name;
    QString description;
    QString group;
    QList<graph> graphs;
    QList<diagramTable> diaTabs;
    double  pos_diagram;
    QString thumb;
    QString picture;
    QString picture_desc;
    QList<link> links;
    QList<resource> resources;
    double  pos_documents;
    int     width_name;
    int     width_document;
};

class XmlContentHandler :public QXmlDefaultHandler
{
  public:
    XmlContentHandler():QXmlDefaultHandler(){}
    ~XmlContentHandler(){}

    const QString getPro(const QXmlAttributes &atts, const QString pro);
    int           getProInt(const QXmlAttributes &atts, const QString pro);
    double        getProDouble(const QXmlAttributes &atts, const QString pro);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &atts );

    app           getIapp()    const;
    QList<author> getAuthors() const;
    QString getPath() const;
    void setPath(const QString &value);
    QList<opt_project> getPrjs() const;
    void setPrj(const opt_project prj, int i);
    QList<QString> getPrj_desc() const;
    QList<experiment> getExps() const;
    geometry getGeo() const;
    QList<column> getCols() const;

private:
    app                iapp;
    geometry           geo;
    QList<column>      cols;
    QList<author>      authors;
    QList<opt_project> prjs;
    QList<experiment>  exps;
    QString            path;
};

bool  xmlSimIntParser(QString &error, QString path, XmlContentHandler &xmlData);
void changeSvgAttr(QDomDocument &doc, QString type, const QString id, QString property, QString value);
void  setSvgAttr(QDomElement &elem, QString strattr, QString strattrval);
void setSvgText(QDomDocument doc, QDomElement &elem, QString strattrval);
QIcon renderSVG(QDomDocument doc);
void domDocToSVG(QDomDocument doc, QString filename);
void domDocToPDF(QDomDocument doc, QString filename, int widthMM, int heightMM);
bool XML_to_QDom(QString filename, QDomDocument &doc);
void SVG_setSize(QSvgWidget *view, double &aspect_ratio, int w, int h);
QDomElement findElement(QDomNode node, QString id, QString value);
bool SVG_size(QDomDocument dom, int &width, int &height);
QImage getImage(QSvgWidget *view);
void saveSVGimage(QWidget *w, QDomDocument doc, QSvgWidget *view);
void SVG_refresh(QDomDocument doc, QSvgWidget *view, int width, int height, double ar);

#endif // COMMONSIMINT_H

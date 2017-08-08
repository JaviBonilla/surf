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

#include "commonsimint.h"
#include "commonapp.h"
#include "common.h"

#include <QErrorMessage>
#include <QApplication>
#include <QDebug>
#include <QStyleOptionViewItem>
#include <QSvgRenderer>
#include <QPrinter>
#include <QFileDialog>

const QString xmlConFileName = ":configfile";

const QString TAG_APP         = "app";
const QString TAG_AUTHOR      = "author";
const QString TAG_EXPERIMENT  = "experiment";
const QString TAG_PARAMETER   = "parameter";
const QString TAG_PARAMETERS  = "parameters";
const QString TAG_INTEGRATION = "integration";
const QString TAG_INPUTS      = "inputs";
const QString TAG_INPUT       = "input";
const QString TAG_OUTPUTS     = "outputs";
const QString TAG_GRAPH       = "graph";
const QString TAG_VARIABLE    = "variable";
const QString TAG_GEOMETRY    = "geometry";
const QString TAG_RESOURCES   = "resources";
const QString TAG_RESOURCE    = "resource";
const QString TAG_DIAGRAM     = "diagram";
const QString TAG_LINK        = "link";
const QString TAG_ID          = "id";
const QString TAG_G_DIAGRAM   = "g_diagram";
const QString TAG_G_TABLE     = "g_table";
const QString TAG_GD_LINK     = "gd_link";
const QString TAG_GT_LINK     = "gt_link";

const QString PRO_COLUMNS = "columns";
const QString PRO_SIZES   = "sizes";

const QString PRO_WIDTH_NAME     = "width_name";
const QString PRO_WIDTH_DESC     = "width_desc";
const QString PRO_WIDTH_DOCUMENT = "width_document";
const QString PRO_WIDTH_UNITS    = "width_units";
const QString PRO_WIDTH_VALUE    = "width_value";

const QString PRO_HEIGHT      = "height";
const QString PRO_WIDTH       = "width";
const QString PRO_POS_VSLIDER = "pos_vslider";
const QString PRO_POS_HSLIDER = "pos_hslider";
const QString PRO_POS_SLIDER  = "pos_slider";
const QString PRO_HORIZONTAL  = "horizontal";

const QString PRO_XAXIS       = "xaxis";
const QString PRO_YAXIS       = "yaxis";
const QString PRO_YALIGN      = "yalign";
const QString PRO_IGNORED_VAL = "ignored_val";
const QString PRO_X           = "x";

const QString PRO_NAME    = "name";
const QString PRO_EMAIL   = "email";
const QString PRO_WEBSITE = "website";
const QString PRO_COMPANY = "company";
const QString PRO_ROLE    = "role";

const QString PRO_LOGO               = "logo";
const QString PRO_ICON               = "icon";
const QString PRO_TITLE_COLOR        = "title_color";
const QString PRO_BACK_COLOR         = "background_color";
const QString PRO_SECTION_COLOR      = "section_color";
const QString PRO_FONT_TITLE_COLOR   = "font_title_color";
const QString PRO_FONT_SECTION_COLOR = "font_section_color";
const QString PRO_CONF_INTEGRATOR    = "conf_integrator";
const QString PRO_CONF_INPUTS        = "conf_experiment";
const QString PRO_CONF_OUTPUTS       = "conf_outputs";
const QString PRO_RESULTS            = "results";

const QString PRO_TITLENAME   = "titlename";
const QString PRO_WINDOWNAME  = "windowname";
const QString PRO_DESCRIPTION = "description";
const QString PRO_VERSION     = "version";
const QString PRO_LINESTYLE   = "linestyle";
const QString PRO_LINECOLOR   = "linecolor";
const QString PRO_BRUSHSTYLE  = "brushstyle";
const QString PRO_BRUSHCOLOR  = "brushcolor";
const QString PRO_GROUP       = "group";
const QString PRO_STEADY      = "steady";
const QString PRO_EXP_MENU    = "experiment_text_menu";
const QString PRO_EXP_TITLE   = "experiment_text_title";

const QString PRO_FMUNAME   = "fmuname";
const QString PRO_FMUFILE   = "fmufile";
const QString PRO_INPUTFILE = "inputfile";

const QString PRO_SOLVER   = "solver";
const QString PRO_ABSTOL   = "abstolerance";
const QString PRO_RELTOL   = "reltolerance";
const QString PRO_ORDER    = "order";
const QString PRO_STEPSIZE = "stepsize";

const QString PRO_VALUE = "value";

const QString PRO_KIND   = "kind";
const QString PRO_NUMBER = "number";
const QString PRO_STEP   = "step";

const QString PRO_START     = "start";
const QString PRO_STOP      = "stop";
const QString PRO_REDUCTION = "reduction";
const QString PRO_TIMEKIND  = "timekind";
const QString PRO_INTERVALS = "intervals";
const QString PRO_TIMEVAR   = "timevar";

const QString PRO_TYPE     = "type";
const QString PRO_DOCUMENT = "document";
const QString PRO_LINK     = "link";

const QString PRO_THUMB   = "thumbnail";
const QString PRO_PICTURE = "picture";

const QString INPUT_FILENAME = "input.csv";

const QString PRO_SEP = ",";

const QString PRO_ID       = "id";
const QString PRO_PROPERTY = "property";
const QString PRO_VARIABLE = "variable";
const QString PRO_PRO_MIN  = "pro_min";
const QString PRO_PRO_MAX  = "pro_max";
const QString PRO_VAR_MIN  = "var_min";
const QString PRO_VAR_MAX  = "var_max";
const QString PRO_UNITS    = "units";

// Graph types
const QString PRO_DIAGRAM_TABLE = "diagram_table";

const QString PRO_TEXT        = "text";
const QString PRO_STORED_GEO  = "store_geo";


void RenderLinkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty()) return;

    painter->save();

    QFont font = option.font;
    font.setUnderline(true);
    painter->setFont(font);
    painter->setPen(option.palette.link().color());

    //if (option.state & QStyle::State_MouseOver)
    //    QApplication::setOverrideCursor(Qt::PointingHandCursor);
    //else
    //    QApplication::restoreOverrideCursor();

    QStyleOptionViewItem option2 = option;
    option2.rect.setLeft(option.rect.left()+4);
    painter->drawText(option2.rect, Qt::AlignLeft | Qt::AlignVCenter, text);
    painter->restore();
}

const QString XmlContentHandler::getPro(const QXmlAttributes &atts, const QString pro)
{
    int pos = atts.index(pro);
    if (pos >= 0) return atts.value(pos);
    return sEmpty;
}

int XmlContentHandler::getProInt(const QXmlAttributes &atts, const QString pro)
{
    QString p = getPro(atts,pro);
    return (p.isEmpty() ? 0 : p.toInt());
}

double XmlContentHandler::getProDouble(const QXmlAttributes &atts, const QString pro)
{
    QString p = getPro(atts,pro);
    return (p.isEmpty() ? 0 : p.toDouble());
}

bool XmlContentHandler::startElement(const QString &namespaceURI, const QString &localName,
                const QString &qName, const QXmlAttributes &atts )
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);

    // Local variables for reading properties
    QString val,val2;

    // Tag - app
    if (localName.toLower() == TAG_APP)
    {
        // App info
        iapp.setTitleName(getPro(atts,PRO_TITLENAME));
        iapp.setWindowName(getPro(atts,PRO_WINDOWNAME));
        iapp.setDescription(getPro(atts,PRO_DESCRIPTION));
        iapp.setVersion(getPro(atts,PRO_VERSION));
        iapp.setCompany(getPro(atts,PRO_COMPANY));
        iapp.setLogo(getPro(atts,PRO_LOGO));
        iapp.setTitleColor(getPro(atts,PRO_TITLE_COLOR));
        iapp.setBackColor(getPro(atts,PRO_BACK_COLOR));
        iapp.setSectionColor(getPro(atts,PRO_SECTION_COLOR));
        iapp.setFontTitleColor(getPro(atts,PRO_FONT_TITLE_COLOR));
        iapp.setFontSectionColor(getPro(atts,PRO_FONT_SECTION_COLOR));
        iapp.setIcon(getPro(atts,PRO_ICON));
        iapp.setConf_integrator(getPro(atts,PRO_CONF_INTEGRATOR).toLower().trimmed() == sTrue.toLower());
        iapp.setConf_inputs(getPro(atts,PRO_CONF_INPUTS).toLower().trimmed() == sTrue.toLower());
        iapp.setConf_outputs(getPro(atts,PRO_CONF_OUTPUTS).toLower().trimmed() == sTrue.toLower());
        iapp.setSteady(getPro(atts,PRO_STEADY).toLower().trimmed() == sTrue.toLower());
        iapp.setExperimentMenu(getPro(atts,PRO_EXP_MENU));
        iapp.setExperimentTitle(getPro(atts,PRO_EXP_TITLE));
        iapp.setResults(getPro(atts,PRO_RESULTS).toLower().trimmed() == sTrue.toLower());
        iapp.setReadStoredGeo(getPro(atts,PRO_STORED_GEO).toLower().trimmed() == sTrue.toLower());
    }
    // Tag - author
    else if (localName.toLower() == TAG_AUTHOR)
    {
        // Each author info
        author a(getPro(atts,PRO_NAME),getPro(atts,PRO_EMAIL),getPro(atts,PRO_WEBSITE),getPro(atts,PRO_COMPANY),getPro(atts,PRO_ROLE));
        authors.append(a);
    }
    // Tag - experiment
    else if (localName.toLower() == TAG_EXPERIMENT)
    {
        // Add new experiment
        exps.append(experiment(getPro(atts,PRO_NAME),getPro(atts,PRO_DESCRIPTION),getPro(atts,PRO_GROUP)));

        // Add new project
        prjs.append(opt_project(ptSimulation));

        // Get index
        int i = prjs.count()-1;

        // FMU file - copy to a temporary directory
        opt_model     mo = prjs[i].getModel();
        QString rFMUfile = getPro(atts,PRO_FMUFILE);
        // Orignal model name for FMU simulation
        QString mFMUfile = path + QDir::separator() + getPro(atts,PRO_FMUNAME);

        if (!rFMUfile.isEmpty())
        {
            QFile::copy(rFMUfile, mFMUfile);
            mo.setFMUfileName(fileRelativeToProject(&prjs[i],mFMUfile),projectPath(&prjs[i]));
            prjs[i].setModel(mo);
        }

        // Input file - copy to a temporary directory
        opt_exp        exp = prjs[i].getExp();
        QString rInputfile = getPro(atts,PRO_INPUTFILE);
        QString mInputfile = path + QDir::separator() + QString::number(i) + INPUT_FILENAME;

        if (!rInputfile.isEmpty())
        {
            QFile::copy(rInputfile, mInputfile);
            exp.fileName   = fileRelativeToProject(&prjs[i],mInputfile),projectPath(&prjs[i]);
            exp.fileFormat = ffCSV;
            prjs[i].setExp(exp);
        }
    }
    // Tag - graph
    else if (localName.toLower() == TAG_GRAPH)
    {
        if (exps.count()>0)
        {
            // Type of graph
            QString type = getPro(atts,PRO_TYPE);

            if (type == PRO_DIAGRAM_TABLE)
                exps.last().addDiaTab(diagramTable(getPro(atts,PRO_NAME),getProDouble(atts,PRO_POS_SLIDER),
                                                   getPro(atts,PRO_HORIZONTAL).toLower() == sTrue.toLower()));                
            else
                exps.last().addGraph(graph(type,getPro(atts,PRO_NAME),getPro(atts,PRO_XAXIS),getPro(atts,PRO_YAXIS)));
        }
    }
    // Tag - graph diagram
    else if (localName.toLower() == TAG_G_DIAGRAM)
    {
        QList<diagramTable> dt = exps.last().getDiaTabs();

        dt.last().setPicture(getPro(atts,PRO_PICTURE));
        exps.last().setDiaTabs(dt);
    }
    // Tag - graph diagram link
    else if (localName.toLower() == TAG_GD_LINK)
    {
        QList<diagramTable> dt = exps.last().getDiaTabs();

        dt.last().addDLink(link(getPro(atts,PRO_TYPE),getPro(atts,PRO_ID),getPro(atts,PRO_VARIABLE),getPro(atts,PRO_UNITS)));
        exps.last().setDiaTabs(dt);
    }
    // Tag -
    else if (localName.toLower() == TAG_G_TABLE)
    {
        QList<diagramTable> dt = exps.last().getDiaTabs();

        dt.last().setWidth_name( getProInt(atts,PRO_WIDTH_NAME));
        dt.last().setWidth_desc( getProInt(atts,PRO_WIDTH_DESC));
        dt.last().setWidth_value(getProInt(atts,PRO_WIDTH_VALUE));
        dt.last().setWidth_units(getProInt(atts,PRO_WIDTH_UNITS));
        exps.last().setDiaTabs(dt);
    }
    // Tag - graph table link
    else if (localName.toLower() == TAG_GT_LINK)
    {
        QList<diagramTable> dt = exps.last().getDiaTabs();

        dt.last().addTLink(link(PRO_TEXT,getPro(atts,PRO_NAME),getPro(atts,PRO_VARIABLE),getPro(atts,PRO_UNITS),getPro(atts,PRO_DESCRIPTION)));
        exps.last().setDiaTabs(dt);
    }
    // Tag - graph variable
    else if  (localName.toLower() == TAG_VARIABLE)
    {
        if (exps.count()>0 && exps.last().getGraphs().count()>0)
        {
            QList<graph> graphs = exps.last().getGraphs();

            val = getPro(atts,PRO_LINESTYLE);
            unsigned lstyle = val.isEmpty() ? 1 : val.toUInt();
            val = getPro(atts,PRO_YALIGN);
            QString yalign = val.isEmpty() ? algLeft : val;
            val = getPro(atts,PRO_IGNORED_VAL);
            bool ignored = !val.isEmpty() ? true : false;
            QString ignored_val = ignored ? val : sEmpty;
            QString x = getPro(atts,PRO_X);
            val = getPro(atts,PRO_BRUSHSTYLE);
            unsigned bstyle = val.isEmpty() ? 1 : val.toUInt();

            graphs.last().addVar(var(getPro(atts,PRO_NAME),getPro(atts,PRO_DESCRIPTION),lstyle,
                                     getPro(atts,PRO_LINECOLOR),bstyle,getPro(atts,PRO_BRUSHCOLOR),
                                     yalign,getPro(atts,PRO_YAXIS),ignored,ignored_val,x));
            exps.last().setGraphs(graphs);
        }
    }
    // Tag - parameter
    else if (localName.toLower() == TAG_PARAMETER)
    {
        // Get index
        int i = prjs.count()-1;

        // Get model
        opt_model mo = prjs[i].getModel();

        // Set parameter name and value
        val  = getPro(atts,PRO_NAME);
        val2 = getPro(atts,PRO_VALUE);
        if (!val.trimmed().isEmpty() && !val2.trimmed().isEmpty())
        {
            mo.p_name_new.append(val.trimmed());
            mo.p_value_new.append(val2.trimmed());
        }
        prjs[i].setModel(mo);
    }
    // Tag - integration
    else if (localName.toLower() == TAG_INTEGRATION)
    {
        // Get index
        int i = prjs.count()-1;

        // Get simulation
        opt_sim sim = prjs[i].getSim();

        // Set integration values
        val = getPro(atts,PRO_SOLVER);
        if (!val.isEmpty()) sim.integrator = (IntegratorType)val.toInt();

        val = getPro(atts,PRO_ABSTOL);
        if (!val.isEmpty()) sim.absTol = val.toDouble();

        val = getPro(atts,PRO_RELTOL);
        if (!val.isEmpty()) sim.relTol = val.toDouble();

        val = getPro(atts,PRO_ORDER);
        if (!val.isEmpty()) sim.order = val.toInt();

        val = getPro(atts,PRO_STEPSIZE);
        if (!val.isEmpty()) sim.stepSize = val.toDouble();

        // Set simulation in project
        prjs[i].setSim(sim);
    }
    // Tag - inputs
    else if (localName.toLower() == TAG_INPUTS)
    {
        // Get index
        int i = prjs.count()-1;

        // Get experiment
        opt_exp exp = prjs[i].getExp();

        // Set values for inputs
        val = getPro(atts,PRO_START);
        if (!val.isEmpty()) exp.startTime = val.toDouble();

        val = getPro(atts,PRO_STOP);
        if (!val.isEmpty()) exp.stopTime = val.toDouble();

        val = getPro(atts,PRO_REDUCTION);
        if (!val.isEmpty()) exp.ReducSam = val.toDouble();

        val = getPro(atts,PRO_TIMEKIND);
        if (!val.isEmpty()) exp.timeInType = val.toUInt();

        val = getPro(atts,PRO_INTERVALS);
        if (!val.isEmpty()) exp.nInInt = val.toInt();

        val = getPro(atts,PRO_TIMEVAR);
        if (!val.isEmpty()) exp.timeVar = val;

        prjs[i].setExp(exp);
    }
    // Tag - input
    else if (localName.toLower() == TAG_INPUT)
    {
        // Get index
        int i = prjs.count()-1;

        // Get experiment
        opt_exp exp = prjs[i].getExp();

        // Set info in experiment for the input
        exp.model_inputs.append(getPro(atts,PRO_NAME));
        exp.fmiType_inputs.append(FMI_REAL); // WARNING: only real inputs considered right now
        exp.type_inputs.append(getPro(atts,PRO_KIND).toUInt());
        exp.file_inputs.append(getPro(atts,PRO_VALUE));
        exp.fixed_inputs.append(getPro(atts,PRO_VALUE));

        // Set experiment
        prjs[i].setExp(exp);
    }
    // Tag - outputs
    else if (localName.toLower() == TAG_OUTPUTS)
    {
        // Get index
        int i = prjs.count()-1;

        // Get experiment
        opt_exp exp = prjs[i].getExp();

        // Set values
        val = getPro(atts,PRO_KIND);
        if (!val.isEmpty()) exp.timeOutType = val.toUInt();

        val = getPro(atts,PRO_NUMBER);
        if (!val.isEmpty()) exp.nOutInt = val.toInt();

        val = getPro(atts,PRO_STEP);
        if (!val.isEmpty()) exp.stepOutTime = val.toDouble();

        // Set experiment in project
        prjs[i].setExp(exp);
    }
    // Tag - geometry
    else if (localName.toLower() == TAG_GEOMETRY)
    {
        val = getPro(atts,PRO_HEIGHT);
        if (!val.isEmpty()) geo.setHeight(val.toInt());

        val = getPro(atts,PRO_WIDTH);
        if (!val.isEmpty()) geo.setWidth(val.toInt());

        val = getPro(atts,PRO_POS_HSLIDER);
        if (!val.isEmpty()) geo.setPos_hslider(val.toDouble());

        val = getPro(atts,PRO_POS_VSLIDER);
        if (!val.isEmpty()) geo.setPos_vslider(val.toDouble());
    }
    // Tag - columns
    else if (localName.toLower() == TAG_PARAMETERS)
    {
        val = getPro(atts,PRO_COLUMNS);
        if (!val.isEmpty())
        {
           QStringList columns = val.split(PRO_SEP);
           QStringList sizes;

           val = getPro(atts,PRO_SIZES);
           if (!val.isEmpty())
               sizes = val.split(PRO_SEP);

           for(int i=0;i<columns.count();i++)
           {
               column c;

               c.setName(columns[i]);
               c.setSize(sizes.count()>i ? sizes[i].toInt() : 0);
               cols.append(c);
           }
        }
    }
    // Tag resources
    else if (localName.toLower() == TAG_RESOURCES && !exps.isEmpty())
    {                        
        val = getPro(atts,PRO_POS_HSLIDER);
        if (!val.isEmpty()) exps.last().setPos_documents(val.toDouble());
        val = getPro(atts,PRO_WIDTH_NAME);
        if (!val.isEmpty()) exps.last().setWidth_name(val.toInt());
        val = getPro(atts,PRO_WIDTH_DOCUMENT);
        if (!val.isEmpty()) exps.last().setWidth_document(val.toInt());
    }
    // Tag resource
    else if (localName.toLower() == TAG_RESOURCE)
    {
        if (!exps.isEmpty())
            exps.last().addResource(resource(getPro(atts,PRO_TYPE),getPro(atts,PRO_NAME),getPro(atts,PRO_DOCUMENT),getPro(atts,PRO_LINK),getPro(atts,PRO_DESCRIPTION)));
    }
    // Tag diagram
    else if (localName.toLower() == TAG_DIAGRAM)
    {
        if (exps.count()>0)
        {
            experiment exp = exps.last();

            val = getPro(atts,PRO_POS_HSLIDER);
            if (!val.isEmpty()) exp.setPos_diagram(val.toDouble());
            val = getPro(atts,PRO_THUMB);
            if (!val.isEmpty()) exp.setThumb(val);
            val = getPro(atts,PRO_PICTURE);
            if (!val.isEmpty()) exp.setPicture(val);
            val = getPro(atts,PRO_DESCRIPTION);
            if (!val.isEmpty()) exp.setPicture_desc(val);

            exps.removeLast();
            exps.append(exp);
        }
    }
    // Tag link
    else if (localName.toLower() == TAG_LINK)
    {
        if (getPro(atts,PRO_TYPE) == LINK_ASSIGN)
        {
            experiment exp = exps.last();

            exp.addLink(link(getPro(atts,PRO_TYPE),getPro(atts,PRO_ID),getPro(atts,PRO_PROPERTY),
                             getPro(atts,PRO_VARIABLE),
                             getPro(atts,PRO_PRO_MIN).toDouble(),getPro(atts,PRO_PRO_MAX).toDouble(),
                             getPro(atts,PRO_VAR_MIN).toDouble(),getPro(atts,PRO_VAR_MAX).toDouble()));
            exps.removeLast();
            exps.append(exp);
        }
        else if (getPro(atts,PRO_TYPE) == LINK_TEXT)
        {
            experiment exp = exps.last();

            exp.addLink(link(getPro(atts,PRO_TYPE),getPro(atts,PRO_ID),getPro(atts,PRO_VARIABLE),getPro(atts,PRO_UNITS)));
            exps.removeLast();
            exps.append(exp);
        }
    }
    return true;
}

bool xmlSimIntParser(QString &error, QString path, XmlContentHandler &xmlData)
{
    QFile              xmlConFile(xmlConFileName);
    QXmlInputSource    source(&xmlConFile);
    QXmlSimpleReader   xmlReader;
    bool               ok;

    xmlData.setPath(path);
    xmlReader.setContentHandler(&xmlData);
    xmlReader.setErrorHandler(&xmlData);
    ok = xmlReader.parse(source);
    // Error parsing xml
    if (!ok)
    {
        error = xmlData.errorString();
        return ok;
    }
    // Set information from file
    return ok;
}

bool SVG_size(QDomDocument doc, int &width, int &height)
{
    QDomNodeList list = doc.elementsByTagName("svg");
    if(list.length() > 0)
    {
          QDomElement svgElement = list.item(0).toElement();
          QStringList parameters = svgElement.attribute("viewBox").split(" ");
          if (parameters.count()>=4)
          {
              width  = parameters.at(2).toInt();
              height = parameters.at(3).toInt();
              return true;
          }
    }
    return false;
}

QDomElement findElement(QDomNode node, QString id, QString value)
{
    QDomNode domNode = node.firstChild();
    QDomElement domElement;

    while(!(domNode.isNull()))
    {
        if(domNode.isElement())
        {
            domElement = domNode.toElement();
            if(!(domElement.isNull()))
            {
                if (domElement.attribute(id) == value) return domElement;
            }
        }
        domElement = findElement(domNode,id,value);
        if (domElement.attribute(id) == value) return domElement;
        domNode = domNode.nextSibling();
    }
    return QDomElement();
}

void changeSvgAttr(QDomDocument &doc, QString type, const QString id, QString property, QString value)
{
    Q_UNUSED(property);

    // Get element
    QDomElement del_old = findElement(doc,TAG_ID,id);

    // Copy element
    QDomElement del_new = del_old;

    // Type of change
    if (type == LINK_ASSIGN)
    {
        // Change attribute
        setSvgAttr(del_new, property, value);
    }
    else if (type == LINK_TEXT)
    {
        // Change text
        setSvgText(doc, del_new, value);
    }

    // Insert new element
    doc.insertBefore(del_new,del_old);

    // Delete old element
    doc.removeChild(del_old);
}

QIcon renderSVG(QDomDocument doc)
{
    // create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc.toByteArray(-1));

    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);

    // create painter to act over pixmap
    QPainter pixPainter(&pix);

    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);

    // Create icon
    QIcon icon = QIcon(pix);

    // return icon
    return icon;
}

void setSvgAttr(QDomElement &elem, QString strattr, QString strattrval)
{
    elem.setAttribute(strattr, strattrval);
    if(!elem.firstChildElement().isNull())
        elem.firstChildElement().setAttribute(strattr, strattrval);
}

void setSvgText(QDomDocument doc, QDomElement &elem, QString strattrval)
{
    Q_UNUSED(doc);

    // WARNING: fix this.
    if (!elem.firstChild().isNull())
    {
        elem.firstChild().setNodeValue(strattrval);
        if (!elem.firstChild().firstChild().isNull())
            elem.firstChild().firstChild().setNodeValue(strattrval);
    }
}

author::author(QString n, QString e, QString w, QString c, QString r)
{
    setName(n);
    setEmail(e);
    setWebsite(w);
    setCompany(c);
    setRole(r);
}

QString author::getName() const
{
    return name;
}

void author::setName(const QString &value)
{
    name = value;
}

QString author::getEmail() const
{
    return email;
}

void author::setEmail(const QString &value)
{
    email = value;
}

QString author::getWebsite() const
{
    return website;
}

void author::setWebsite(const QString &value)
{
    website = value;
}

QString author::getCompany() const
{
    return company;
}

void author::setCompany(const QString &value)
{
    company = value;
}

QString author::getRole() const
{
    return role;
}

void author::setRole(const QString &value)
{
    role = value;
}

QString app::getWindowName() const
{
    return windowName;
}

void app::setWindowName(const QString &value)
{
    windowName = value;
}

QString app::getTitleName() const
{
    return titleName;
}

void app::setTitleName(const QString &value)
{
    titleName = value;
}

QString app::getDescription() const
{
    return description;
}

void app::setDescription(const QString &value)
{
    description = value;
}

QString app::getVersion() const
{
    return version;
}

void app::setVersion(const QString &value)
{
    version = value;
}

QString app::getCompany() const
{
    return company;
}

void app::setCompany(const QString &value)
{
    company = value;
}

QString app::getLogo() const
{
    return logo;
}

void app::setLogo(const QString &value)
{
    logo = value;
}

QString app::getBackColor() const
{
    return backColor;
}

void app::setBackColor(const QString &value)
{
    backColor = value;
}

QString app::getTitleColor() const
{
    return titleColor;
}

void app::setTitleColor(const QString &value)
{
    titleColor = value;
}

QString app::getSectionColor() const
{
    return sectionColor;
}

void app::setSectionColor(const QString &value)
{
    sectionColor = value;
}

QString app::getFontTitleColor() const
{
    return fontTitleColor;
}

void app::setFontTitleColor(const QString &value)
{
    fontTitleColor = value;
}

QString app::getFontSectionColor() const
{
    return fontSectionColor;
}

void app::setFontSectionColor(const QString &value)
{
    fontSectionColor = value;
}

QString app::getIcon() const
{
    return icon;
}

void app::setIcon(const QString &value)
{
    icon = value;
}

bool app::getConf_integrator() const
{
    return conf_integrator;
}

void app::setConf_integrator(bool value)
{
    conf_integrator = value;
}

bool app::getConf_inputs() const
{
    return conf_inputs;
}

void app::setConf_inputs(bool value)
{
    conf_inputs = value;
}

bool app::getConf_outputs() const
{
    return conf_outputs;
}

void app::setConf_outputs(bool value)
{
    conf_outputs = value;
}

bool app::getSteady() const
{
    return steady;
}

void app::setSteady(bool value)
{
    steady = value;
}

QString app::getExperimentMenu() const
{
    return experimentMenu;
}

void app::setExperimentMenu(const QString &value)
{
    experimentMenu = value;
}

QString app::getExperimentTitle() const
{
    return experimentTitle;
}

void app::setExperimentTitle(const QString &value)
{
    experimentTitle = value;
}

bool app::getResults() const
{
    return results;
}

void app::setResults(bool value)
{
    results = value;
}

bool app::getReadStoredGeo() const
{
    return readStoredGeo;
}

void app::setReadStoredGeo(bool value)
{
    readStoredGeo = value;
}

app XmlContentHandler::getIapp() const
{
    return iapp;
}

QList<author> XmlContentHandler::getAuthors() const
{
    return authors;
}

QString XmlContentHandler::getPath() const
{
return path;
}

void XmlContentHandler::setPath(const QString &value)
{
path = value;
}

QList<opt_project> XmlContentHandler::getPrjs() const
{
return prjs;
}

void XmlContentHandler::setPrj(const opt_project prj, int i)
{
    if(i>=0 && i<prjs.count())
        prjs[i] = prj;
}

QList<experiment> XmlContentHandler::getExps() const
{
    return exps;
}

geometry XmlContentHandler::getGeo() const
{
    return geo;
}

QList<column> XmlContentHandler::getCols() const
{
    return cols;
}

QList<resource> experiment::getResources() const
{
    return resources;
}

void experiment::setResources(const QList<resource> &value)
{
    resources = value;
}

void experiment::addResource(const resource r)
{
    resources.append(r);
}

double experiment::getPos_documents() const
{
    return pos_documents;
}

void experiment::setPos_documents(double value)
{
    pos_documents = value;
}

int experiment::getWidth_name() const
{
    return width_name;
}

void experiment::setWidth_name(int value)
{
    width_name = value;
}

int experiment::getWidth_document() const
{
    return width_document;
}

void experiment::setWidth_document(int value)
{
    width_document = value;
}

QList<diagramTable> experiment::getDiaTabs() const
{
    return diaTabs;
}

void experiment::setDiaTabs(const QList<diagramTable> &value)
{
    diaTabs = value;
}

QString var::getName() const
{
    return name;
}

void var::setName(const QString &value)
{
    name = value;
}

QString var::getDescription() const
{
    return description;
}

void var::setDescription(const QString &value)
{
    description = value;
}

unsigned var::getLineStyle() const
{
    return lineStyle;
}

void var::setLineStyle(const unsigned &value)
{
    lineStyle = value;
}

QString var::getYalign() const
{
    return yalign;
}

void var::setYalign(const QString &value)
{
    yalign = value;
}

QString var::getYaxis() const
{
    return yaxis;
}

void var::setYaxis(const QString &value)
{
    yaxis = value;
}

QString var::getLineColor() const
{
    return lineColor;
}

void var::setLineColor(const QString &value)
{
    lineColor = value;
}

bool var::getIgnored() const
{
    return ignored;
}

void var::setIgnored(bool value)
{
    ignored = value;
}

QString var::getIgnored_val() const
{
    return ignored_val;
}

void var::setIgnored_val(const QString &value)
{
    ignored_val = value;
}

QString var::getX() const
{
    return x;
}

void var::setX(const QString &value)
{
    x = value;
}

unsigned var::getBrushStyle() const
{
    return brushStyle;
}

void var::setBrushStyle(const unsigned &value)
{
    brushStyle = value;
}

QString var::getBrushColor() const
{
    return brushColor;
}

void var::setBrushColor(const QString &value)
{
    brushColor = value;
}

QString graph::getName() const
{
    return name;
}

void graph::setName(const QString &value)
{
    name = value;
}

QString graph::getXaxis() const
{
    return xaxis;
}

void graph::setXaxis(const QString &value)
{
    xaxis = value;
}

QString graph::getYaxis() const
{
    return yaxis;
}

void graph::setYaxis(const QString &value)
{
    yaxis = value;
}

QList<var> graph::getVars() const
{
    return vars;
}

void graph::setVars(const QList<var> &value)
{
    vars = value;
}

void graph::addVar(const var value)
{
    vars.append(value);
}

QString graph::getType() const
{
    return type;
}

void graph::setType(const QString &value)
{
    type = value;
}

QString experiment::getName() const
{
    return name;
}

void experiment::setName(const QString &value)
{
    name = value;
}

QString experiment::getDescription() const
{
    return description;
}

void experiment::setDescription(const QString &value)
{
    description = value;
}

QList<graph> experiment::getGraphs() const
{
    return graphs;
}

void experiment::setGraphs(const QList<graph> &value)
{
    graphs = value;
}

void experiment::addGraph(const graph value)
{
    graphs.append(value);
}

void experiment::addDiaTab(const diagramTable value)
{
    diaTabs.append(value);
}

int geometry::getHeight() const
{
    return height;
}

int geometry::getWidth() const
{
    return width;
}

void geometry::setWidth(int value)
{
    width = value;
}

double geometry::getPos_vslider() const
{
    return pos_vslider;
}

double geometry::getPos_hslider() const
{
    return pos_hslider;
}

void geometry::setPos_hslider(double value)
{
    pos_hslider = value;
}

void geometry::setHeight(int value)
{
    height = value;
}

void geometry::setPos_vslider(double value)
{
    pos_vslider = value;
}

double experiment::getPos_diagram() const
{
    return pos_diagram;
}

void experiment::setPos_diagram(double value)
{
    pos_diagram = value;
}

QString experiment::getThumb() const
{
    return thumb;
}

void experiment::setThumb(const QString &value)
{
    thumb = value;
}

QString experiment::getPicture() const
{
    return picture;
}

void experiment::setPicture(const QString &value)
{
    picture = value;
}

QString experiment::getPicture_desc() const
{
    return picture_desc;
}

void experiment::setPicture_desc(const QString &value)
{
    picture_desc = value;
}

QList<link> experiment::getLinks() const
{
    return links;
}

void experiment::setLinks(const QList<link> &value)
{
    links = value;
}

void experiment::addLink(const link value)
{
    links.append(value);
}

void diagramTable::addDLink(const link value)
{
    dLinks.append(value);
}

void diagramTable::addTLink(const link value)
{
    tLinks.append(value);
}

double diagramTable::getPos_hslider() const
{
    return pos_hslider;
}

void diagramTable::setPos_hslider(double value)
{
    pos_hslider = value;
}

int diagramTable::getWidth_name() const
{
    return width_name;
}

void diagramTable::setWidth_name(int value)
{
    width_name = value;
}

int diagramTable::getWidth_desc() const
{
    return width_desc;
}

void diagramTable::setWidth_desc(int value)
{
    width_desc = value;
}

int diagramTable::getWidth_value() const
{
    return width_value;
}

void diagramTable::setWidth_value(int value)
{
    width_value = value;
}

int diagramTable::getWidth_units() const
{
    return width_units;
}

void diagramTable::setWidth_units(int value)
{
    width_units = value;
}

bool diagramTable::getHorizontal() const
{
    return horizontal;
}

void diagramTable::setHorizontal(bool value)
{
    horizontal = value;
}

QString experiment::getGroup() const
{
    return group;
}

void experiment::setGroup(const QString &value)
{
    group = value;
}

QString column::getName() const
{
    return name;
}

void column::setName(const QString &value)
{
    name = value;
}

int column::getSize() const
{
    return size;
}

void column::setSize(const int &value)
{
    size = value;
}

QString resource::getType() const
{
    return type;
}

void resource::setType(const QString &value)
{
    type = value;
}

QString resource::getName() const
{
    return name;
}

void resource::setName(const QString &value)
{
    name = value;
}

QString resource::getDocument() const
{
    return document;
}

void resource::setDocument(const QString &value)
{
    document = value;
}

QString resource::getDescription() const
{
    return description;
}

void resource::setDescription(const QString &value)
{
    description = value;
}

QString resource::getLink() const
{
    return link;
}

void resource::setLink(const QString &value)
{
    link = value;
}

QString link::getType() const
{
    return type;
}

void link::setType(const QString &value)
{
    type = value;
}

QString link::getId() const
{
    return id;
}

void link::setId(const QString &value)
{
    id = value;
}

QString link::getProperty() const
{
    return property;
}

void link::setProperty(const QString &value)
{
    property = value;
}

QString link::getVariable() const
{
    return variable;
}

void link::setVariable(const QString &value)
{
    variable = value;
}

double link::getPro_min() const
{
    return pro_min;
}

void link::setPro_min(double value)
{
    pro_min = value;
}

double link::getPro_max() const
{
    return pro_max;
}

void link::setPro_max(double value)
{
    pro_max = value;
}

double link::getVar_min() const
{
    return var_min;
}

void link::setVar_min(double value)
{
    var_min = value;
}

double link::getVar_max() const
{
    return var_max;
}

void link::setVar_max(double value)
{
    var_max = value;
}

QString link::getUnits() const
{
    return units;
}

void link::setUnits(const QString &value)
{
    units = value;
}

QString link::getDesc() const
{
    return desc;
}

void link::setDesc(const QString &value)
{
    desc = value;
}

QString diagramTable::getName() const
{
    return name;
}

void diagramTable::setName(const QString &value)
{
    name = value;
}

QString diagramTable::getPicture() const
{
    return picture;
}

void diagramTable::setPicture(const QString &value)
{
    picture = value;
}

QList<link> diagramTable::getDLinks() const
{
    return dLinks;
}

void diagramTable::setDLinks(const QList<link> &value)
{
    dLinks = value;
}

QList<link> diagramTable::getTLinks() const
{
    return tLinks;
}

void diagramTable::setTLinks(const QList<link> &value)
{
    tLinks = value;
}

void domDocToSVG(QDomDocument doc, QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &file );
        stream << doc.toString();
        file.close();
    }
}

void domDocToPDF(QDomDocument doc, QString filename, int widthMM, int heightMM)
{
    QSvgRenderer svg(doc.toByteArray(-1));
    QPainter *painter = new QPainter();
    QPrinter *printer = new QPrinter();

    printer->setOutputFileName(filename);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPaperSize(QSize(heightMM, widthMM),QPrinter::Millimeter);
    printer->setFontEmbeddingEnabled(true);
    printer->setCreator("Simu PDF file");
    printer->setDocName("Diagram");
    printer->setOutputFormat(QPrinter::PdfFormat);
    painter->begin(printer);
    svg.render(painter);
    painter->end();

    delete(painter);
    delete(printer);
}

bool XML_to_QDom(QString filename, QDomDocument &doc)
{
    if (!filename.isEmpty())
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        // Open svg resource load contents to qbytearray
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        const QByteArray &baData = file.readAll();

        // Load svg contents to xml document
        if (!doc.setContent(baData,true,&errorStr, &errorLine,&errorColumn))
        {
            //qDebug() << errorStr << errorLine << errorColumn;
            return false;
        }
        return true;
    }
    return false;
}

void SVG_setSize(QSvgWidget *view, double &aspect_ratio, int w, int h)
{
    int x0;
    int y0;
    int new_h;
    int new_w;

    if (aspect_ratio > 0 && w/aspect_ratio<=h)
    {
        new_w = w;
        new_h = new_w/aspect_ratio;
    }
    else
    {
        new_h = h;
        new_w = new_h*aspect_ratio;
    }

    x0 = 0.5*(w - new_w);
    y0 = 0.5*(h - new_h);
    view->move(x0,y0);
    view->resize(new_w,new_h);

    //qDebug() << w << h << new_w << new_h;
}

QImage getImage(QSvgWidget *view)
{
    QImage image(view->width(), view->height(), QImage::Format_RGB32);
    QPainter painter;
    painter.begin(&image);
    view->render(&painter);
    painter.end();

    return image;
}

void saveSVGimage(QWidget *w, QDomDocument doc, QSvgWidget *view)
{
    QFileDialog dlg(w, mImagFile, sEmpty, PDFFilter + ";;" + SVGFilter + ";;" + PNGFilter + ";;" + BMPFilter + ";;" + JPGFilter + ";;" + JPEGFilter);

    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setOption(QFileDialog::DontConfirmOverwrite,false);

    if (dlg.exec())
    {
        if (dlg.selectedNameFilter() == PDFFilter)       domDocToPDF(doc,dlg.selectedFiles()[0],view->widthMM(),view->heightMM());
        else if (dlg.selectedNameFilter() == SVGFilter)  domDocToSVG(doc,dlg.selectedFiles()[0]);
        else                                             imageToImg(getImage(view),false,dlg.selectedFiles()[0]);
    }
}

void SVG_refresh(QDomDocument doc, QSvgWidget *view, int width, int height, double ar)
{
    // Load
    view->load(doc.toByteArray(-1)); // For proper layout -1
    // Set background color - white
    QPalette pal = view->palette();
    pal.setColor(QPalette::Background,Qt::white);
    view->setPalette(pal);
    // Resize
    SVG_setSize(view,ar,width,height);
}

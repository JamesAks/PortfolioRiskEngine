#include "DateValueGraph.hpp"
#include "TimeSeries.hpp"

#include <qstring.h>
#include <qcolor.h>
#include <QtGraphs/qgraphstheme.h>
#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qdatetimeaxis.h>
#include <QtGraphs/qvalueaxis.h>
#include <QtQuickWidgets/qquickwidget.h>

#include <chrono>



DateValueGraph::DateValueGraph(QWidget* parent) {

	quick_widget = new QQuickWidget();
	line_series = new QLineSeries();

	x_axis = new QDateTimeAxis();
	y_axis = new QValueAxis();

	initialiseGraph();
}


DateValueGraph::~DateValueGraph() {

    delete quick_widget;
    delete line_series;
    delete x_axis;
    delete y_axis;
}


void DateValueGraph::initialiseGraph() {

    x_axis->setLabelFormat("MMM yy");

    auto theme = new QGraphsTheme(quick_widget);
    theme->setBackgroundColor(qRgb(22, 33, 62));
    theme->setPlotAreaBackgroundColor(qRgb(11, 16, 32));
    theme->setLabelBorderVisible(true);
    theme->setLabelBackgroundVisible(true);
    theme->setGridVisible(false);

    x_axis->setTitleText("Volatility");
    y_axis->setTitleText("Return");
    x_axis->setTitleFont(QFont("Segoe UI", 10, 700));
    y_axis->setTitleFont(QFont("Segoe UI", 10, 700));
    x_axis->setTitleColor(qRgb(212, 175, 55));
    y_axis->setTitleColor(qRgb(212, 175, 55));

    quick_widget->setInitialProperties(
        {
            {"theme", QVariant::fromValue(theme)},
            {"axisX", QVariant::fromValue(x_axis)},
            {"axisY", QVariant::fromValue(y_axis)},
            {"seriesList", QVariant::fromValue(QList<QAbstractSeries*>({line_series})) }
        }
    );

    quick_widget->setResizeMode(
        QQuickWidget::SizeRootObjectToView
    );

    quick_widget->loadFromModule("QtGraphs", "GraphsView");
}


void DateValueGraph::updateGraph(const std::vector<std::chrono::year_month_day>& dates, const std::vector<double> returns) {

    // Clear the current graph.
    line_series->clear();

    // Set Axis scales.
    auto min_return = *std::min_element(returns.begin(),returns.end());
    auto max_return = *std::max_element(returns.begin(), returns.end());

    //QDate min_date{ dates[1] };
    //QDate max_date{ dates[returns.size() - 1] };

    QDate max_date{ dates[returns.size() - 1] };

    QDateTime qmin_date{ QDate{ dates[1] }, QTime(0, 0), Qt::UTC };
    QDateTime qmax_date{ QDate{ max_date.year(),max_date.month() + 1, 1 }, QTime(0, 0), Qt::UTC };


    //QDateTime qmin_date{ min_date.addMonths(-1), QTime(0, 0), Qt::UTC};
    //QDateTime qmax_date{ max_date.addDays(3), QTime(0, 0), Qt::UTC };

    x_axis->setMin(qmin_date);
    x_axis->setMax(qmax_date);

    y_axis->setMin(min_return - (0.1 * (max_return - min_return)));
    y_axis->setMax(max_return + (0.1 * (max_return - min_return)));

    // Place points on the graph.
    for (int i = 0; i < returns.size(); i++) {

        QDate current_date{ dates[i+1] };
        QDateTime qcurrent_date{ current_date, QTime(0, 0), Qt::UTC };

        line_series->append(qcurrent_date.toMSecsSinceEpoch(),returns[i]);
    }      
}


void DateValueGraph::setXTickInterval(size_t count){ x_axis->setTickInterval(count); }


void DateValueGraph::setYTickInterval(size_t count){ y_axis->setTickInterval(count); }


void DateValueGraph::setXSubTickCount(size_t count){ x_axis->setSubTickCount(count); }


void DateValueGraph::setYSubTickCount(size_t count){ y_axis->setSubTickCount(count); }


QQuickWidget* DateValueGraph::graph() const { return quick_widget; }


QLineSeries* DateValueGraph::lineSeries() const { return line_series; }
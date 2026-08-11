#include "EfficientFrontierGraph.hpp"

#include "EfficientFrontier.hpp"
#include "Portfolio.hpp"
#include "RiskEngine.hpp"

#include <qstring.h>
#include <QtQuickWidgets/qquickwidget.h>
#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qscatterseries.h>
#include <QtGraphs/qvalueaxis.h>
#include <QtGraphs/qgraphstheme.h>
#include <qcolor.h>

#include <algorithm>


EfficientFrontierGraph::EfficientFrontierGraph(QWidget* parent){

	quick_widget = new QQuickWidget();
	scatter_series = new QScatterSeries();
	line_series = new QLineSeries();

    x_axis = new QValueAxis();
    y_axis = new QValueAxis();

	initialiseGraph();
}


void EfficientFrontierGraph::initialiseGraph(){

    auto theme = new QGraphsTheme(quick_widget);
    theme->setBackgroundColor(qRgb(22, 33, 62));
    theme->setPlotAreaBackgroundColor(qRgb(11, 16, 32));
    theme->setLabelBorderVisible(true);
    theme->setLabelBackgroundVisible(true);
    theme->setGridVisible(false);


    x_axis->setTitleText("Volatility");
    y_axis->setTitleText("Return");
    x_axis->setTitleFont(QFont("Segoe UI", 10,700));
    y_axis->setTitleFont(QFont("Segoe UI", 10,700));
    x_axis->setTitleColor(qRgb(212, 175, 55));
    y_axis->setTitleColor(qRgb(212, 175, 55));

    quick_widget->setInitialProperties(
        {
            {"theme", QVariant::fromValue(theme)},
            {"axisX", QVariant::fromValue(x_axis)},
            {"axisY", QVariant::fromValue(y_axis)},
            {"seriesList", QVariant::fromValue(QList<QAbstractSeries*>({scatter_series, line_series})) }
        }
    );

    quick_widget->setResizeMode(
        QQuickWidget::SizeRootObjectToView
    );

    quick_widget->loadFromModule("QtGraphs", "GraphsView");
}



EfficientFrontierGraph::~EfficientFrontierGraph() {

    delete quick_widget;
	delete scatter_series;
	delete line_series;
    delete x_axis;
    delete y_axis;
}

QQuickWidget* EfficientFrontierGraph::graph() const { return quick_widget; }


QScatterSeries* EfficientFrontierGraph::scatterSeries() const { return scatter_series; }


QLineSeries* EfficientFrontierGraph::lineSeries() const { return line_series; }


void EfficientFrontierGraph::updateGraph(const EfficientFrontier& efficient_frontier) {

    // Clear the current graph.
    line_series->clear();
    scatter_series->clear();

    // Set Axis scales.
    auto min_return = efficient_frontier.minimumReturn();
    auto max_return = efficient_frontier.maximumReturn();
    auto min_volatility = efficient_frontier.minimumVolatility();
    auto max_volatility = efficient_frontier.maximumVolatility();

    x_axis->setMin(min_volatility - (0.3 * (max_volatility - min_volatility)));
    x_axis->setMax(max_volatility + (0.3 * (max_volatility - min_volatility)));

    y_axis->setMin(min_return - (0.3 * (max_return - min_return)));
    y_axis->setMax(max_return + (0.3 * (max_return - min_return)));

    // Place points on the graph.
    size_t count = 0;
    for (const auto& point : efficient_frontier.data()) {

        count++;

        line_series->append(point.volatitity, point.expected_return);

        // Only place a scatter graph point on every 5 points.
        if (count % 5 == 0 || count == 1) { scatter_series->append(point.volatitity, point.expected_return); }
    }


}


void EfficientFrontierGraph::test() const {

    line_series->clear();
    scatter_series->clear();

    scatter_series->append(5, 5);

    x_axis->setMax(5);
    y_axis->setMin(5);
}

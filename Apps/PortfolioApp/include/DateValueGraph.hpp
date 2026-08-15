#include <qobject.h>


class TimeSeries;
class QDateTimeAxis;
class QValueAxis;
class QQuickWidget;
class QScatterSeries;
class QLineSeries;

class DateValueGraph : public QObject{

	// Class for visualising date-to-value series as a graph

	Q_OBJECT

	private:

		QQuickWidget* quick_widget;
		QLineSeries* line_series;

		QDateTimeAxis* x_axis;
		QValueAxis* y_axis;

		void initialiseGraph();

	public:

		explicit DateValueGraph(QWidget* parent = nullptr);
		~DateValueGraph();

		void updateGraph(const std::vector<std::chrono::year_month_day>&, const std::vector<double>);
		
		void setXTickInterval(size_t);
		void setYTickInterval(size_t);
		void setXSubTickCount(size_t);
		void setYSubTickCount(size_t);

		QQuickWidget* graph() const;

		QLineSeries* lineSeries() const;
};
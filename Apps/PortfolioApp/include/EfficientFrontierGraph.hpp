#include <qobject.h>
#include <memory>


struct EfficientFrontierPoint;
class EfficientFrontier;
class QQuickWidget;
class QValueAxis;
class QQickWidget;
class QScatterSeries;
class QLineSeries;

class EfficientFrontierGraph : public QObject {

	Q_OBJECT

	private:

		QQuickWidget* quick_widget;
		QScatterSeries* scatter_series;
		QLineSeries* line_series;

		QValueAxis* x_axis;
		QValueAxis* y_axis;

		void initialiseGraph();

	public:

		explicit EfficientFrontierGraph(QWidget* parent = nullptr);
		~EfficientFrontierGraph();

		QQuickWidget* graph() const;
		QScatterSeries* scatterSeries() const;
		QLineSeries* lineSeries() const;

		void updateGraph(const EfficientFrontier&);

		void test() const;


	private slots:
};

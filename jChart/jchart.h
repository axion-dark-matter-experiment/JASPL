#ifndef JCHART_H
#define JCHART_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QSplineSeries>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

class JChart : QMainWindow {

    Q_OBJECT

  public:
    explicit JChart( QWidget *parent = 0 );
    ~JChart();

    template <class T> void Plot( T time_series );

  private:

    QLineSeries* time_series;
    QValueAxis* x_axis;
    QValueAxis* y_axis;
    QChartView* chartView;
    QTimer* auto_timer;
    QChart *chart;

    void UnitTest();

  private slots:
    void UpdateSignal();

  signals:
    void SignalChanged();

};

#include "jChart/jchart_templates.tpp"

#endif // JCHART_H

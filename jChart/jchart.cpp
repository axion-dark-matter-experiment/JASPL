#include "jchart.h"

static unsigned long x=123456789, y=362436069, z=521288629;

unsigned long xorshf96(void) {          //period 2^96-1
unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

   t = x;
   x = y;
   y = z;
   z = t ^ x ^ y;

  return z;
}

void JChart::UnitTest() {

    std::vector<int> time_series2;

    for (int i =0; i < 500; i++) {
        int b = xorshf96();
        b = b % 20 + 1;
        time_series2.push_back(b);
    }

    Plot( time_series2 );
}

JChart::JChart(QWidget *parent) : QMainWindow( parent ) {

    time_series = new QLineSeries();
    x_axis = new QValueAxis;
    y_axis = new QValueAxis;
    chart = new QChart();

    chart->addSeries(time_series);
    chart->legend()->hide();
    chart->setTitle("Time Series");

    chart->createDefaultAxes();
    chart->setAxisX(x_axis, time_series);
    chart->setAxisY(y_axis, time_series);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

//    auto_timer = new QTimer(this);
//    QObject::connect(auto_timer, SIGNAL(timeout()), this, SLOT(UpdateSignal()));
//    auto_timer->setInterval(50);
//    auto_timer->start();

//    connect(auto_timer, SIGNAL(timeout()), this, SLOT(UpdateSignal()));
//    auto_timer->start(0);

    setCentralWidget(chartView);
    resize(1280, 720);
    show();

}

void JChart::UpdateSignal() {
    UnitTest();
}

JChart::~JChart() {

}

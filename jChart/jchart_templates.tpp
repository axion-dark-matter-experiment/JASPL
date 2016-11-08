template <class T>
void JChart::Plot( T input ) {

    uint N = input.size();

    time_series->clear();

    for ( uint i = 0; i < N ; i ++ ) {
        time_series->append(i, input[i]);
    }

    auto min = std::min_element(input.begin(), input.end());
    auto max = std::max_element(input.begin(), input.end());

    chart->axisX()->setRange(0, N);
    chart->axisY()->setRange(*min, *max);
}

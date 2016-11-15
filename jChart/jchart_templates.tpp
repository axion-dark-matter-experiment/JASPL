template <class T>
void JChart::Plot( T input ) {

    uint N = input.size();

    time_series->clear();

    uint k = static_cast<uint>( ceil( static_cast<double>(N) / 1000.0f ) );
    std::cout << k << std::endl;

    for ( uint i = 0; i < N - k ; i += k ) {
        time_series->append(i, input[i]);
    }

    auto min = std::min_element(input.begin(), input.end());
    auto max = std::max_element(input.begin(), input.end());

    chart->axisX()->setRange(0, N);
    chart->axisY()->setRange(*min, *max);
}

template <class T>
void JChart::Plot( T input, std::string chart_title ) {

    Plot( input );
    chart->setTitle( QString::fromStdString( chart_title ) );
}

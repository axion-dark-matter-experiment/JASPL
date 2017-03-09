template < typename T >
RecurseMean<T>::RecurseMean(uint num_samples) {
    static_assert(is_stdlib_container< T >::value,
                  "Recurse mean can only operate on container-like objects.");
    static_assert(std::is_floating_point< typename T::value_type >::value,
                  "Recurse mean can only accept floating-point types");

    index = static_cast< typename T::value_type >( 0 );

    last = T(num_samples, static_cast< typename T::value_type >(0.0));
}

template < typename T >
void RecurseMean<T>::operator()(const T &next_value) {

    if ( last.size() != next_value.size() ) {
        std::cout << "________________________________________________" << std::endl;
        std::cout << "Sample size did not match averaging engine size" << std::endl;
        std::cout << "Sample Size: " << last.size() << " Input Size: " << next_value.size() << std::endl;
        std::cout << "________________________________________________" << std::endl;
        return;
    }

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    index += 1.0f;

    for ( uint i = 0; i < next_value.size() ; i++ ) {
        last[i] = ((index - 1.0f) / index) * last[i] + next_value[i] / index;
    }

    std::cout << __CLASS__ << "index is currently: " << index << std::endl;
}

template < typename T >
typename T::value_type RecurseMean<T>::Index() {
    boost::shared_lock<boost::shared_mutex> lock( monitor );
    return index;
}

template < typename T >
void RecurseMean<T>::Reset() {

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    last = T(last.size() , static_cast< typename T::value_type >(0.0f));
    index = 0.0f;

}

template < typename T >
T RecurseMean<T>::ReturnValue() {

    boost::shared_lock<boost::shared_mutex> lock( monitor );
    return last;

}

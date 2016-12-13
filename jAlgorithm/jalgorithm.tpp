template < typename T >
RecurseMean<T>::RecurseMean( T initial_value ) {
    static_assert( is_stdlib_container< T >::value, "Recurse mean can only operate on container-like objects." );
    static_assert( std::is_arithmetic< typename T::value_type >::value, "Recurse mean can only accept arithmetic types");

    last = initial_value;
}

template < typename T >
RecurseMean<T>::RecurseMean( T& initial_value ) {
    static_assert( is_stdlib_container< T >::value, "Recurse mean can only operate on container-like objects." );
    static_assert( std::is_arithmetic< typename T::value_type >::value, "Recurse mean can only accept arithmetic types");

    last = initial_value;
}

template < typename T >
void RecurseMean<T>::operator()( T  next_value ) {

    index += 1.0f;

    for( uint i = 0; i < next_value.size() ; i++ ) {
        last.at(i) = ( ( index - 1.0f )/ index )*last.at(i) + next_value.at(i) / index;
    }
}

template < typename T >
void RecurseMean<T>::operator()( T&  next_value ) {

    index += 1.0f;

    for( uint i = 0; i < next_value.size() ; i++ ) {
        last.at(i) = ( ( index - 1.0f )/ index )*last.at(i) + next_value.at(i) / index;
    }
}

template < typename T >
T RecurseMean<T>::ReturnValue() {
    return last;
}

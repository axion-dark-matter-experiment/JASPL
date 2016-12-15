template < typename T >
RecurseMean<T>::RecurseMean(uint num_samples) {
  static_assert(is_stdlib_container< T >::value,
                "Recurse mean can only operate on container-like objects.");
  static_assert(std::is_floating_point< typename T::value_type >::value,
                "Recurse mean can only accept floating-point types");

  last = T(num_samples, static_cast< typename T::value_type >(0.0f));
}

template < typename T >
void RecurseMean<T>::operator()(const T &next_value) {

  index += 1.0f;

  for (uint i = 0; i < next_value.size() ; i++) {
    last[i] = ((index - 1.0f) / index) * last[i] + next_value[i] / index;
  }
}

template < typename T >
void RecurseMean<T>::Reset() {

  last = T(last.size() , static_cast< typename T::value_type >(0.0f));
  index = 0.0f;

}

template < typename T >
T RecurseMean<T>::ReturnValue() {
  return last;
}

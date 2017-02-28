#ifndef OUROBOROS_H
#define OUROBOROS_H

// C System-Headers
//
// C++ System headers
#include <mutex>
#include <vector>
// Boost Headers
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/lexical_cast.hpp>
// Project Specific Headers
#include "../Debug/debug.h"
#include "../jTypeTraits/jtypetraits.h"

namespace jaspl {

template < typename T >
class ouroborus {

  public:
    ouroborus();
    ouroborus( uint buffer_size );

    template < typename F >
    friend std::ostream& operator<< (std::ostream& stream, ouroborus<F>& buffer);

    void HeadInsert( T* to_insert, uint insert_size );
    void TailInsert( T* to_insert, uint insert_size );

    std::vector<T> HeadRead( uint read_size );
    template < typename F >
    std::vector< F > HeadReadAndConvert( uint read_size );
    bool CheckHead( uint read_size );

    std::vector<T> TailRead( uint read_size );
    template < typename F >
    std::vector< F > TailReadAndConvert( uint read_size );
    bool CheckTail( uint read_size );

    void Reset();

    uint capacity();
    uint size();

    bool overrun();

  private:

    void update_counter( int update_value );

    boost::circular_buffer<T> internal_buffer;
    boost::shared_mutex monitor;

    bool overrun_flag = false;

};

template < typename T >
ouroborus<T>::ouroborus() {
    //
}

template < typename T>
ouroborus<T>::ouroborus( uint buffer_size ) : internal_buffer( buffer_size ) {
    DEBUG_PRINT( "Built new ring_buffer of size " << buffer_size );
}

template < typename T >
std::ostream& operator << (std::ostream& stream, ouroborus<T>& buffer) {

    stream << "Maximum buffer size: " << buffer.internal_buffer.capacity() << std::endl;
    stream << "Number of elements in buffer: " << buffer.internal_buffer.size() << std::endl;
    stream << "Front of buffer: " << buffer.internal_buffer.front() << std::endl;
    stream << "Back of buffer: " << buffer.internal_buffer.back() << std::endl;
    stream << "Buffer contents: " << std::endl;

    for( const auto& elem : buffer.internal_buffer ) {
        stream << elem << ",";
    }

    stream << std::endl;

    return stream;
}

template < typename T >
bool ouroborus<T>::overrun() {
    boost::shared_lock<boost::shared_mutex> lock( monitor );
    return overrun_flag;
}

template < typename T >
uint ouroborus<T>::capacity() {
    boost::shared_lock<boost::shared_mutex> lock( monitor );
    return internal_buffer.capacity();
}

template < typename T >
uint ouroborus<T>::size() {
    boost::shared_lock<boost::shared_mutex> lock( monitor );
    return internal_buffer.size();
}

template < typename T >
void ouroborus<T>::Reset() {
    boost::shared_lock<boost::shared_mutex> lock( monitor );

    internal_buffer.clear();
    overrun_flag = false;
}

template < typename T >
void ouroborus<T>::HeadInsert( T* to_insert, uint insert_size ) {

    DEBUG_PRINT( "Inserting " << insert_size << " elements into ring buffer" );

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    auto head = to_insert;
    auto tail = head + insert_size;

    if( (internal_buffer.size() + insert_size) >= internal_buffer.capacity() ) {
        overrun_flag = true;
    }

    internal_buffer.insert( internal_buffer.begin(), head, tail );

}

template < typename T >
void ouroborus<T>::TailInsert( T* to_insert, uint insert_size ) {

    DEBUG_PRINT( "Inserting " << insert_size << " elements into ring buffer" );

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    auto head = to_insert;
    auto tail = head + insert_size;

    if( (internal_buffer.size() + insert_size) >= internal_buffer.capacity() ) {
        overrun_flag = true;
    }

    internal_buffer.insert( internal_buffer.end(), head, tail );

}

template < typename T >
bool ouroborus<T>::CheckHead( uint data_size ) {

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    auto first = internal_buffer.begin();
    auto last = internal_buffer.begin() + data_size;

    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < data_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );
    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    return !check_condition;
}

template < typename T >
bool ouroborus<T>::CheckTail( uint data_size ) {

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    auto first = internal_buffer.end() - data_size;
    auto last = internal_buffer.end() ;

    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < data_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );
    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    return !check_condition;
}

template < typename T >
std::vector<T> ouroborus<T>::TailRead( uint read_size ) {

    // get shared access
    boost::shared_lock<boost::shared_mutex> lock( monitor );

    auto first = internal_buffer.end() - read_size;
    auto last = internal_buffer.end() ;
    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < read_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );

        std::string err_str = "Not enough new data in ring buffer: Have ";
        err_str += boost::lexical_cast<std::string>( internal_buffer.size() );
        err_str += " new data samples, but  ";
        err_str += boost::lexical_cast<std::string>( read_size );
        err_str += " were requested.";
        throw std::ios_base::failure(err_str);

    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    auto copy_vec = std::vector< T >( first, last );

    internal_buffer.erase ( first, last );

    return copy_vec;
}

template < typename T >
template < typename F >
std::vector< F > ouroborus< T >::TailReadAndConvert( uint read_size ) {

    // get shared access
    boost::shared_lock<boost::shared_mutex> lock( monitor );

    auto first = internal_buffer.end() - read_size;
    auto last = internal_buffer.end() ;
    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < read_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );

        std::string err_str = "Not enough new data in ring buffer: Have ";
        err_str += boost::lexical_cast<std::string>( internal_buffer.size() );
        err_str += " new data samples, but  ";
        err_str += boost::lexical_cast<std::string>( read_size );
        err_str += " were requested.";
        throw std::ios_base::failure(err_str);

    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    auto copy_vec = std::vector< F >( first, last );

    internal_buffer.erase ( first, last );

    return copy_vec;
}

template < typename T >
std::vector<T> ouroborus<T>::HeadRead( uint read_size ) {

    // get shared access
    boost::shared_lock<boost::shared_mutex> lock( monitor );

    auto first = internal_buffer.begin() + 0;
    auto last = internal_buffer.begin() + read_size;

    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < read_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );

        std::string err_str = "Not enough new data in ring buffer: Have ";
        err_str += boost::lexical_cast<std::string>( internal_buffer.size() );
        err_str += " new data samples, but  ";
        err_str += boost::lexical_cast<std::string>( read_size );
        err_str += " were requested.";
        throw std::ios_base::failure(err_str);
    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    auto copy_vec = std::vector< T >( first, last );

    internal_buffer.erase ( first, last );

    return copy_vec;
}

template < typename T >
template < typename F >
std::vector< F > ouroborus<T>::HeadReadAndConvert ( uint read_size ) {

    // get shared access
    boost::shared_lock<boost::shared_mutex> lock( monitor );

    auto first = internal_buffer.begin() + 0;
    auto last = internal_buffer.begin() + read_size;

    //Attempt to read more samples than are current stored in
    //the ring buffer
    bool check_condition = (std::distance(first, last) < read_size);

    if (check_condition) {
        DEBUG_PRINT( __func__ << " Unable to read, not enough samples in ring buffer" );

        std::string err_str = "Not enough new data in ring buffer: Have ";
        err_str += boost::lexical_cast<std::string>( internal_buffer.size());
        err_str += " new data samples, but  ";
        err_str += boost::lexical_cast<std::string>( read_size );
        err_str += " were requested.";
        throw std::ios_base::failure(err_str);
    } else {
        DEBUG_PRINT( __func__ << " Able to read." );
    }

    auto copy_vec = std::vector< F >( first, last );

    internal_buffer.erase ( first, last );

    return copy_vec;
}


}

#endif // OUROBOROS_H

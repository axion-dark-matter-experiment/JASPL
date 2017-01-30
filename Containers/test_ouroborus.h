#ifndef TEST_OUROBORUS_H
#define TEST_OUROBORUS_H

// C System-Headers
//
// C++ System headers
#include <iostream>
// Boost Headers
//
// Project Specific Headers
#include "ouroboros.h"

namespace jaspl {

void test_ouroboros() {

    std::vector<int> test_data { 1, 2, 3, 4, 5, 6 };
    std::vector<int> test_data_2 { 0, 0, 0, 0 };

    ouroborus<int> tester( 10 );
    tester.TailInsert( test_data.data(), test_data.size() );
    tester.TailInsert( test_data.data(), test_data.size() );
    tester.TailInsert( test_data_2.data(), test_data_2.size() );

    auto tail_read_test = tester.TailRead( 3 );

    for ( const auto& elem: tail_read_test ) {
        std::cout << elem << ",";
    }
    std::cout << std::endl;

    std::cout << tester << std::endl;

}

}

#endif // TEST_OUROBORUS_H

#include <iostream>
#include "jvector.h"

int main() {
    std::string test = "df\nagh\n2\n1\n2\n1\n2";
    auto j_vect = jaspl::JVector<double> (test);

    std::vector<double> test_2 = std::vector<double> {1,2,3,4};
    auto j_vect_2 = jaspl::JVector<double> ( test_2 );

    auto j_vect_3 = j_vect - j_vect_2;

    j_vect_3*3.0;

    std::cout << j_vect_3;

    return 0;
}


#include "ocl_jlinearfilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>//pwd
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>      // std::ifstream
#include <iostream>

namespace jaspl {

namespace ocl {

JLinearConvolve::JLinearConvolve( uint device_number ) : JFilter( device_number ) {}

}

}

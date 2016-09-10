JVector<F> operator*= (F scalar) {

    for(unsigned int i=0; i<underlying_vector.size(); i++) {
        underlying_vector[i]*=scalar;
    }

    return *this;
}

JVector<F> operator* (F scalar) {

    for(unsigned int i=0; i<underlying_vector.size(); i++) {
        underlying_vector[i]*=scalar;
    }

    return *this;
}

JVector<F> operator+= (F scalar) {

    for(unsigned int i=0; i<underlying_vector.size(); i++) {
        underlying_vector[i]+=scalar;
    }
    return *this;
}


friend bool operator== (JVector<F>& vector_a, JVector<F>& vector_b) {

    return ( vector_a.underlying_vector == vector_b.underlying_vector);
}


friend bool operator!= (JVector<F>& vector_a, JVector<F>& vector_b) {

    return ( vector_a.underlying_vector != vector_b.underlying_vector);
}


friend std::ostream& operator << (std::ostream& stream, JVector<F>& spectrum) {

    for(unsigned int i=0; i<spectrum.size(); i++) {
        stream << spectrum.underlying_vector[i] << std::endl;
    }

    return stream;
}


friend std::ofstream& operator << (std::ofstream& stream, JVector<F>& spectrum) {

    for(unsigned int i=0; i<spectrum.size(); i++) {
        stream << spectrum.underlying_vector[i] << std::endl;
    }

    return stream;
}


friend JVector<F> operator+ (JVector<F>& vector_a, JVector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = __FUNCTION__ ;
        err_mesg += "\nJVectors are not the same size";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    std::transform(vector_c.underlying_vector.begin(),\
                   vector_c.underlying_vector.end(),\
                   vector_b.underlying_vector.begin(),\
                   vector_c.underlying_vector.begin(),\
                   std::plus<F>());

    return vector_c;
}


friend JVector<F> operator+ (JVector<F>& vector_a, std::vector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = "JVector and vector are not the same size ";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    vector_c.underlying_vector.resize(vector_b.size());

    std::transform(vector_c.underlying_vector.begin(),\
                   vector_c.underlying_vector.end(),\
                   vector_b.begin(),\
                   vector_c.underlying_vector.begin(),\
                   std::plus<F>());

    return vector_c;
}

friend JVector<F> operator- (JVector<F>& vector_a, JVector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = __FUNCTION__ ;
        err_mesg += "\nJVectors are not the same size";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    std::transform(vector_c.underlying_vector.begin(),\
                   vector_c.underlying_vector.end(),\
                   vector_b.underlying_vector.begin(),\
                   vector_c.underlying_vector.begin(),\
                   std::minus<F>());

    return vector_c;
}

friend JVector<F> operator- (JVector<F>& vector_a, std::vector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = "JVectors and vector are not the same size ";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    std::transform(vector_c.underlying_vector.begin(),\
                   vector_c.underlying_vector.end(),\
                   vector_b.begin(),\
                   vector_c.underlying_vector.begin(),\
                   std::minus<F>());

    return vector_c;
}

friend JVector<F> operator* (JVector<F>& vector_a, JVector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = __FUNCTION__ ;
        err_mesg += "\nJVectors are not the same size";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    for(unsigned int i=0; i<vector_a.size(); i++) {
        vector_c.underlying_vector[i] =\
                                       vector_a.underlying_vector[i] *\
                                       vector_b.underlying_vector[i];
    }

    return vector_c;
}

friend JVector<F> operator* (JVector<F>& vector_a, std::vector<F>& vector_b) {

    if( vector_a.size() != vector_b.size() ) {
        std::string err_mesg = "JVectors and vector are not the same size ";
        err_mesg += boost::lexical_cast<std::string> (vector_a.size());
        err_mesg += " vs ";
        err_mesg += boost::lexical_cast<std::string> (vector_b.size());
        throw std::length_error(err_mesg);
    }

    auto vector_c = vector_a;

    for(unsigned int i=0; i<vector_a.size(); i++) {
        vector_c.underlying_vector[i] =\
                                       vector_a.underlying_vector[i] *\
                                       vector_b[i];
    }

    return vector_c;
}

//Re-implementation of std::vector methods

void push_back( F element ) {
    underlying_vector.push_back( element );
}

void push_front( F element ) {
    underlying_vector.insert( underlying_vector.begin(), element);
}

typename std::vector<F>::iterator begin() {
    return underlying_vector.begin();
}

typename std::vector<F>::iterator end() {
    return underlying_vector.end();
}

F* data() {
    return underlying_vector.data();
}

F& operator[] ( const uint index ) {
    return underlying_vector[index];
}

F& at( const uint index ) {
    return underlying_vector.at( index );
}

void reserve ( uint n ) {
    underlying_vector.reserve( n );
}

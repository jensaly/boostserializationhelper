#include <boost_serialization_helper.h>

class Basic {
    int a;
    SERIALIZABLE float b;

    template<class Archive>
    void serialize(Archive & ar) {
        ar & a;
    }
};

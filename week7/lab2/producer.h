#ifndef PRODUCER_H
#define PRODUCER_H

#include "buffer.h"

class Producer {
public:
    void random_product(Buffer& buffer, int numProducers, int thread_id);

private:
};

#endif
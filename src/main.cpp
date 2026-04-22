#include <iostream>

#include "Indexer/Indexer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    InvertedIndex* idx = new InvertedIndex();
    Indexer* indexer = new Indexer(*idx);

    indexer->addPath("test");
    indexer->buildIndex();

    return 0;
}

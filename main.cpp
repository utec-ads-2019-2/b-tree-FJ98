#include "btree.h"

int main(int argc, char *argv[]) {
    const int ELEMENTS = 50; const int DEGREE = 3;
    BTree<int> bTree(DEGREE);

    for(int i = 1; i <= ELEMENTS; ++i) {
        bTree.insert(i);
    }

    std::cout << "Found: " << std::boolalpha << bTree.find(20) << std::endl;
    bTree.print();



    return EXIT_SUCCESS;
}
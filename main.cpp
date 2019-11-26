#include "btree.h"

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

int main(int argc, char *argv[]) {
    const int ELEMENTS = 30; const int DEGREE = 3;
    BTree<int> bTree(DEGREE);
    std::vector<int> elements;

    for(int i = 0; i < ELEMENTS; ++i) {
        bTree.insert(i);
        elements.push_back(i);
    }

    std::cout << "Original tree    : "; bTree.print();
    std::cout << "Original vector  : "; for (auto && item: elements){ std:: cout << item << " ";} std::cout << std::endl;

    std::cout << "\nTesting Remove" << std::endl;
    for(int i = 0; i < ELEMENTS; ++i) {
        elements.erase(elements.begin());
        bTree.remove(i);
        bool findInVec = std::find(elements.begin(), elements.end(), i) != elements.end();
        ASSERT(bTree.find(i) == findInVec, "There is a problem with the remove or find");
        ASSERT(!bTree.find(i), "There is a problem with the remove or find");
        std::cout << "In my tree    : "; bTree.print();
        std::cout << "In the vector : ";  for (auto && item: elements){ std:: cout << item << " "; }
        std::cout << std:: endl << std::endl;
    }

//    std::cout << "==========================================================" << std:: endl;
//    std::cout << "                      All test passed                     " << std:: endl;
//    std::cout << "==========================================================" << std:: endl;

    return EXIT_SUCCESS;
}
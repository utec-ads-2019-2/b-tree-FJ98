#ifndef BTREE_H
#define BTREE_H

#include "node.h"

template <typename T>
class BTree {
    private:
        Node<T>* root;
        unsigned int degree;

    public:
        explicit BTree(unsigned int degree) : degree(degree), root(nullptr) {};

        bool find(T data) { // TO DO
            if (!this->root) { return false; }
            return this->root->search(data);
        }

        bool insert(T data) { // TO DO
            if (this->find(data)) { return false; }

            /* If tree is empty */
            if (!this->root) {
                this->root = new Node<T>(this->degree);
                this->root->keys[0] = data; /* Insert key */
                this->root->size = 1;
            }

            /* If root is full */
            else if(this->root->size == this->degree - 1){
                auto newRoot = new Node<T>(this->degree, false);
                newRoot->children[0] = this->root;
                newRoot->splitChild(0, this->root);

                int i = 0;
                if (newRoot->keys[0] < data) { ++i; }
                newRoot->children[i]->insert(data);

                this->root = newRoot;
            }

            /* If root is not full */
            else {
                this->root->insert(data);
            }

            return true;
        }

        bool remove(T k) {
            if(!this->find(k)){ return false; }

            this->root->remove(k);

            if(this->root->size == 0){
                auto oldNode = this->root;
                this->root = this->root->isLeaf ? nullptr : this->root->children[0];
//                delete oldNode;
            }

            return true;
        }

        void print() { // TO DO
            if (!this->root) { return; }
            this->root->print();
            std::cout << std::endl;
        }

        ~BTree(){
            if (!this->root) { return; }
            this->root->killSelf();
        }
};

#endif
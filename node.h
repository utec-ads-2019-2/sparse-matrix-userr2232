#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    unsigned rows, columns, nonZeroes;
    Node<T> *next, *down;
    T value;
    unsigned indexValue;
    unsigned row, column;
    bool head;
    bool root;
public:
    explicit Node(unsigned indexValue) {
        this->indexValue = indexValue;
        this->next = nullptr;
        this->down = nullptr;
        this->head = true;
        this->root = false;
    }

    explicit Node(unsigned rows, unsigned columns) {
        this->rows = rows;
        this->columns = columns;
        this->nonZeroes = 0;
        this->head = true;
        this->next = nullptr;
        this->down = nullptr;
        this->root = true;
    }

    explicit Node(unsigned row, unsigned column, T value) {
        this->row = row;
        this->column = column;
        this->value = value;
        this->next = nullptr;
        this->down = nullptr;
        this->head = false;
        this->root = false;
    }

    void print() const {
        if(this->root) {
            cout << "root node" << endl;
            cout << "rows: " << this->rows << " columns: " << this->columns << endl;
            cout << "nonZeroes: " << this->nonZeroes << endl;
        } else if(this->head) {
            cout << "head" << endl;
            cout << "indexValue: " << this->indexValue << endl;
        } else {
            cout << "common node" << endl;
            cout << "row: " << this->row << " column: " << this->column << " value: " << this->value << endl;
        }
    }

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H

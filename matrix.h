#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {
private:
    Node<T> *root;
    unsigned rows, columns;
    vector<Node<T>* > rowHeads;
public:
    Matrix(unsigned rows, unsigned columns) {
        if(rows == 0 || columns == 0) throw runtime_error("error");
        this->rows = rows;
        this->columns = columns;
        this->root = new Node<T>(rows, columns);

        Node<T> *firstColumnsHead = new Node<T>(0);
        this->root->next = firstColumnsHead;
        firstColumnsHead->next = this->root;

        Node<T> *firstRowsHead = new Node<T>(0);

        Node<T> *current;
        current = firstRowsHead;
        rowHeads.push_back(current);
        for(int row = 1; row < rows; ++row) {
            Node<T> *newNode = new Node<T>(row);
            newNode->next = newNode;
            newNode->down = current->down;
            current->next = current;
            current->down = newNode;
            current = current->down;
            rowHeads.push_back(current);
        }

        current = firstColumnsHead;
        for(int column = 1; column < columns; ++column) {
            Node<T> *newNode = new Node<T>(column);
            newNode->next = current->next;
            newNode->down = newNode;
            current->next = newNode;
            current->down = current;
            current = current->next;
        }
    }

    void set(unsigned row, unsigned column, T value) {
        Node<T> *current = this->root;
        for(int i = 0; i <= column; ++i)
            current = current->next;

        bool nodeCreated = false;
        for(int i = 0; i <= row; ++i) {
            if(!current->head && current->row == row) break;
            if(current->down->head || current->down->row > row) {
                Node<T> *newNode = new Node<T>(row, column, value);
                if(value != 0) this->root->nonZeroes = this->root->nonZeroes + 1;
                nodeCreated = true;

                newNode->down = current->down;
                current->down = newNode;

                Node<T> *tmpRowNode = rowHeads[row];

                while(!tmpRowNode->next->head && tmpRowNode->next->column < column)
                    tmpRowNode = tmpRowNode->next;

                newNode->next = tmpRowNode->next;
                tmpRowNode->next = newNode;

                current = newNode;
                break;
            }
            current = current->down;
        }
        if(!nodeCreated) {
            if(current->value == 0 && value != 0) this->root->nonZeroes = this->root->nonZeroes + 1;
            current->value = value;
        }
    }
    T operator()(unsigned row, unsigned column) const {
        if(row > rows || column > columns) throw runtime_error("error");
        Node<T> *current = this->root;

        for(int i = 0; i <= column; ++i)
            current = current->next;

        for(int i = 0; i <= row; ++i) {
            current = current->down;
            if(!current->head && current->row == row) return current->value;
        }

        return 0;
    }
    Matrix<T> operator*(T scalar) const {
        Matrix<T> *result = new Matrix<T>(rows, columns);
        Node<T> *current = this->root->next;
        for(int column = 0; column < columns; ++column) {
            current = current->down;
            while(!current->head) {
                result->set(current->row, current->column, scalar*current->value);
                current = current->down;
            }
            current = current->next;
        }

        return *result;
    }
    Matrix<T> operator*(Matrix<T> other) const {
        Node<T> *current = this->root->next;
        Matrix<T> *result = new Matrix<T>(rows, columns);

        unsigned columns2 = other.getColumns();

        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < columns2; ++j) {
                for(int k = 0; k < columns; ++k) {
                    result->set(i, j, (*result)(i, j) + (*this)(i, k)*other(k, j));
                }
            }
        }
        return *result;
    }
    Matrix<T> operator+(Matrix<T> other) const {
        Node<T> *current = this->root->next;
        Matrix<T> *result = new Matrix<T>(rows, columns);

        for(int row = 0; row < rows; ++row)
            for(int column = 0; column < columns; ++column)
                result->set(row, column, (*this)(row, column) + other(row, column));

        return *result;
    }
    Matrix<T> operator-(Matrix<T> other) const {
        Node<T> *current = this->root->next;
        Matrix<T> *result = new Matrix<T>(rows, columns);

        for(int row = 0; row < rows; ++row)
            for(int column = 0; column < columns; ++column)
                result->set(row, column, (*this)(row, column) - other(row, column));

        return *result;
    }
    Matrix<T> transpose() const {
        Node<T> *current = this->root->next;
        Matrix<T> *result = new Matrix<T>(rows, columns);

        for(int row = 0; row < rows; ++row)
            for(int column = 0; column < columns; ++column)
                result->set(column, row, (*this)(row, column));
        
        return *result;
    }

    void print() const {
        Node<T> *current = this->root->next;
        for(int row = 0; row < rows; ++row) {
            for(int column = 0; column < columns; ++column)
                cout << (*this)(row, column) << " ";
            cout << endl;
        }
    }

    unsigned getColumns() const {
        return this->columns;
    }

    ~Matrix() {
        Node<T> *current = this->root;
        vector<Node<T>* > toDelete;
        toDelete.push_back(this->root);
        current = current->next;

        for(int column = 0; column < columns; ++column) {
            toDelete.push_back(current);
            current = current->next;
        }

        for(int row = 0; row < rows; ++row) {
            current = rowHeads[row]->next;
            toDelete.push_back(current);
            while(!current->head) {
                toDelete.push_back(current);
                current = current->next;
            }
        }

        int l = toDelete.size();
        for(int i = 0; i < l; ++i) {
            Node<T> *d = toDelete.back();
            toDelete.pop_back();
            d = nullptr;
            delete d;
        }
    }
};

#endif //SPARSE_MATRIX_MATRIX_H
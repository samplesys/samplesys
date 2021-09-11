
#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>

namespace SparseMatrix
{
    template <typename T>
    class SparseMatrix {
    public:
        SparseMatrix(int m, int n);
        SparseMatrix(int m, int n, long long val_num, long long *rows, int *cols, float *vals);
        ~SparseMatrix();

        T get(int row, int col) const;
        SparseMatrix<T> &set(T val, int row, int col);

        void validateCoordinates(int row, int col) const;
        void insert(int index, int row, int col, T val);
        void remove(int index, int row);

        SparseMatrix<T> operator+(const SparseMatrix &M) const;
        SparseMatrix<T> add(const SparseMatrix &M) const;

        std::vector<T> operator*(const std::vector<T> &x) const;
        std::vector<T> multiply(const std::vector<T> &x) const;

        SparseMatrix<T> operator*(const SparseMatrix &M) const;
        SparseMatrix<T> multiply(const SparseMatrix &M) const;

    protected:
        // row: 0 ~ m-1 ; column: 0 ~ n-1
        int m;
        int n;
        int *cols;

        long long *rows;

        T *vals;
    };

    class Exception : public std::exception
    {
    public:
        explicit Exception(const std::string &message) : exception(), message(message) {}

        virtual ~Exception(void) throw() {}

        inline std::string getMessage(void) const
        {
            return this->message;
        }

    protected:
        std::string message;
    };
    class InvalidDimensionsException : public Exception {
    public:
        InvalidDimensionsException(const std::string &message) : Exception(message) {}
    };
    class InvalidCoordinatesException : public Exception {
    public:
        InvalidCoordinatesException(const std::string &message) : Exception(message) {}
    };

} // namespace SparseMatrix


#include "utils/sparsematrix.h"

namespace SparseMatrix
{
    template <typename T>
    SparseMatrix<T>::SparseMatrix(int m, int n) {
        this->m = m;
        this->n = n;
        this->rows = static_cast<long long *>(malloc((m + 1) * sizeof(long long)));
        this->cols = nullptr;
        this->vals = nullptr;
    }

    template <typename T>
    SparseMatrix<T>::SparseMatrix(int m, int n, long long val_num,long long *rows, int *cols, float *vals) {
        this->m = m;
        this->n = n;
        this->rows = static_cast<long long *>(malloc((m + 1) * sizeof(long long)));
        memcpy(this->rows, rows, (m + 1) * sizeof(long long));
        if (val_num) {
            this->vals = static_cast<T *>(malloc(val_num * sizeof(T)));
            this->cols = static_cast<int *>(malloc(val_num * sizeof(int)));
            memcpy(this->vals, vals, val_num * sizeof(T));
            memcpy(this->cols, cols, val_num * sizeof(int));
        } else {
            this->vals = nullptr;
            this->cols = nullptr;
        }
    }

    template <typename T>
    T SparseMatrix<T>::get(int row, int col) const {
        int currCol;
        this->validateCoordinates(row, col);
        for (int p = this->rows[row]; p < this->rows[row + 1]; p++) {
            currCol = this->cols[this->rows[p]];
            if (currCol == col)
                return this->vals[p];
            else if (currCol > col)
                break;
        }
        return T();
    }

    template <typename T>
    SparseMatrix<T> &SparseMatrix<T>::set(T val, int row, int col) {
        this->validateCoordinates(row, col);

        int p = this->rows[row];
        int currCol = 0;

        for (; p < this->rows[row + 1]; p++) {
            currCol = this->cols[p];
            if (currCol >= col)
                break;
        }

        if (currCol != col) {
            if (val != T()) 
                this->insert(p, row, col, val);
        } else if (val == T()) 
            this->remove(p, row);
        else
            this->vals[p] = val;

        return *this;
    }

    template <typename T>
    void SparseMatrix<T>::validateCoordinates(int row, int col) const {
        if (row < 0 || col < 0 || row >= this->m || col >= this->n)
            throw InvalidCoordinatesException("Coordinates out of range.");
    }

    template<typename T>
	void SparseMatrix<T>::insert(int index, int row, int col, T val) {
		if (this->vals == NULL) {
			this->vals = static_cast<T *>(malloc(sizeof(T)));
			this->cols = static_cast<int *>(malloc(sizeof(int)));
		} else {
            T *tmpval = malloc((this->rows[this->m] + 1) * sizeof(T));
            int *tmpcol = malloc((this->rows[this->m] + 1) * sizeof(int));
            memcpy(tmpval, this->vals, index * sizeof(T));
            memcpy(tmpcol, this->cols, index * sizeof(int));
            tmpval[index] = val;
            tmpcol[index] = col;
            memcpy(tmpval + index + 1, this->val + index, (this->rows[this->m] - index) * sizeof(T));
            memcpy(tmpcol + index + 1, this->cols + index, (this->rows[this->m] - index) * sizeof(int));
			free(this->vals);
            free(this->cols);
            this->vals = tmpval;
            this->cols = tmpcol;
		}

		for (int i = row + 1; i <= this->m; i++) 
			this->rows[i] += 1;
	}
    
    template<typename T>
	void SparseMatrix<T>::remove(int index, int row) {
        T *tmpval = malloc((this->rows[this->m] - 1) * sizeof(T));
        int *tmpcol = malloc((this->rows[this->m] - 1) * sizeof(int));
        memcpy(tmpval, this->vals, index * sizeof(T));
        memcpy(tmpcol, this->cols, index * sizeof(int));
        memcpy(tmpval + index, this->val + index + 1, (this->rows[this->m] - index - 1) * sizeof(T));
        memcpy(tmpcol + index, this->cols + index + 1, (this->rows[this->m] - index - 1) * sizeof(int));
        free(this->vals);
        free(this->cols);
        this->vals = tmpval;
        this->cols = tmpcol;
		for (int i = row + 1; i <= this->m; i++) 
			this->rows[i] -= 1;
		
	}

    template <typename T>
    SparseMatrix<T>::~SparseMatrix() {
        free(this->rows);
        free(this->cols);
        free(this->vals);
    }

    template <typename T>
    SparseMatrix<T> SparseMatrix<T>::operator*(const SparseMatrix &M) const {
        return this->multiply(M);
    }

    template <typename T>
    SparseMatrix<T> SparseMatrix<T>::multiply(const SparseMatrix &M) const {
        if (this->n != M.m)
            throw InvalidDimensionsException("Cannot multiply: Left matrix column count and right matrix row count don't match.");
        SparseMatrix<T> result(this->m, M.n);
        T tmp;
        for (int i = 0; i < this->m; i++) {
            for (int j = 0; j < M.n; j++) {
                tmp = T();
                for (int k = 0; k < this->n; k++)
                    tmp = tmp + this->get(i, k) * M.get(k, j);
                result.set(tmp, i, j);
            }
        }
        return result;
    }

    template<typename T>
	SparseMatrix<T> SparseMatrix<T>::add(const SparseMatrix<T> & M) const {
		if (this->m != M.m || this->n != M.n) 
			throw InvalidDimensionsException("Cannot add: matrices dimensions don't match.");
		
		SparseMatrix<T> result(this->m, this->n);
		for (int i = 1; i <= this->m; i++) {
			for (int j = 1; j <= this->n; j++) {
				result.set(this->get(i, j) + M.get(i, j), i, j);
			}
		}
		return result;
	}


	template<typename T>
	SparseMatrix<T> SparseMatrix<T>::operator + (const SparseMatrix<T> & M) const {
		return this->add(M);
	}
} // namespace SparseMatrix

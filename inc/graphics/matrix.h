#ifndef MATRIX_H
#define MATRIX_H

#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <cmath>

template<unsigned int N, typename T>
class Matrix
{
private:
    T* pData;
    bool bDelete;
    unsigned int aSize[N];
    bool bTransposed = false;

public:
    Matrix()
            :
        pData()
    {}//constructor

    template<typename... Args>
    Matrix(Args... aSize)
            :
        bDelete(true),
        aSize { aSize... }
    {
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
            iTotal *= this->aSize[i];
            
        pData = new T[iTotal]();//initiate the array to zeros.
    }//constructor
    
    Matrix(const unsigned int aSize[N])
            :
        bDelete(true),
        aSize(aSize)
    {
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
            iTotal *= this->aSize[i];
            
        pData = new T[iTotal]();//initiate the array to zeros.
    }//constructor

    template<typename ...Args>
    Matrix(std::initializer_list<T> lElements, Args ...aSize)
            :
        bDelete(true),
        aSize { aSize... }
    {
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
            iTotal *= this->aSize[i];
            
        pData = new T[iTotal];//initiate the array.

        if(iTotal != lElements.size())
        throw std::out_of_range("trying to access a element int dimension 0");

        std::copy(lElements.begin(), lElements.end(), pData);
    }//constructor

    Matrix(std::initializer_list<std::initializer_list<T>> lElements)
            :
        bDelete(true)
    {
        if(N != 2)
            throw std::out_of_range("can only initialize two dimensional matrices this way");
        this->aSize[0] = lElements.size();
        this->aSize[1] = lElements.begin()->size();

        pData = new T[ lElements.begin()->size() * lElements.size()];//initiate the array.

        unsigned int counter = 0;
        for(std::initializer_list<T> lEle : lElements)
        {
            if(this->aSize[1] != lEle.size())
                throw std::out_of_range("all inner lists must have the same size");
                
            std::copy(lEle.begin(), lEle.end(), pData+counter++);
        }

    }//constructor

    Matrix(std::initializer_list<T> lElements)
            :
        bDelete(true)
    {
        if(N != 1)
            throw std::out_of_range("can only initialize one dimensional matrices this way");
        this->aSize[0] = lElements.size();
            
        pData = new T[lElements.size()];//initiate the array.

        std::copy(lElements.begin(), lElements.end(), pData);
    }//constructor

    Matrix(
        const Matrix<N+1,T> *pM,
        unsigned int iX
    )
            :
        bDelete(false)
    {
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
        {
            this->aSize[i] = pM->size(i+1);
            iTotal *= pM->size(i+1);
        }//for
        pData = &**pM + iX*iTotal;
    }//constructor

    ~Matrix()
    {
        if(bDelete)
            delete[] pData;
    }//deconstructor

    Matrix<N-1,T> operator[](unsigned int iX) const
    {
        if(N == 0)
            throw std::out_of_range("trying to access a element in dimension 0");
        if(iX >= size())
            throw std::out_of_range("trying to access a element in dimension 0");
        return Matrix<N-1,T>(this, iX);
    }//operator

    T& operator*() const
    {
        return *pData;
    }//operator

    Matrix<N,T> operator*(const Matrix<N,T> &rM) const
    {
        if(N > 2)
            throw std::out_of_range("multiplication is only defined for N <= 2");
        if(aSize[N-1] != rM.aSize[0])
            throw std::invalid_argument(
                    "the last dimension of this matrix must be equal "
                    "to the first dimension of the given matrix"
                );

        if(N == 0)
            return Matrix<N,T>( { **this * *rM } );

        unsigned int aNewSize[N](aSize);
        aNewSize[N-1] = rM.aSize[N-1];
        Matrix<N,T> xRet(aNewSize);

        for(unsigned int i = 0; i < aNewSize[0]; i++)
        {
            if(N == 1)
                xRet[i] = *(*this)[i] * *rM[i];
            else
            {
                T sum = 0;
                for(unsigned int i = 0; i < aNewSize[1]; i++)
                {

                }//for
            }//else
        }//for

        return xRet;
    }//operator

    Matrix& operator=(const T& rT)
    {
        pData[0] = rT;

        return *this;
    }//operator

    Matrix& operator=(std::initializer_list<T> lElements)
    {
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
            iTotal *= this->aSize[i];

        if(iTotal != lElements.size())
            throw std::out_of_range("trying to access a element int dimension 0");

        std::copy(lElements.begin(), lElements.end(), pData);

        return *this;
    }//operator

    unsigned int size(unsigned int i) const
    {
        if(N > i)
            return aSize[i];
        if( (N == i) == 0)
            return 0;
        throw std::out_of_range("trying to access size of unpresent dimension");
    }//function

    unsigned int size() const
    {
        return size(0);
    }//function

    T abs() const
    {
        T sum = 0;
        unsigned int iTotal = 1;
        for(unsigned int i = 0; i < N; i++)
            iTotal *= this->aSize[i];

        for(unsigned int i=0; i <iTotal;i++)
            sum += pData[i] * pData[i];

        return sqrt(sum);
    }//function

};//class


template<typename T>
std::ostream& operator<<(std::ostream& rOs, const Matrix<0,T> &rM )
{
    rOs << *rM;
    return rOs;
}//operator

template<unsigned int N, typename T>
std::ostream& operator<<(std::ostream& rOs, const Matrix<N,T> &rM )
{
    rOs << "{";
    for(unsigned int i = 0; i < rM.size()-1; i++)
        rOs << rM[i] << ",";
    rOs << rM[rM.size()-1] << "}";
    return rOs;
}//operator

#endif
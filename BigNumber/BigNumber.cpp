#include "BigNumber.h"

#include <exception>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>


int BigNumber::numOfDigit(unsigned long long x)
{
    int result = 1;

    while (x >= 10)
    {
        x /= 10;
        ++result;
    }

    return result;
}

int BigNumber::compare (const BigNumber& y) const
{
    if (this->n != y.n)
    {
        throw std::invalid_argument("Lenght of numbers are not equal.");
    }

    for (int i = n - 1; i >= 0; --i)
    {
        if ((*this)[i] > y[i])
            return 1;

        if ((*this)[i] < y[i])
            return -1;
    }

    return 0;
}

unsigned char& BigNumber::operator[] (const long long x) const
{
    return this->arrayBegin[x];
}



BigNumber::BigNumber()
{
    n = 1;
    sign = SIGN::PLUS;

    arrayBegin = new unsigned char [1];
    arrayBegin[0] = 0;
}

BigNumber::BigNumber(const BigNumber& x)
{
    n = x.n;
    sign = x.sign;

    arrayBegin = new unsigned char [n];
    std::memcpy(arrayBegin, x.arrayBegin, sizeof(unsigned char) * n);

}

BigNumber::BigNumber(int number)
{
    if (number < 0)
    {
        sign = MINUS;
        number *= (-1);
    }
    else
    {
        sign = PLUS;
    }

    n = numOfDigit(number);
    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n; ++i)
    {
        arrayBegin[i] = (number % 10);
        number /= 10;
    }

}

BigNumber::BigNumber(long long number)
{
    if (number < 0)
    {
        sign = SIGN::MINUS;
        number *= (-1);
    }
    else
    {
        sign = SIGN::PLUS;
    }

    n = numOfDigit(number);
    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n; ++i)
    {
        arrayBegin[i] = (number % 10);
        number /= 10;
    }

}

BigNumber::BigNumber(const int * arr, int arrSize)
{
    n = arrSize;
    sign = PLUS;
    arrayBegin = new unsigned char [arrSize];

    for (int i = n - 1; i >= 0; --i, ++arr)
    {

        if (*arr >= 0 && *arr <= 9)
        {
            arrayBegin[i] = *arr;
        }
        else //One of number in array is not in rage <0,9>
        {
            throw std::invalid_argument("Incorrect table");
        }

    }
}

BigNumber::BigNumber(const char * number)
{
    bool stringWithSign = 0;

    switch (number[0])
    {

    case '-':
        sign = MINUS;
        n = std::strlen(number) - 1;
        stringWithSign = 1;
        break;

    case '+':
        sign = PLUS;
        n = std::strlen(number) - 1;
        stringWithSign = 1;
        break;

    default:
        if (number[0] >= '0' && number[0] <= '9')
        {
            sign = PLUS;
            n = std::strlen(number);
        }
        else //First character is not from the set {'-', '+', <0-9>}
        {
            throw std::invalid_argument("Incorrect string");
        }

    }

    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n ; ++i)
    {

        int temp =  number[i + stringWithSign] - '0';

        if (temp >= 0 && temp <= 9)
        {
            arrayBegin[(n - 1) - i] = temp;
        }
        else //One of character in string is not in rage <0,9>
        {
            throw std::invalid_argument("Incorrect string");
        }

    }
}

BigNumber::BigNumber(const std::string& number)
{
    bool stringWithSign = 0;

    switch (number[0])
    {

    case SIGN::PLUS:
        sign = PLUS;
        n = number.size() - 1;
        stringWithSign = 1;
        break;

    case SIGN::MINUS:
        sign = MINUS;
        n = number.size() - 1;
        stringWithSign = 1;
        break;

    default:
        if (number[0] >= '0' && number[0] <= '9')
        {
            sign = PLUS;
            n = number.size();
        }
        else //First character is not from the set {'-', '+', <0-9>}
        {
            throw std::invalid_argument("Incorrect number");
        }
        break;
    }

    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n ; ++i)
    {
        int temp =  number.at(i + stringWithSign) - '0';

        if (temp >= 0 && temp <= 9)
        {
            arrayBegin[(n - 1) - i] = temp;
        }
        else //One of character in string is not in rage <0,9>
        {
            throw std::invalid_argument("Incorrect number");
        }
    }
}

BigNumber::~BigNumber()
{
    delete [] arrayBegin;
}


unsigned int BigNumber::lenght() const
{
    return n;
}


BigNumber& BigNumber::operator= (const BigNumber& x)
{
    if (this == &x )    //same object
        return *this;

    n = x.n;
    sign = x.sign;

    delete [] arrayBegin;

    arrayBegin = new unsigned char [n];
    std::memcpy(arrayBegin, x.arrayBegin, sizeof(unsigned char) * n);

    return *this;
}

BigNumber& BigNumber::operator= (long long number)
{
    if (number < 0)
    {
        sign = MINUS;
        number *= (-1);
    }
    else
    {
        sign = PLUS;
    }

    n = numOfDigit(number);

    delete [] arrayBegin;

    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n; ++i)
    {
        arrayBegin[i] = (number % 10);
        number /= 10;
    }

    return *this;
}

BigNumber& BigNumber::operator= (const std::string& number)
{
    bool stringWithSign = 0;

    switch (number[0])
    {

    case SIGN::MINUS :
        sign = MINUS;
        n = number.size() - 1;
        stringWithSign = 1;
        break;

    case SIGN::PLUS :
        sign = PLUS;
        n = number.size() - 1;
        stringWithSign = 1;
        break;

    default:
        if (number[0] >= '0' && number[0] <= '9')
        {
            sign = PLUS;
            n = number.size();
        }
        else //First character is not from the set {'-', '+', <0-9>}
        {
            throw std::invalid_argument("Incorrect number");
        }

    }

    delete [] arrayBegin;

    arrayBegin = new unsigned char [n];

    for (unsigned int i = 0; i < n ; ++i)
    {
        int temp =  number.at(i + stringWithSign) - '0';

        if (temp >= 0 && temp <= 9)
        {
            arrayBegin[(n - 1) - i] = temp;
        }
        else //One of character in string is not in rage <0,9>
        {
            throw std::invalid_argument("Incorrect number");
        }
    }

    return *this;
}


BigNumber BigNumber::operator+ (const BigNumber& y) const
{

    unsigned int maxLenghtOfSum = std::max(this->n, y.n) + 1;

    signed char* tempSum = new signed char [maxLenghtOfSum];

    /**
    * We add numbers to each other in columns.
    *
    *                               1  2  3
    *   e.g. (123 + (-99)) =>    +   -(9  9)
    *                            ------------
    *                              = 1 -7 -6
    *
    * We obtain the correct number but in a specific
    * numerical system (base= 10, digit= <-18,18>).
    *
    * The number is fixing in two phases:
    * ("Phase One") Transform into => (base= 10, digit= <-9,9>)
    * ("Phase Two") Transform into => (base= 10, digit= <0,9>, with separate sign)
    */

    for (unsigned int i = 0; i < maxLenghtOfSum; ++i) //add numbers to each other in columns
    {
        tempSum[i] = 0;

        if (i < this->n)
        {
            if (this->sign == SIGN::PLUS)
                tempSum[i] += (*this)[i];
            else
                tempSum[i] -= (*this)[i];
        }

        if (i < y.n)
        {
            if (y.sign == SIGN::PLUS)
                tempSum[i] += y[i];
            else
                tempSum[i] -= y[i];
        }

    }

    long long lastNotZero = -1; //index of last number in table "tempSum"
                                //which is not zero (-1 mean bouth number are same)
    SIGN signOfSum;

    for (long long i = maxLenghtOfSum - 1; i >= 0; --i)
    {
        if (tempSum[i] != 0)
        {
            lastNotZero = i;
            if (tempSum[lastNotZero] > 0)
                signOfSum = SIGN::PLUS;
            else
                signOfSum = SIGN::MINUS;

            break;
        }
    }

    if (lastNotZero == -1)  //sum is equal 0
        return *(new BigNumber(0) );

    if (signOfSum == SIGN::PLUS)    //fixing number "Phase One"
    {
        for (unsigned int i = 0; i <= lastNotZero ; ++i)
        {
            if (tempSum[i] < 0)
            {
                --tempSum[i + 1];
                tempSum[i] += 10;
            }

            if (tempSum[i] > 9)
            {
                tempSum[i] -= 10;
                ++tempSum[i + 1];
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i <= lastNotZero ; ++i)
        {
            if (tempSum[i] > 0)
            {
                ++tempSum[i + 1];
                tempSum[i] -= 10;
            }

            if (tempSum[i] < -9)
            {
                tempSum[i] += 10;
                --tempSum[i + 1];
            }
        }
    }

    if (tempSum[lastNotZero + 1] != 0)  //when repairing the number the length has increased by one
        ++lastNotZero;
    else
    {
        if(tempSum[lastNotZero] == 0) //when repairing the number the length has decreased by one
        --lastNotZero;
    }


    BigNumber sum;
    delete [] sum.arrayBegin; //BigNumber() creates an array with a size of 1

    sum.n = lastNotZero + 1;

    sum.arrayBegin = new unsigned char[sum.n];

    for (unsigned int i = 0; i <= lastNotZero; ++i) //fixing number "Phase Two"
    {
        if (signOfSum == SIGN::PLUS)
            sum[i] = tempSum[i];
        else
            sum[i] = -tempSum[i];
    }

    sum.sign = signOfSum; //fixing number "Phase Two"

    delete [] tempSum;

    return sum;
}

BigNumber& BigNumber::operator+=(const BigNumber& y)
{
    *this = *this + y;
    return *this;
}


BigNumber BigNumber::operator-(const BigNumber& y) const
{
    BigNumber temp = (*this) + (-y);
    return temp;
}

BigNumber& BigNumber::operator-=(const BigNumber& y)
{
    *this = *this - y;
    return *this;
}


BigNumber BigNumber::operator*(const BigNumber& y) const
{
    if (*this == 0 || y == 0)
        return BigNumber(0);

    BigNumber product;
    delete [] product.arrayBegin;   //BigNumber(void) creates an array with a size of 0

    /**
    * Before multiplication, we are not able to determine how many digits
    * will be the result (can be (this->n + y.n) or (this->n + y.n - 1)).
    * So we allocate enough memory for all cases ( this->n + y.n ).
    * Only after obtaining the product we will assign the size of "product.n".
    */
    unsigned int maxLenghtOfProduct = this->n + y.n;
    product.arrayBegin = new unsigned char [maxLenghtOfProduct];

    for (unsigned int i = 0; i < maxLenghtOfProduct; ++i)
        product[i] = 0;

    if (this->sign != y.sign)
        product.sign = SIGN::MINUS;
    else
        product.sign = SIGN::PLUS;

    for (unsigned int i = 0; i < this->n; ++i)
    {
        for (unsigned int j = 0; j < y.n ; ++j)
        {
            product[i + j] += (*this)[i] * y[j];
        }
        for (unsigned int j = 0; j < y.n; ++j)
        {
            product[i + j + 1] += product[i + j] / 10;
            product[i + j] = product[i + j] % 10;
        }
    }

    if (product[maxLenghtOfProduct - 1] == 0) //fix lenght of product
        product.n = this->n + y.n - 1;
    else
        product.n = this->n + y.n;

    return product;
}

BigNumber& BigNumber::operator*=(const BigNumber& y)
{
    *this = *this * y;
    return *this;
}


BigNumber BigNumber::operator/(BigNumber y) const
{
    if (y == 0)
    {
        throw std::invalid_argument("Divide by zero");
    }

    BigNumber copyThis = *this;
    BigNumber quotient = 0;

    SIGN signOfQuotient;
    if (this->sign == y.sign )
        signOfQuotient = SIGN::PLUS;
    else
        signOfQuotient = SIGN::MINUS;

    if (this->sign == SIGN::MINUS)
        copyThis = (-copyThis);

    if (y.sign == SIGN::MINUS)
        y = (-y);

    while (copyThis >= y)
    {
        copyThis -= y;
        quotient = quotient + 1;
    }

    if (signOfQuotient == SIGN::MINUS)
        quotient = -quotient;

    return quotient;
}

BigNumber& BigNumber::operator/=(const BigNumber& y)
{
    *this = *this / y;
    return *this;
}


BigNumber BigNumber::operator+ () const
{
    return *this;
}

BigNumber BigNumber::operator- () const
{
    BigNumber temp = *this;

    if (temp.sign == SIGN::MINUS)
        temp.sign = SIGN::PLUS;
    else
        temp.sign = SIGN::MINUS;

    return temp;
}


BigNumber& BigNumber::operator++()
{
    (*this) += 1;
    return *this;
}

BigNumber& BigNumber::operator--()
{
    *this -= 1;
    return *this;
}


bool BigNumber::operator==(const BigNumber& y) const
{
    if(this->sign != y.sign)
        return false;

    if (this->n != y.n)
        return false;

    int result = this->compare(y);

    if (result != 0)
        return false;

    return true;
}

bool BigNumber::operator!=(const BigNumber& y) const
{
    return !(*this == y);
}

bool BigNumber::operator>(const BigNumber& y) const
{
    if(this->sign == SIGN::PLUS && y.sign == SIGN::MINUS)
        return true;

    if(this->sign == SIGN::MINUS && y.sign == SIGN::PLUS)
        return false;

    if(this->sign == SIGN::PLUS && y.sign == SIGN::PLUS)
    {
        if (this->n > y.n)
            return true;

        if (this->n < y.n)
            return false;

        int result =  this->compare(y);

        if (result > 0)
            return true;
        else
            return false;
    }
    else //both negative numbers
    {
        if (this->n > y.n)
            return false;

        if (this->n < y.n)
            return true;

        int result =  this->compare(y);

        if (result >= 0)
            return false;
        else
            return true;
    }

}

bool BigNumber::operator<(const BigNumber& y) const
{
    if(this->sign == SIGN::PLUS && y.sign == SIGN::MINUS)
        return false;

    if(this->sign == SIGN::MINUS && y.sign == SIGN::PLUS)
        return true;

    if(this->sign == SIGN::PLUS && y.sign == SIGN::PLUS)
    {
        if (this->n > y.n)
            return false;

        if (this->n < y.n)
            return true;

        int result = this->compare(y);

        if (result >= 0)
            return false;
        else
            return true;

    }
    else //both negative numbers
    {
        if (this->n > y.n)
            return true;

        if (this->n < y.n)
            return false;

        return  this->compare(y) > 0;
//        if (result > 0)
 //           return true;
   //     else
     //       return false;
    }

}

bool BigNumber::operator>=(const BigNumber& y) const
{
    return !(*this < y);
}

bool BigNumber::operator<=(const BigNumber& y) const
{
    return !(*this > y);
}


std::ostream& operator<<(std::ostream& out, const BigNumber& x)
{
    if (x.sign == SIGN::MINUS)
        out << '-';

    for (int i = x.n - 1; i >= 0; --i)
        out << (int)x.arrayBegin[i];


    return out;
}

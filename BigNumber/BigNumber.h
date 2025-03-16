#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <string>


enum SIGN {PLUS = '+' , MINUS = '-'};

class BigNumber
{
    private:

        unsigned int n;
        unsigned char * arrayBegin;
        SIGN sign;

        static int numOfDigit (unsigned long long x);



        unsigned char& operator[] (const long long x) const; //returns the reference to X of that number <noexcept>


    public:

        BigNumber();                                        // <throw std::bad_alloc>
        BigNumber(const BigNumber& x);                      // <throw std::bad_alloc>
        BigNumber(int number);                              // <throw std::bad_alloc>
        BigNumber(long long number);                        // <throw std::invalid_argument, std::bad_alloc>
        BigNumber(const int arr[], int arrSize);            // <throw std::invalid_argument, std::bad_alloc>
        BigNumber(const char * number);                     // <throw std::invalid_argument, std::bad_alloc>
        BigNumber(const std::string& number);               // <throw std::invalid_argument, std::bad_alloc>
        ~BigNumber();

        unsigned int lenght() const;

        BigNumber& operator= (const BigNumber& x);          // <throw std::bad_alloc>
        BigNumber& operator= (long long number);            // <throw std::bad_alloc>
        BigNumber& operator= (const std::string& number);   // <throw std::invalid_argument, std::bad_alloc>

        BigNumber operator+ (const BigNumber& y) const;     // <throw std::bad_alloc>
        BigNumber& operator+= (const BigNumber& x);         // <throw std::bad_alloc>

        BigNumber operator- (const BigNumber& y) const;     // <throw std::bad_alloc>
        BigNumber& operator-= (const BigNumber& x);         // <throw std::bad_alloc>

        BigNumber operator* (const BigNumber& y) const;     // <throw std::bad_alloc>
        BigNumber& operator*= (const BigNumber& y);         // <throw std::bad_alloc>

        BigNumber operator/ (BigNumber y) const;            // <throw std::invalid_argument, std::bad_alloc>
        BigNumber& operator/= (const BigNumber& x);         // <throw std::invalid_argument, std::bad_alloc>

        BigNumber operator+ () const;
        BigNumber operator- () const;

        BigNumber& operator++ ();       // Prefix           // <throw std::bad_alloc>
        BigNumber& operator-- ();       // Prefix           // <throw std::bad_alloc>

        bool operator== (const BigNumber& y) const;
        bool operator!= (const BigNumber& y) const;
        bool operator> (const BigNumber& y) const;
        bool operator< (const BigNumber& y) const;
        bool operator>= (const BigNumber& y) const;
        bool operator<= (const BigNumber& y) const;

        friend std::ostream& operator<< (std::ostream &, const BigNumber& x);

        int compare (const BigNumber& y) const; //compare two BigNumber with same lenght
                                                //Returm: 0 - equal , 1 - first bigger, -1 - second bigger
};

#endif //BIGNUMBER_H


/*****************************************************************************
 * fixed.cpp        Blitz++ array using a custom type
 *
 * $Id$
 *
 * $Log$
 * Revision 1.2  2001/01/24 23:41:54  tveldhui
 * Widespread changes to reduce compile time.  For backwards
 * compatibility, #include <blitz/array.h> enables BZ_GANG_INCLUDE
 * mode which includes all array and vector functionality (about
 * 120000 lines of code).  #include <blitz/array-only.h> includes
 * a minimal subset of Array funcitonality; other features must
 * be included explicitly.
 *
 * Revision 1.1.1.1  2000/06/19 12:26:17  tveldhui
 * Imported sources
 *
 * Revision 1.1  1997/07/19 14:19:35  tveldhui
 * Initial revision
 *
 *
 *****************************************************************************
 * This example illustrates how simple it is to create Blitz++ arrays
 * using a custom type.  
 */

#include <blitz/array-only.h>
#include <blitz/array/ops.h>
#include <blitz/numinquire.h>

#ifdef BZ_NAMESPACES
using namespace blitz;
#endif

// A simple fixed point arithmetic class which represents a point
// in the interval [0,1].
class FixedPoint {

public:
    typedef unsigned int T_mantissa;

    FixedPoint() { }

    explicit FixedPoint(T_mantissa mantissa)
    {  
        mantissa_ = mantissa;
    }

    FixedPoint(double value)
    {
        assert((value >= 0.0) && (value <= 1.0));
        mantissa_ = T_mantissa(value * huge(T_mantissa()));
    }
   
    FixedPoint operator+(FixedPoint x)
    { return FixedPoint(mantissa_ + x.mantissa_); }

    double value() const
    { return mantissa_ / double(huge(T_mantissa())); }

private:
    T_mantissa mantissa_;
};

ostream& operator<<(ostream& os, const FixedPoint& a)
{
    os << a.value();
    return os;
}

int main()
{
    // Now create an array using the FixedPoint class:

    Array<FixedPoint, 2> A(4,4), B(4,4);

    A = 0.5, 0.3, 0.8, 0.2,
        0.1, 0.3, 0.2, 0.9,
        0.0, 1.0, 0.7, 0.4,
        0.2, 0.3, 0.8, 0.4;

    B = A + 0.05;

    cout << "B = " << B << endl;

    return 0;
}


// Program output:
// B = 4 x 4
//      0.55      0.35      0.85      0.25
//      0.15      0.35      0.25      0.95
//      0.05      0.05      0.75      0.45
//      0.25      0.35      0.85      0.45

/*
 * Note: Just because Array<T,N> supports all possible operators doesn't
 * mean that a user-defined class has to.  You only need to define the 
 * operators you actually use on the array.  This works because the ISO/ANSI
 * draft standard forbids instantiation of unused member functions:
 *
 * [temp.inst, paragraph 7]
 * An implementation shall not implicitly instantiate  a  function,  non-
 * virtual  member  function,  class  or  member  template  that does not
 * require instantiation.  It is unspecified whether or not an  implemen-
 * tation implicitly instantiates a virtual member function that does not
 * require specialization.
 */

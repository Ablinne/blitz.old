// -*- C++ -*-
/***************************************************************************
 * blitz/matgen.h       Declarations for RowMajor and ColumnMajor matrices
 *
 * $Id$
 *
 * Copyright (C) 1997-2011 Todd Veldhuizen <tveldhui@acm.org>
 *
 * This file is a part of Blitz.
 *
 * Blitz is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Blitz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with Blitz.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Suggestions:          blitz-devel@lists.sourceforge.net
 * Bugs:                 blitz-support@lists.sourceforge.net    
 *
 * For more information, please see the Blitz++ Home Page:
 *    https://sourceforge.net/projects/blitz/
 *
 ***************************************************************************/

#ifndef BZ_MATGEN_H
#define BZ_MATGEN_H

#ifndef BZ_MSTRUCT_H
 #error <blitz/matgen.h> must be included via <blitz/mstruct.h>
#endif // BZ_MSTRUCT_H

BZ_NAMESPACE(blitz)

class GeneralMatrix : public AsymmetricMatrix {

public:
    GeneralMatrix()
    { }

    GeneralMatrix(unsigned rows, unsigned cols)
        : AsymmetricMatrix(rows, cols)
    {
    }

    unsigned firstInRow(unsigned i) const
    { return 0; }

    unsigned lastInRow(unsigned i) const
    { return cols_ - 1; }

    unsigned firstInCol(unsigned j) const
    { return 0; }

    unsigned lastInCol(unsigned j) const
    { return rows_ - 1; }

    unsigned numElements() const
    { return rows_ * cols_; }
};

class GeneralIterator {
public:
    GeneralIterator(unsigned rows, unsigned cols)
    {
        rows_ = rows;
        cols_ = cols;
        i_ = 0;
        j_ = 0;
        offset_ = 0;
        good_ = true;
    }

    unsigned offset() const { return offset_; }
    operator bool()   const { return good_; }
    unsigned row()    const { return i_; }
    unsigned col()    const { return j_; }
 
protected:
    unsigned rows_, cols_;
    unsigned offset_;
    unsigned i_, j_;
    bool     good_;
};

class RowMajorIterator : public GeneralIterator {
public:
    RowMajorIterator(unsigned rows, unsigned cols)
        : GeneralIterator(rows, cols)
    { }

    void operator++()
    {
        ++offset_;
        ++j_;
        if (j_ == cols_)
        {
            j_ = 0;
            ++i_;
            if (i_ == rows_)
                good_ = false;
        }
    }
};

class RowMajor : public GeneralMatrix {

public:
    typedef RowMajorIterator T_iterator;

    RowMajor()
    { }

    RowMajor(unsigned rows, unsigned cols)
        : GeneralMatrix(rows, cols)
    { }

    unsigned coordToOffset(unsigned i, unsigned j) const
    {
        return i*cols_+j;
    }

    template<typename T_numtype>
    T_numtype get(const T_numtype * restrict data,
        unsigned i, unsigned j) const
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }

    template<typename T_numtype>
    T_numtype& get(T_numtype * restrict data, unsigned i, unsigned j)
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }
};

class ColumnMajorIterator : public GeneralIterator {
public:
    ColumnMajorIterator(unsigned rows, unsigned cols)
        : GeneralIterator(rows, cols)
    {
    }

    void operator++()
    {
        ++offset_;
        ++i_;
        if (i_ == rows_)
        {
            i_ = 0;
            ++j_;
            if (j_ == cols_)
                good_ = false;
        }
    }
};

class ColumnMajor : public GeneralMatrix {

public:
    ColumnMajor()
    { }

    ColumnMajor(unsigned rows, unsigned cols)
        : GeneralMatrix(rows, cols)
    { }

    unsigned coordToOffset(unsigned i, unsigned j) const
    {
        return j*rows_ + i;
    }

    template<typename T_numtype>
    T_numtype get(const T_numtype * restrict data,
        unsigned i, unsigned j) const
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }

    template<typename T_numtype>
    T_numtype& get(T_numtype * restrict data, unsigned i, unsigned j)
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }
};

BZ_NAMESPACE_END

#endif // BZ_MATGEN_H


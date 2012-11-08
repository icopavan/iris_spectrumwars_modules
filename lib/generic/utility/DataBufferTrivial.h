/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file DataBufferTrivial.h
 * A stripped-down version of the DataBuffer for component tests.
 *
 *  Created on: 19-Oct-2009
 *  Created by: lotzej
 *  $Revision: 714 $
 *  $LastChangedDate: 2009-10-20 20:14:02 +0100 (Tue, 20 Oct 2009) $
 *  $LastChangedBy: lotzej $
 *
 */

#ifndef DATABUFFERTRIVIAL_H_
#define DATABUFFERTRIVIAL_H_

#include "irisapi/DataBufferInterfaces.h"
#include "irisapi/Exceptions.h"
#include "irisapi/TypeInfo.h"

namespace iris
{

/*!
*   \brief The DataBufferTrivial class implements a buffer between Iris components for testing.
*
*	The buffer consists of a number of DataSet objects which can be written and read by the components.
*	Components can get a DataSet to write to by calling GetWriteSet(). When finished writing, the component
*	releases the DataSet by calling ReleaseWriteSet().
*	Components can get a DataSet to read from by calling GetReadSet(). When finished reading, the component
*	releases the DataSet by calling ReleaseReadSet().
*	The DataBufferTrivial is not thread-safe. It does not block, and keeps growing if new DataSets are
*	requested.
*/
template <typename T>
class DataBufferTrivial : public ReadBuffer<T>, public WriteBuffer<T>
{
public:

    /*!
    *   \brief Constructor
    */
    explicit DataBufferTrivial(std::size_t buffer_size = 3) throw (InvalidDataTypeException)
        :d_buffer(buffer_size) ,
        d_isReadLocked(false),
        d_isWriteLocked(false),
        d_readIndex(0),
        d_writeIndex(0),
        d_notEmpty(false),
        d_notFull(true)
    {
        //Set the type identifier for this buffer
        typeIdentifier = TypeInfo<T>::identifier;
        if( typeIdentifier == -1)
            throw InvalidDataTypeException("Data type not supported");
    };

    virtual ~DataBufferTrivial(){};

    //! Get the identifier for the data type of this buffer
    virtual int getTypeIdentifier() const   {  return typeIdentifier; }

    //! Is there any data in this buffer?
    virtual bool hasData() const { return is_not_empty(); }

    // empty implementation - not needed for tests
    virtual void setLinkDescription(LinkDescription) {};
    virtual LinkDescription getLinkDescription() const { return LinkDescription(); }

    /*!
    *   \brief Get the next DataSet to read
    *
    *   \param setPtr   A DataSet pointer which will be set by the buffer
    */
    virtual void getReadData(DataSet<T>*& setPtr) throw(DataBufferReleaseException)
    {
        if(d_isReadLocked)
            throw DataBufferReleaseException("getReadData() called before previous DataSet was released");
        d_isReadLocked = true;
        setPtr = &d_buffer[d_readIndex];
    };

    /*!
    *   \brief Get the next DataSet to be written
    *
    *   \param setPtr   A DataSet pointer which will be set by the buffer
    *   \param size     The number of elements required in the DataSet
    */
    virtual void getWriteData(DataSet<T>*& setPtr, std::size_t size) throw(DataBufferReleaseException)
    {
        if(d_isWriteLocked)
            throw DataBufferReleaseException("getWriteData() called before previous DataSet was released");
        d_isWriteLocked = true;
        if(d_buffer[d_writeIndex].data.size() != size)
            d_buffer[d_writeIndex].data.resize(size);
        setPtr = &d_buffer[d_writeIndex];
    };

    /*!
    *   \brief Release a read DataSet
    *
    *   \param setPtr   A pointer to the DataSet to be released
    */
    virtual void releaseReadData(DataSet<T>*& setPtr)
    {
        if(++d_readIndex == d_buffer.size())
        {
            d_buffer.resize(d_readIndex+1);
        }
        if(d_readIndex == d_writeIndex)
            d_notEmpty = false;
        d_notFull = true;
        d_isReadLocked = false;
        setPtr = NULL;
    };

    /*!
    *   \brief Release a write DataSet
    *
    *   \param setPtr   A pointer to the DataSet to be released
    */
    virtual void releaseWriteData(DataSet<T>*& setPtr)
    {
        if(++d_writeIndex == d_buffer.size())
        {
            d_buffer.resize(d_writeIndex+1);
        }
        if(d_readIndex == d_writeIndex)
            d_notFull = false;
        d_notEmpty = true;
        d_isWriteLocked = false;
        setPtr = NULL;
    };

    std::vector< DataSet<T> > getBuffer() { return d_buffer; }

private:
    //! The data type of this buffer
    int typeIdentifier;

    //! The vector of DataSets
    std::vector< DataSet<T> > d_buffer;

    bool d_isReadLocked;
    bool d_isWriteLocked;

    std::size_t d_readIndex;
    std::size_t d_writeIndex;

    bool d_notEmpty;
    bool d_notFull;

    bool is_not_empty() const { return d_notEmpty; }
    bool is_not_full() const { return d_notFull; }

};

} /* namespace iris */


#endif /* DATABUFFERTRIVIAL_H_ */
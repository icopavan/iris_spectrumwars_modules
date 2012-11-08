/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file UdpSocketTxComponent.cpp
 *
 *  Created on: 30-Jan-2009
 *  Created by: suttonp
 *  $Revision: 714 $
 *  $LastChangedDate: 2009-10-20 20:14:02 +0100 (Tue, 20 Oct 2009) $
 *  $LastChangedBy: lotzej $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "UdpSocketTxComponent.h"

using namespace std;
using namespace boost::asio::ip;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, UdpSocketTxComponent);

    UdpSocketTxComponent::UdpSocketTxComponent(string name):
        PNComponent(name, "udpsockettxpncomponent", "A UDP socket tx", "Paul Sutton", "0.1")
    {
        //Register all parameters
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("address", "Address of the target machine", "127.0.0.1", false, x_address);
        registerParameter("port", "Port of the target machine", "1234", false, x_port);

        d_socket = NULL;
        d_endPoint = NULL;
    }

    void UdpSocketTxComponent::registerPorts()
    {
        //Register all ports
        vector<int> validTypes; //This component supports all data types
        validTypes.push_back( int(TypeInfo< uint8_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint16_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint32_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint64_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int8_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int16_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int32_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int64_t >::identifier) );
        validTypes.push_back( int(TypeInfo< float >::identifier) );
        validTypes.push_back( int(TypeInfo< double >::identifier) );
        validTypes.push_back( int(TypeInfo< long double >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<float> >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<double> >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<long double> >::identifier) );

        //format:        (name, vector of valid types)
        registerInputPort("input1", validTypes);
    }

    map<string, int> UdpSocketTxComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
        //No output
        map<string, int> outputTypes;
        return outputTypes;
    }

    void UdpSocketTxComponent::initialize()
    {
        //Create socket
        try{
            d_socket = new boost::asio::ip::udp::socket(d_ioService);
            d_socket->open(udp::v4());
            d_endPoint = new udp::endpoint(address::from_string(x_address), x_port);
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to create socket: " << e.what();
        }
    }

    void UdpSocketTxComponent::process()
    {
        if( outputBuffers.size() != 0 || inputBuffers.size() != 1)
        {
            //Need to throw an exception here
        }

       switch(inputBuffers[0]->getTypeIdentifier())
        {
        case 0:
            writeOutput<uint8_t>();
            break;
        case 1:
            writeOutput<uint16_t>();
            break;
        case 2:
            writeOutput<uint32_t>();
            break;
        case 3:
            writeOutput<uint64_t>();
            break;
        case 4:
            writeOutput<int8_t>();
            break;
        case 5:
            writeOutput<int16_t>();
            break;
        case 6:
            writeOutput<int32_t>();
            break;
        case 7:
            writeOutput<int64_t>();
            break;
        case 8:
            writeOutput<float>();
            break;
        case 9:
            writeOutput<double>();
            break;
        case 10:
            writeOutput<long double>();
            break;
        case 11:
            writeOutput< complex<float> >();
            break;
        case 12:
            writeOutput< complex<double> >();
            break;
        case 13:
            writeOutput<complex< long double> >();
            break;
        default:
            break;
        }
    }

    template<typename T>
    void UdpSocketTxComponent::writeOutput()
    {
        //Get a read buffer
        ReadBuffer<T>* inBuf = castToType<T>(inputBuffers[0]);
        DataSet<T>* readDataSet = NULL;
        inBuf->getReadData(readDataSet);

        try
	    {
            d_socket->send_to(boost::asio::buffer(readDataSet->data), *d_endPoint);
	    }
	    catch (boost::system::system_error &e)
	    {
            LOG(LERROR) << "An error occurred while sending data to " << x_address << ", port " << x_port << \
                ": " << e.what();
	    }

        inBuf->releaseReadData(readDataSet);
    }

	UdpSocketTxComponent::~UdpSocketTxComponent()
	{
		try{
            d_socket->shutdown(udp::socket::shutdown_send);
            d_socket->close();

        }catch (boost::system::system_error &e)
	    {
            LOG(LERROR) << "An error occurred closing socket: " << e.what();
	    }
        delete d_socket;
        delete d_endPoint;
	}


} /* namespace iris */
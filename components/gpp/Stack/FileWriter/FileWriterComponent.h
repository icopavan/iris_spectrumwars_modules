/**
 * @file FileWriterComponent.h
 * @version 1.0
 *
 * @section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * @section LICENSE
 *
 * This file is part of the Iris Project.
 *
 * Iris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Iris is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 * @section DESCRIPTION
 *
 * A sink stack component which writes data to file.
 */

#ifndef FILEWRITERCOMPONENT_H_
#define FILEWRITERCOMPONENT_H_

#include <fstream>

#include "irisapi/StackComponent.h"

namespace iris
{

class FileWriterComponent: public StackComponent
{
private:
	//Exposed parameters
    std::string x_fileName;
	bool x_fromBelow;

    std::ofstream hOutFile;
	void writeBlock(boost::shared_ptr<StackDataSet> toWrite);

public:
    FileWriterComponent(std::string name);
    virtual void initialize();
    virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
	virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);
};

} /* namespace iris */

#endif /* FILEWRITERCOMPONENT_H_ */

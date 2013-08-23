/**
 * \file controllers/SpectrumWarsDisplay/test/SpectrumWarsDisplayController_test.cpp
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * \section LICENSE
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
 * \section DESCRIPTION
 *
 * Main test file for SpectrumWarsDisplay controller.
 */

#define BOOST_TEST_MODULE SpectrumWarsDisplayController_Test

#include <boost/test/unit_test.hpp>
#include <qapplication.h>
#include "../SpectrumWarsDisplayController.h"

#define PI     3.14159265358979323846

using namespace std;
using namespace iris;

BOOST_AUTO_TEST_SUITE (SpectrumWarsDisplayController_Test)

BOOST_AUTO_TEST_CASE(SpectrumWarsDisplayController_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(SpectrumWarsDisplayController c);
}

BOOST_AUTO_TEST_CASE(SpectrumWarsDisplayController_Parm_Test)
{
  SpectrumWarsDisplayController c;
  BOOST_CHECK(c.getParameterDefaultValue("spectrogramcomponent") == "spectrogram1");
  BOOST_CHECK(c.getParameterDefaultValue("numrows") == "512");
}

BOOST_AUTO_TEST_CASE(SpectrumWarsDisplayController_Init_Test)
{
  SpectrumWarsDisplayController c;
  c.initialize();
}

void threadMain1()
{
  SpectrumWarsDisplayController c;
  c.initialize();

  int n=512;
  float step = 2.0*PI/n;
  float* data = new float[n*2];
  for(int i=0;i<n*2;i++)
    data[i] = sinf(step*i);

  for(int i=0;i<n;i++)
  {
    Event e;
    e.data.assign(data+i, data+i+n);
    c.processEvent(e);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }
}

BOOST_AUTO_TEST_CASE(SpectrumWarsDisplayController_Process_Test)
{
  int argc = 1;
  char* argv[] = { const_cast<char *>("SpectrumWarsDisplayController_Process_Test"), NULL };
  QApplication a(argc, argv);

  boost::scoped_ptr< boost::thread > thread1_;
  thread1_.reset( new boost::thread( &threadMain1 ) );
  qApp->exec();
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
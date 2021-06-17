// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Julianus Pfeuffer $
// $Authors: Julianus Pfeuffer $
// --------------------------------------------------------------------------


/**

  Most of the tests, generously provided by the BALL people, taken from version 1.2

*/

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

// OpenMP support
#ifdef _OPENMP
    #include <omp.h>
#endif
#include <limits>

///////////////////////////

using namespace OpenMS;
//using namespace Logger;
using namespace std;


START_TEST(Multithreading, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION((OpenMP test))
{
  int wanted_threads = 1;
  int threads = 1;
  #ifdef _OPENMP
  wanted_threads = 2;
  omp_set_dynamic(0);  // Explicitly disable dynamic teams
  // Use 2 threads for all consecutive parallel regions
  omp_set_num_threads(wanted_threads);
  threads = omp_get_max_threads();
  #endif

  int max = INT_MIN;
  int i = 0;
  #pragma omp parallel private(i)
  {
    int maxi = INT_MIN;

    #pragma omp for
    for (i = 0; i < 10; i++)
    {
      #ifdef _OPENMP
      int threadnum = omp_get_thread_num() + 1;
      #else
      int threadnum = 1;
      #endif

      if (threadnum > maxi)
        maxi = threadnum;
    }

    #pragma omp critical
    {
      if (maxi > max)
        max = maxi;
    }
  }

  TEST_EQUAL(threads, wanted_threads)
  TEST_EQUAL(max, wanted_threads)
}
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

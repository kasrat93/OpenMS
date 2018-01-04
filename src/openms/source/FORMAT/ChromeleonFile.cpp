// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2017.
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
// $Maintainer: Douglas McCloskey, Pasquale Domenico Colaianni $
// $Authors: Douglas McCloskey, Pasquale Domenico Colaianni $
// --------------------------------------------------------------------------

#include <OpenMS/FORMAT/ChromeleonFile.h>

namespace OpenMS
{
  ChromeleonFile::ChromeleonFile() {}

  ChromeleonFile::~ChromeleonFile() {}

  void ChromeleonFile::load(const String& filename, MSExperiment& experiment) const
  {
    std::ifstream ifs(filename, std::ifstream::in);
    if (!ifs.is_open())
    {
      throw Exception::FileNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, filename);
    }
    const Size BUFSIZE = 65536;
    char line[BUFSIZE];
    experiment.clear(true);
    MSChromatogram chromatogram;
    std::cmatch m;
    std::regex re_injection("^Injection\t(.+)", std::regex::optimize);
    std::regex re_processing_method("^Processing Method\t(.+)", std::regex::optimize);
    std::regex re_instrument_method("^Instrument Method\t(.+)", std::regex::optimize);
    std::regex re_injection_date("^Injection Date\t(.+)", std::regex::optimize);
    std::regex re_injection_time("^Injection Time\t(.+)", std::regex::optimize);
    std::regex re_detector("^Detector\t(.+)", std::regex::optimize);
    std::regex re_signal_quantity("^Signal Quantity\t(.+)", std::regex::optimize);
    std::regex re_signal_unit("^Signal Unit\t(.+)", std::regex::optimize);
    std::regex re_signal_info("^Signal Info\t(.+)", std::regex::optimize);
    std::regex re_raw_data("^Raw Data:", std::regex::optimize);
    while (!ifs.eof())
    {
      ifs.getline(line, BUFSIZE);
      if (std::regex_search(line, m, re_injection))
      {
        std::cout << std::endl << "re_injection START" << std::endl;
        for (Size i = 0; i < m.size(); ++i)
        {
          std::cout << "[" << i << "] " << m[i] << std::endl;
        }
        std::cout << "re_injection END" << std::endl;
        experiment.setMetaValue("mzml_id", std::string(m[1]));
      }
      else if (std::regex_search(line, m, re_processing_method))
      {
        // TODO save this info somewhere
      }
      else if (std::regex_search(line, m, re_instrument_method))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_injection_date))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_injection_time))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_detector))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_signal_quantity))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_signal_unit))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_signal_info))
      {
        // save this info somewhere
      }
      else if (std::regex_search(line, m, re_raw_data))
      {
        ifs.getline(line, BUFSIZE); // remove the subsequent line, right before the raw data
        break;
      }
    }
    while (!ifs.eof())
    {
      ifs.getline(line, BUFSIZE);
      double rt, intensity;
      int ret = std::sscanf(line, "%lf\t%*f\t%lf", &rt, &intensity);
      if (ret == 2)
      {
        chromatogram.push_back(ChromatogramPeak(rt, intensity));
      }
      else if (!strcmp(line, "\r") || !strcmp(line, ""))
      {
        continue; // skips eventual empty lines, eg. the last before EOF
      }
      else
      {
        throw Exception::ParseError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, std::string(line), "Couldn't parse the raw data.");
      }
    }
    ifs.close();
    experiment.addChromatogram(chromatogram);
  }
}

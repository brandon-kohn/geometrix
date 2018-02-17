//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#include "scope_timer.hpp"
#include <boost/format.hpp>
#include <fstream>

namespace geometrix {
	namespace scope_timer_detail
	{
		inline call_map& call_map::instance()
		{
			static call_map theMap;
			return theMap;
		}

		inline call_map::~call_map()
		{
			write();
		}
		
		inline void call_map::write() const
		{
			if (!empty())
			{
			    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				std::string outputFile = str(boost::format("geometrix_scope_timer_timings_%1%.csv") % timestamp);
				std::ofstream ofs(outputFile.c_str());
				ofs << "Function Name,Counts,Total Time(s),Mean Time(s),Std.Dev(s)" << std::endl;
				
                const double conv = 1.0e-9;
				for (const auto& item : *this)
				{
                    auto& stat = item.second;
					auto counts = stat.counts();
					auto totalTime = stat.sum() * conv;
					double avgTime = stat.mean() * conv;
                    double stdDev = stat.standard_deviation() * conv;
					ofs << "\"" << item.first << "\"," << counts << "," << totalTime << "," << avgTime << "," << stdDev << std::endl;
				}
				ofs.flush();
				ofs.close();
			}
		}

	}//! namespace scope_timer_detail;
}//! namespace geometrix;



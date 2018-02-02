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
				ofs << "Function Name,Counts,Total Time,Average Time" << std::endl;
				
				for (const auto& item : *this)
				{
					auto counts = item.second.counts;
					auto totalTime = item.second.time / 1.0e9;
					double avgTime = totalTime / static_cast<double>(counts);
					ofs << "\"" << item.first << "\"," << counts << "," << totalTime << "," << avgTime << std::endl;
				}
				ofs.flush();
				ofs.close();
			}
		}

	}//! namespace scope_timer_detail;
}//! namespace geometrix;



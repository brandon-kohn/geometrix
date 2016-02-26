#include "scope_timer.hpp"

#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>

#include <cassert>

using namespace utility;
using namespace utility::detail;

/////////////////////////////////////////////////////////////////////////////
//
// Anonymous Namespace
//
/////////////////////////////////////////////////////////////////////////////
namespace
{
    //! Function to perform a timing (lowish? resolution timing) via boost.    
    template <typename TimePoint>
    double seconds_elapsed(const TimePoint& t1, const TimePoint& t2)
    {
        typedef boost::chrono::duration<double> sec;
        sec seconds(t2 - t1);
        return seconds.count();
    }
}

scoped_timer::scoped_timer( const std::string& functionName )        
: m_function( functionName )
, t1(boost::chrono::high_resolution_clock::now())
{
    call_stack& callStack = call_stack::instance();

    m_ancestorFunction = ( ( callStack.empty() ) ? "" : callStack.top() );
    callStack.push( functionName );///update stack.  
}

scoped_timer::~scoped_timer()
{
    t2 = (boost::chrono::high_resolution_clock::now());
    double seconds = seconds_elapsed( t1, t2 );

    BOOST_ASSERT( call_stack::instance().top() == m_function );
    call_map& callMap = call_map::instance();

    ///correct the parent time.
    if( m_ancestorFunction != "" )
    {
        call_data& parentData = callMap[m_ancestorFunction];
        parentData.selfTime -= seconds;///leave the overhead in this- it will have already been subtracted by the parent for the one time.
    }
    
    //update stack    
    call_stack::instance().pop();
   
    call_data& data = callMap[m_function];
    data.time += ( seconds /*- gTimeOverhead*/ );
    data.selfTime += ( seconds /*- gTimeOverhead*/ );
    data.data.push_back( seconds );
    data.counts += 1;
}

call_map::call_map()
{

}

call_map::~call_map()
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time(); //use the clock     
    std::string outputFile = str( boost::format( "function_timings_%1%.csv" ) % boost::posix_time::to_iso_string( now ) );
    std::ofstream ofs( outputFile.c_str() );
    ofs << "Function Name,Counts,Total Time (Self+Children),Average Time (Self+Children),Total Time (Self),Average Time (Self)" << std::endl;
    call_map& callMap = call_map::instance();
    BOOST_FOREACH( map_type::value_type& item, callMap )
    {
        std::size_t counts = item.second.counts;
        double totalTime = item.second.time;
        double avgTime = totalTime / boost::numeric_cast< double >( counts );

        double totalSelfTime = item.second.selfTime;
        double avgSelfTime = totalSelfTime / boost::numeric_cast< double >( counts );

        ofs << "\"" << item.first << "\"," << counts << "," << totalTime << "," << avgTime << "," << totalSelfTime << "," << avgSelfTime << std::endl;
    }

    BOOST_FOREACH( map_type::value_type& item, callMap )    
    {
        BOOST_FOREACH( double time, item.second.data )
        {
            ofs << "\"" << item.first << "\"," << time << std::endl;
        }
    }
}

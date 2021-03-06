#include "Logger.h"
#include <fstream>

namespace HEVC
{
	const LogId Logger::Off{ "Off" };
	const LogId Logger::Console{ "Console" };
	const LogId Logger::Dump{ "Dump" };
	const LogId Logger::ForwardTransform{ "ForwardTransform" };
	const LogId Logger::InverseTransform{ "InverseTransform" };
	const LogId Logger::Quant{ "Quant" };
	const LogId Logger::Dequant{ "Dequant" };
	const LogId Logger::LumaPrediction{ "LumaPrediction" };
	const LogId Logger::LumaReco{ "LumaReco" };
	const LogId Logger::ChromaPred{ "ChromaPred" };
	const LogId Logger::ChromaReco{ "ChromaReco" };
	const LogId Logger::DeblockingFilter{ "DeblockingFilter" };
	const LogId Logger::DeblockingFilterResult{ "DeblockingFilterResult" };
	const LogId Logger::RDO{ "RDO" };
	const LogId Logger::Binarization{ "Binarization" };
	const LogId Logger::BinarizationSummary{ "BinarizationSummary" };
	const LogId Logger::Prediction{ "Prediction" };
	const LogId Logger::Overview{ "Overview" };
	const LogId Logger::IO{ "Overview" };
	const LogId Logger::BinOut{ "BinOut" };
	const LogId Logger::Decoder{ "Decoder" };

	LoggingControl::LoggingControl( )
	{
		this->loadSettings( );
	}

	std::shared_ptr<Logger> LoggingControl::getLog( LogId logId )
	{
		return LoggingControl::instance( )._logs[ logId ];
	}

	void LoggingControl::turnOff( LogId logId )
	{
		_muteStack[ logId ]++;

		if( _muteStack[ logId ] > 0 && _logs[ logId ] != nullptr )
		{
			_swapBuffer[ logId ] = _logs[ logId ];
			_logs[ logId ] = nullptr;
		}
	}

	void LoggingControl::turnOn( LogId logId )
	{
		if( _muteStack[ logId ] > 0 )
		{
			_muteStack[ logId ]--;
			if( _muteStack[ logId ] == 0 && _swapBuffer[ logId ] != nullptr )
			{
				_logs[ logId ] = _swapBuffer[ logId ];
				_swapBuffer[ logId ] = nullptr;
			}
		}
	}

	LoggingControl::~LoggingControl( )
	{
	}

	void LoggingControl::loadSettings( )
	{
		_logs[ Logger::Console ] = createLog( Logger::Console, "" );
		_logs[ Logger::Dump ] = createLog( Logger::Dump, "" );

		_logs[ Logger::ForwardTransform ] = nullptr;//createLog(Logger::ForwardTransform, "D:\\HEVC\\ft.txt");
		_logs[ Logger::InverseTransform ] = nullptr;//createLog(Logger::InverseTransform, "D:\\HEVC\\it.txt");
		_logs[ Logger::Quant ] = nullptr;// createLog(Logger::Quant, "D:\\HEVC\\q.txt");
		_logs[ Logger::Dequant ] = nullptr;// createLog(Logger::Dequant, "D:\\HEVC\\iq.txt");
		_logs[ Logger::LumaPrediction ] = nullptr;// createLog(Logger::LumaPrediction, "D:\\HEVC\\lp.txt");
		_logs[ Logger::LumaReco ] = nullptr;// createLog(Logger::LumaReco, "D:\\HEVC\\lr.txt");
		_logs[ Logger::ChromaPred ] = createLog( Logger::ChromaPred, "D:\\HEVC\\cp.txt" );
		_logs[ Logger::ChromaReco ] = createLog( Logger::ChromaReco, "D:\\HEVC\\cr.txt" );
		_logs[ Logger::RDO ] = createLog( Logger::RDO, "D:\\HEVC\\rdo.txt" );
		_logs[ Logger::Binarization ] = createLog( Logger::Binarization, "D:\\HEVC\\ec.txt" );
		_logs[ Logger::BinarizationSummary ] = createLog( Logger::BinarizationSummary, "D:\\HEVC\\ec_summary.txt" );
		_logs[ Logger::DeblockingFilter ] = createLog( Logger::DeblockingFilter, "D:\\HEVC\\fdb.txt" );
		_logs[ Logger::DeblockingFilterResult ] = createLog( Logger::DeblockingFilterResult, "D:\\HEVC\\HMfdbres.txt" );
		_logs[ Logger::Prediction ] = createLog( Logger::Prediction, "D:\\HEVC\\predykcja.txt" );
		_logs[ Logger::Overview ] = nullptr;// createLog(Logger::Overview, "D:\\HEVC\\overview.txt");
		_logs[ Logger::IO ] = nullptr;//createLog(Logger::IO, "D:\\HEVC\\IO.txt");
		_logs[ Logger::BinOut ] = createLog( Logger::BinOut, "D:\\HEVC\\binout.txt" );
		_logs[ Logger::Decoder ] = createLog( Logger::Decoder, "D:\\HEVC\\decoder.txt" );
	}

	std::shared_ptr<Logger> LoggingControl::createLog( LogId logId, std::string path )
	{
		if( logId == Logger::Off )
			return nullptr;
		if( logId == Logger::Console )
			return Logger::createConsoleLogger( );
		if( logId == Logger::Dump )
			return Logger::createFileLogger( "D:\\dump.txt" );
		return Logger::createFileLogger( path );
	}

	void Indent::tab( LogId key )
	{
		auto log = LoggingControl::instance( ).getLog( key );
		if( log == nullptr )
			return;
		log->printSpaces( );
		log->getStream( ) << delimeters[0] << std::endl;
		log->increaseSpaces( );
	}

	void Indent::untab( LogId key )
	{
		auto log = LoggingControl::instance( ).getLog( key );
		if( log == nullptr )
			return;
		log->decreaseSpaces( );
		log->printSpaces( );
		log->getStream( ) << delimeters.substr(1) << std::endl;
	}

	Indent::Indent( LogId key ):
		_logId( key )
	{
		auto log = LoggingControl::instance( ).getLog( key );
		if( log == nullptr )
			return;
		tab( key );
	}

	Indent::Indent( LogId key, std::string custom_delimeters ):
		_logId( key ),
		delimeters(custom_delimeters )
	{
		auto log = LoggingControl::instance( ).getLog( key );
	if( log == nullptr )
		return;
	tab( key );
	}

	Indent::Indent( const char* func, LogId key ):
		_logId( key )
	{
		auto log = LoggingControl::instance( ).getLog( key );
		if( log == nullptr )
			return;
		LOGLN( key, func );
		tab( key );
	}

	Indent::~Indent( )
	{
		auto log = LoggingControl::instance( ).getLog( _logId );
		if( log == nullptr )
			return;
		untab( _logId );
	}

	Mute::Mute( LogId inId ):
		_logId( inId )
	{
		LOG_OFF( inId );
	}

	Mute::~Mute( )
	{
		LOG_ON( _logId );
	}

	Logger::Logger( std::string logPath ):
		_logPath( logPath ),
		_numTabs( 0 ),
		_step( 2 ),
		_spaces( "" ),
		_shouldDeleteStream( true )
	{
		_logStream = new std::ofstream( logPath, std::fstream::out | std::fstream::ate );
	}

	Logger::Logger( ):
		_logPath( "" ),
		_numTabs( 0 ),
		_step( 2 ),
		_spaces( "" ),
		_shouldDeleteStream( false )
	{
		_logStream = &std::cout;
	}

	std::shared_ptr<Logger> Logger::createFileLogger( std::string logPath )
	{
		return std::make_shared<Logger>( logPath );
	}

	std::shared_ptr<Logger> Logger::createConsoleLogger( )
	{
		return std::make_shared<Logger>( );
	}

	void Logger::printSpaces( )
	{
		for( int i = 0; i < _numTabs; ++i )
			*_logStream << "  ";
	}

	Logger::~Logger( )
	{
		if( _shouldDeleteStream )
		{
			_logStream->flush( );
			delete _logStream;
		}
		_logStream = nullptr;
	}

	void Logger::increaseSpaces( )
	{
		setTabLength( _numTabs + 1 );
	}

	void Logger::decreaseSpaces( )
	{
		setTabLength( _numTabs - 1 );
	}

	void Logger::setTabLength( int len )
	{
		if( len >= 0 )
		{
			_numTabs = len;
		}
		else
			setTabLength( 0 );
	}

	int Logger::getTabLength( )
	{
		return _numTabs;
	}

	void Logger::setTabStep( int len )
	{
		_step = len;
		setTabLength( _numTabs );
	}

	std::ostream& Logger::getStream( )
	{
		return *_logStream;
	}
}
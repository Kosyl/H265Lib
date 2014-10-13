/**
 * @file	Logger.cpp
 *
 * @brief	Implementacja klas loguj¹cych
 */

#include "Logger.h"

Logger::Logger( )
{
	Logger( "D:\\txt\\default_log.txt", false );
}

/** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
*	\param logPath sciezka do pliku logu
\param isLogging pisze log, jesli true
*/
Logger::Logger( std::string logPath, Bool isLogging ) :
includeDetails( true ),
m_loggingOn( isLogging ),
m_printSpaces( true ),
m_logPath( logPath ),
m_numTabs( 0 ),
m_step( 2 ),
m_spaces( "" )
{
	if( isLogging )
	{
		m_logStream.open( logPath, std::fstream::out | std::fstream::ate );
	}
}

Logger::~Logger( )
{
	if( m_logStream.is_open( ) )
	{
		m_logStream.flush( );
		m_logStream.close( );
	}
}

/** Wlaczenie/wylaczenie logowania
*	\param log true, jesli ma logowac; false, jesli nie
*	\return void
*/

Void Logger::printSpaces( )
{
	m_printSpaces = false;
	m_logStream << m_spaces;
}

void Logger::setLogging( Bool log )
{
	if( log )
	{
		if( !m_logStream.is_open( ) )
		{
			m_logStream.open( m_logPath, std::fstream::out | std::fstream::ate );
		}
	}
	else
	{
		m_logStream.flush( );
		m_logStream.close( );
	}
}

Void Logger::changeLogPath( std::string path )
{
	if( m_logStream.is_open( ) )
	{
		m_logStream.flush( );
		m_logStream.close( );
	}

	m_logPath = path;
	m_logStream.open( path, std::fstream::out | std::fstream::ate );

	return;
}

Void Logger::setTabLength( UInt len )
{
	if( len >= 0 )
	{
		m_spaces = "";
		for( UInt i = 0; i < len*m_step; ++i )
		{
			m_spaces = m_spaces.append( " " );
		}

		m_numTabs = len;
	}
	else
		setTabLength( 0 );
}

Void Logger::setTabStep( UInt len )
{
	m_step = len;
	setTabLength( m_numTabs );
}

Void Logger::increaseSpaces( )
{
	setTabLength( m_numTabs + 1 );
}

Void Logger::decreaseSpaces( )
{
	setTabLength( m_numTabs - 1 );
}

Logger& operator<<( Logger& o, std::ostream& ( *f )( std::ostream& ) )
{
	if( LoggingControl::validLog )
	if( o.isLogging( ) )
	{
		o.write( "\n" );
	}
	return o;
}

std::ofstream& Logger::getStream( )
{
	return this->m_logStream;
}

//////////////////////////////////////

LoggingControl* LoggingControl::instance = nullptr;
std::string LoggingControl::lastKey = "";
std::string LoggingControl::mainSettingsPath = "D:\\txt\\logging.cfg";
Bool LoggingControl::validLog = true;

LoggingControl::LoggingControl( )
{
	this->LoadSettings( );
}

LoggingControl* LoggingControl::getInstance( )
{
	if( LoggingControl::instance == nullptr )
		LoggingControl::instance = new LoggingControl( );
	return LoggingControl::instance;
}

LoggingControl::~LoggingControl( )
{
	if( logNames.size( ) > 0 )
	{
		Logger* osptr = nullptr;
		for( UInt i = 0; i < ( logNames.size( ) ); ++i )
		{
			osptr = logs[ logNames[ i ] ];
			if( osptr != nullptr )
			{
				delete osptr;
				osptr = nullptr;
			}
		}

		logs.clear( );
	}
}

void LoggingControl::LoadSettings( )
{
	std::ifstream cfg( LoggingControl::mainSettingsPath );

	std::string s, key, log, value;
	bool isLog = false;

	while( std::getline( cfg, s ) )
	{
		std::string::size_type begin = s.find_first_not_of( " \f\t\v" );

		if( begin == std::string::npos ) continue;

		std::string::size_type end = s.find( '=', begin );
		key = s.substr( begin, end - begin );

		key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

		if( key.empty( ) ) continue;

		std::string::size_type dwukropek = s.find( ':', begin );
		log = s.substr( begin, dwukropek - begin );

		log.erase( log.find_last_not_of( " \f\t\v" ) + 1 );
		if( log == "log" )
		{
			isLog = true;
			key = s.substr( dwukropek + 1, end - dwukropek - 1 );
			key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );
		}
		else
			isLog = false;

		begin = s.find_first_not_of( " \f\n\r\t\v", end + 1 );
		end = s.find_last_not_of( " \f\n\r\t\v" ) + 1;

		value = s.substr( begin, end - begin );

		if( isLog )
		{
			/*std::ofstream* stream = new std::ofstream( );
			stream->open( value, std::fstream::out | std::fstream::ate );
			logNames.push_back( key );
			logs[ key ] = stream;*/
			std::cout << "nowy strumien logownaia: " << value << std::endl;
			Logger* log = new Logger( value, true );
			logNames.push_back( key );
			logs[ key ] = log;
		}
		else
		{
			std::cout << "flaga " << key << " = " << value << std::endl;
			triggers[ key ] = value == "1" ? true : false;
		}
	}
}

Logger& LOG( std::string key )
{
	LoggingControl::lastKey = key;
	if( LoggingControl::getInstance( )->triggers.find( key + "_ON" ) != LoggingControl::getInstance( )->triggers.end( ) && LoggingControl::getInstance( )->triggers[ key + "_ON" ] )
	{
		LoggingControl::validLog = true;
	}
	else
	{
		LoggingControl::validLog = false;
	}
	return *( LoggingControl::getInstance( )->logs[ key ] );
}

Void LOG_TAB( std::string key )
{
	if( key == "" )
		key = LoggingControl::lastKey;
	if( LoggingControl::getInstance( )->triggers.find( key + "_ON" ) != LoggingControl::getInstance( )->triggers.end( ) && LoggingControl::getInstance( )->triggers[ key + "_ON" ] )
	{
		LOG( key ) << "{" << std::endl;
		LoggingControl::getInstance( )->logs[ key ]->increaseSpaces( );
	}
}

Void LOG_UNTAB( std::string key )
{
	if( key == "" )
		key = LoggingControl::lastKey;
	if( LoggingControl::getInstance( )->triggers.find( key + "_ON" ) != LoggingControl::getInstance( )->triggers.end( ) && LoggingControl::getInstance( )->triggers[ key + "_ON" ] )
	{
		LoggingControl::getInstance( )->logs[ key ]->decreaseSpaces( );
		LOG( key ) << "}" << std::endl;
	}
}
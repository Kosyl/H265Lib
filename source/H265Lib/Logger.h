/**
* @file	Logger.h
*
* @brief	Deklaracje klas logowania
*/

#ifndef _LOGGER_
#define _LOGGER_

#include "TypeDef.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <map>

/**
 * @class	Logger
 *
 * @brief	A logger.
 */

class Logger
{
private:

	/**
	 * @brief	Sciezka do pliku logu.
	 */

	std::string m_logPath;

	/**
	 * @brief	Strumien do pliku logu.
	 */

	std::ofstream m_logStream;

	/**
	 * @brief	Flaga w³¹cza/wy³¹cza wypisywanie tabulacji.
	 */

	Bool m_printSpaces;

	/**
	 * @brief	Iloœæ tabulacji
	 */

	UInt m_numTabs;

	/**
	 * @brief	Iloœæ spacji/tabulacja.
	 */

	UInt m_step;

	/**
	 * @brief	String pojedynczej tabulacji.
	 */

	std::string m_spaces;

	/**
	 * @brief	Wlacza/wylacza log.
	 */

	Bool m_loggingOn;

	/**
	 * @struct	identity
	 *
	 * @brief	Struktura-klucz, ma za zadanie jedynie wybraæ odpowiedni¹ funkcjê poprzez okreœlenie typu dla zmiennej T.
	 *
	 * @tparam	T	Typ struktury
	 */

	template<typename T>
	struct identity
	{
		typedef T type;
	};

	/**
	 * @fn	Void Logger::emptyBuffer( );
	 *
	 * @brief	Wyczyszczenie bufora zapisu).
	 *
	 * @return	Void.
	 */

	Void emptyBuffer( );

	/**
	 * @fn	Void Logger::printSpaces( );
	 *
	 * @brief	Wypisuje aktualn¹ liczbê tabulacji.
	 *
	 * @return	Void.
	 */

	Void printSpaces( );

	/**
	 * @fn	template<typename T> Void Logger::write( T s, identity<T> i );
	 *
	 * @brief	Ogólna funkcja zapisuj¹ca:
	 *
	 * @tparam	T	Obiekt do zapisu do strumienia.
	 * @param	s	Obiekt typu T do zapisu do strumienia.
	 * @param	i	pomocnicza struktura-klucz.
	 *
	 * @return	Void.
	 */

	template<typename T>
	Void write( T s, identity<T> i );

	/**
	 * @fn	Void Logger::write( const Char* s, identity<const Char*> i )
	 *
	 * @brief	Specjalizacja metody write dla ci¹gów znaków.
	 *
	 * @param	s	Tablica znaków do wypisania.
	 * @param	i	strutkura-klucz wybieraj¹ca tê funkcjê.
	 *
	 * @return	Void.
	 */

	Void write( const Char* s, identity<const Char*> i )
	{
		std::string str( s );
		write( str );
	}

	/**
	 * @fn	Void Logger::write( std::string s, identity<std::string> i )
	 *
	 * @brief		Specjalizacja metody write dla klasy std::string.
	 *
	 * @param	s	std::string do wypisania
	 * @param	i	strutkura-klucz wybieraj¹ca tê funkcjê.
	 *
	 * @return	Void.
	 */

	Void write( std::string s, identity<std::string> i )
	{
		if( !s.compare( "\n" ) || s.find( "\n" ) != std::string::npos )
		{
			m_printSpaces = true;
			m_logStream << s;
			return;
		}
		else
		{
			if( m_printSpaces )
			{
				printSpaces( );
			}
			m_logStream << s;
		}
	}

public:

	/**
	 * @fn	Logger::Logger( );
	 *
	 * @brief	Konstrutor domyœlny.
	 */

	Logger( );

	/**
	 * @fn	Logger::Logger( std::string logPath, Bool isLogging );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param	logPath	 	Pe³na œcie¿ka do pliku logowania
	 * @param	isLogging	W³¹czenie/wy³¹czenie 
	 */

	Logger( std::string logPath, Bool isLogging );

	/**
	 * @fn	Logger::~Logger( );
	 *
	 * @brief	Destruktor.
	 */

	~Logger( );

	/**
	 * @brief	Czy dodaje parametry typu offsety, shifty, info o glebii bitowej itp.
	 */

	Bool includeDetails;

	/**
	 * @fn	template<typename T> friend Logger& Logger::operator<<( Logger& o, T const& t );
	 *
	 * @brief	Operator << pozwala traktowaæ obiekt Log jak zwyk³y strumieñ.
	 *
	 * @param [in]	o	Instancja logu do wypisania danych
	 * @param	t					 	The T const&amp; 0 to process.
	 *
	 * @return	Obiekt aktualnego logowania przez referencjê.
	 */

	template<typename T>
	friend Logger& operator<<( Logger& o, T const& t );

	/**
	 * @fn	Void Logger::increaseSpaces( );
	 *
	 * @brief	Wciêcie w logowaniu.
	 *
	 * @return	Void.
	 */

	Void increaseSpaces( );

	/**
	 * @fn	Void Logger::decreaseSpaces( );
	 *
	 * @brief	Decrease spaces.
	 *
	 * @return	Void.
	 */

	Void decreaseSpaces( );

	/**
	 * @fn	Logger& Logger::operator++( )
	 *
	 * @brief	Operator inkrementacji - logowanie przechodzi "poziom ni¿ej", tzn robi isê wciêcie.
	 *
	 * @return	referencja na aktualny obiekt Logger.
	 */

	Logger& operator++( )
	{
		this->increaseSpaces( );
		return *this;
	}

	/**
	 * @fn	Logger& Logger::operator--( )
	 *
	 * @brief	Operator dekrementacji.
	 *
	 * @return	referencja na aktualny obiekt Logger.
	 */

	Logger& operator--( )
	{
		this->decreaseSpaces( );
		return *this;
	}

	/**
	 * @fn	Void Logger::setLogging( Bool log );
	 *
	 * @brief	W³¹cza/wy³acza logowanie.
	 *
	 * @param	log	true, ¿eby w³¹czyæ.
	 *
	 * @return	Void.
	 */

	Void setLogging( Bool log );

	/**
	 * @fn	Void Logger::changeLogPath( std::string path );
	 *
	 * @brief Zmiana œcie¿ki logowania.
	 *
	 * @param	path	Œcie¿ka do pliku z wynikami.
	 *
	 * @return	A Void.
	 */

	Void changeLogPath( std::string path );

	/**
	 * @fn	Bool Logger::isLogging( )
	 *
	 * @brief	Sprawdzenie, czy nasz obiekt w ogóle przeprowadza logowanie.
	 *
	 * @return	Prawda, jeœli logowanie jest w³aczone; false w p.p.
	 */

	Bool isLogging( )
	{
		return m_loggingOn;
	}

	/**
	 * @fn	Void Logger::setTabLength( UInt len );
	 *
	 * @brief	Ustawienie ilosci tabulacji.
	 *
	 * @param	len	Wybrana liczba tabulacji.
	 *
	 * @return	Void.
	 */

	Void setTabLength( UInt len );

	/**
	 * @fn	Void Logger::setTabStep( UInt len );
	 *
	 * @brief	Ustawienie d³ugoœci tabulacji.
	 *
	 * @param	len	Wybrana d³ugoœæ tabulacji (w spacjach).
	 *
	 * @return	Void.
	 */

	Void setTabStep( UInt len );

	/**
	 * @fn	std::ofstream& Logger::getStream( );
	 *
	 * @brief	Dostêp do strumienia wyjœciowego.
	 *
	 * @return	Strumieñ wyjœciowy.
	 */

	std::ofstream& getStream( );

	/**
	 * @fn	template<typename T> Void Logger::write( T s );
	 *
	 * @brief	Ogólna funkcja zapisu do pliku.
	 *
	 * @tparam	T	Dowolna klasa/typ, które mog¹ zostaæ podane na strumieñ.
	 * @param	s	instancja T to zapisania
	 *
	 * @return	Void.
	 */

	template<typename T>
	Void write( T s );

};

#include "LoggerImpl.h"

/**
 * @fn	Logger& operator<<( Logger& o, std::ostream& ( *f )( std::ostream& ) );
 *
 * @brief	Operator zapisu do strumienia dla std::endl
 *
 * @param [in]	o	Logger&amp do którego zachodzi zapis.
 * @param [in]	f	Typ std::endl.
 *
 * @return	Logger&amp do dalszych zapisów.
 */

Logger& operator<<( Logger& o, std::ostream& ( *f )( std::ostream& ) );

/**
 * @class	LoggingControl
 *
 * @brief	A logging control.
 */

class LoggingControl
{
private:

	/**
	 * @brief	Instancja.
	 */

	static LoggingControl *instance;

	/**
	 * @brief	Lista nazw logów.
	 */

	std::vector<std::string> logNames;

	/**
	 * @fn	LoggingControl::LoggingControl( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	LoggingControl( );

	/**
	 * @fn	void LoggingControl::LoadSettings( );
	 *
	 * @brief	Wczytanie konfiguracji z pliku.
	 */

	void LoadSettings( );

public:

	/**
	 * @brief	Œcie¿ka do pliku z konfigutacj¹ optogramowania
	 */

	static std::string mainSettingsPath;

	/**
	 * @brief	Klucz ostatnio u¿ywanego logu.
	 */

	static std::string lastKey;

	/**
	 * @fn	static LoggingControl* LoggingControl::getInstance( );
	 *
	 * @brief	Dostêp do instancji kontroli logowania.
	 *
	 * @return	WskaŸnik na instancjê HMLoggingControl
	 */

	static LoggingControl* getInstance( );

	/**
	 * @fn	LoggingControl::~LoggingControl( );
	 *
	 * @brief	Destruktor.
	 */

	~LoggingControl( );

	//std::map<std::string, std::ofstream*> logs;

	/**
	 * @brief	Mapa Loggerów.
	 */

	std::map<std::string, Logger*> logs;

	/**
	 * @brief	Mapa boolowskich flag kontrolnych.
	 */

	std::map<std::string, bool> triggers;

	/**
	 * @brief	Fa³sz, jeœli podejmowana jest próba skorzystania z nieistniej¹cego logu.
	 */

	static bool validLog;
};

/**
 * @fn	Logger& LOG( std::string key );
 *
 * @brief	Zwraca z LoggingControl obiekt Logger o podaym kluczu.
 *
 * @param	key	Klucz logownania.
 *
 * @return	A Logger&amp;
 */

Logger& LOG( std::string key );

/**
 * @fn	Void LOG_TAB( std::string key = "" );
 *
 * @brief	Robi wciêcie w pliku odpowiadajacemu podanemu kluczowi.
 *
 * @param	key	Klucz obiektu logu.
 *
 * @return	Void.
 */

Void LOG_TAB( std::string key = "" );

/**
 * @fn	Void LOG_UNTAB( std::string key = "" );
 *
 * @brief	Usuwa jedno wciêcie w pliku z danego klucza.
 *
 * @param	key	Klucz obiektu logu.
 *
 * @return	Void.
 */

Void LOG_UNTAB( std::string key = "" );

#endif
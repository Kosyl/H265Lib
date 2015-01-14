
template<typename T>
Logger& operator<<( Logger& o, T const& t )
{
	if( o.isLogging( ) )
	{
		o.write( t );
	}
	return o;
}

template<typename T>
Logger& operator<<( Logger& o, T** const& t )
{
	if( o.isLogging( ) )
	{
		int size1 = sizeof( t ) / sizeof( T* );

		for( int i = 0; i < size1; ++i )
		{
			int size2 = sizeof( t[ i ] ) / sizeof( T );

			for( int j = 0; j < size2; ++j )
			{
				o << /*std::setw(4) << */ t[ j ][ i ] << " ";
			}
			o << std::endl;
		}
	}
	return o;
}

template<typename T>
Void Logger::write( T s )
{
	write( s, identity<T>( ) );
}

template<typename T>
Void Logger::write( T s, identity<T> i )
{
	if( m_printSpaces )
	{
		printSpaces( );
	}
	m_logStream << s;
}
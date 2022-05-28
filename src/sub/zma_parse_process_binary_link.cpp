// --------------------------------------------------------------------
//	Z80 Macro Assembler parse
// ====================================================================
//	2019/05/04	t.hara
// --------------------------------------------------------------------

#include "zma_parse.hpp"
#include "zma_text.hpp"
#include "zma_parse_process.hpp"
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// --------------------------------------------------------------------
bool CZMA_PARSE_BINARY_LINK::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	int i, count;
	unsigned char d;
	CVALUE path;

	update_flags( &info, p_last_line );
	if( words.size() < 2 ){
		put_error( "Must be set include file name." );
		return false;
	}
	i = this->expression( info, 1, path );
	if( i == 0 ){
		put_error( "Invalid expression." );
		return false;
	}
	if( path.value_type != CVALUE_TYPE::CV_STRING ){
		put_error( "Invalid parameter." );
		return false;
	}
	if( i < (int)words.size() ){
		put_error( "BINARY_LINK command has only one parameter." );
		return false;
	}
	if( !this->get_fixed_code_size() ) {
		std::ifstream file;
		file.open( path.s, std::ios::binary );
		if( !file ){
			put_error( "BINARY_LINK command has only one parameter." );
			return false;
		}
		file.seekg( 0, std::ifstream::end );
		count = static_cast<int>( file.tellg() );
		file.seekg( 0, std::ifstream::beg );
		this->set_code_size( &info, count );

		for( i = 0; i < count; i++ ){
			file.read( (char*) &d, 1 );
			data.push_back( d );
		}
		this->is_data_fixed = true;
	}
	//	log
	if( !this->is_analyze_phase ) {
		log.push_back( "[\t" + get_line() + "]" );
		this->log_data_dump();
		log.push_back( "" );
	}
	return check_all_fixed();
}

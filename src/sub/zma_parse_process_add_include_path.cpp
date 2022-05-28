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
bool CZMA_PARSE_ADD_INCLUDE_PATH::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	CVALUE v;
	bool is_loaded = false;

	set_code_size( &info, 0 );
	update_flags( &info, p_last_line );
	if( words.size() == 1 ) {
		put_error( "Path not found in ADD_INCLUDE_PATH." );
		return false;
	}
	if( this->expression( info, 1, v ) == 0 ) {
		put_error( "Illegal parameter in ADD_INCLUDE_PATH." );
		return false;
	}
	if( v.value_type == CVALUE_TYPE::CV_INTEGER ) {
		v.s = std::to_string( v.i );
	}
	for( auto s : info.include_path ) {
		if( s == v.s ) {
			is_loaded = true;
		}
	}
	if( !is_loaded ) {
		info.include_path.push_back( v.s );
	}
	//	log
	if( !this->is_analyze_phase ) {
		log.push_back( "[\t" + get_line() + "]" );
		log.push_back( "\tAdd include path: " + v.s );
	}
	return true;
}

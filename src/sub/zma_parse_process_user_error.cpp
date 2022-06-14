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
bool CZMA_PARSE_USER_ERROR::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	CVALUE v;

	set_code_size( &info, 0 );
	update_flags( &info, p_last_line );
	if( words.size() == 1 ) {
		put_error( "Too many parameters." );
		return false;
	}
	if( this->expression( info, 1, v, false ) == 0 ) {
		put_error( "Illegal parameter." );
	}
	else if( v.is_unknown() ){
		put_error( "Illegal parameter." );
	}
	else if( v.is_string() ){
		put_error( v.s );
	}
	else if( v.is_integer() ) {
		put_error( std::to_string( v.i ) );
	}
	else {
		put_error( "Illegal parameter." );
	}
	return false;
}

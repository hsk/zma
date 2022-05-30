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
bool CZMA_PARSE_SCOPE::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( words.size() < 2 ) {
		put_error( "Must be set scope name." );
		return false;
	}
	if( words.size() > 2 ) {
		put_error( "SCOPE command has only one parameter." );
		return false;
	}
	this->is_data_fixed = true;
	this->set_code_size( &info, 0 );
	info.scope.push_back( words[1] );

	//	log
	if( !is_analyze_phase ) {
		log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
		log.push_back( "\tScope path: " + info.get_scope_path() );
		log.write_separator();
	}
	return check_all_fixed();
}


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
bool CZMA_PARSE_LABEL::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	std::string label;
	CVALUE v;

	update_flags( &info, p_last_line );
	//	log
	if( !this->is_analyze_phase ) {
		std::stringstream s;
		s << "0x" << std::hex << std::setw( 6 ) << std::setfill( '0' ) << this->get_code_address();
		log.push_back( "[" + get_line() + "]" );
		log.push_back( "\tlabel address: " + s.str() );
		log.push_back( "" );
	}
	if( this->is_data_fixed ) {
		return check_all_fixed();
	}
	this->set_code_size( &info, 0 );
	update_flags( &info, p_last_line );
	if( words[0][0] == '\"' ) {
		put_error( "Label name cannot be string." );
		return false;
	}
	if( this->get_fixed_code_address() ) {
		label = info.get_scope_path() + words[0];
		if( info.dict.count( label ) ) {
			put_error( std::string("There are declarations of the same label '") + label + "' in multiple places." );
			return false;
		}
		else {
			this->is_data_fixed = true;
			v.value_type = CVALUE_TYPE::CV_INTEGER;
			v.i = this->get_code_address();
			info.dict[label] = v;
			info.is_updated = true;
		}
	}
	else {
		put_error( std::string( "Label '" ) + words[0] + "' is indeterminate." );
		return false;
	}
	return check_all_fixed();
}

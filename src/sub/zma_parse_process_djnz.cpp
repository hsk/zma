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
bool CZMA_PARSE_DJNZ::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	int relative;

	update_flags( &info, p_last_line );
	if( words.size() != 2 ) {
		put_error( "Illegal operand" );
		return false;
	}
	if( !this->is_data_fixed ) {
		this->set_code_size( &info, 2 );
		relative = this->relative_address( info, 1 );
		if( relative == -9999 ) {
			put_error( "Illegal operand" );
			return false;
		}
		this->is_data_fixed = true;
		this->data.push_back( 0x10 );
		this->data.push_back( relative );
	}
	//	log
	if( !this->is_analyze_phase ) {
		log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
		log.push_back( "[\t" + get_line() + "] Z80:14cyc(cond=true), 9cyc(cond=false), R800:3cyc(cond=true), 2cyc(cond=false)" );	//	DJNZ nn
		log.write_dump( this->code_address, this->file_address, this->data );
		log.write_separator();
	}
	return check_all_fixed();
}


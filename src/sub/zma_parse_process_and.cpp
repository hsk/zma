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
bool CZMA_PARSE_AND::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( this->opecode_a_sss( info, 0xA0 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( data.size() == 2 ) {
				log.push_back( "[\t" + get_line() + "] Z80:10cyc, R800:2cyc" );	//	AND A, IXh
			}
			else {
				log.push_back( "[\t" + get_line() + "] Z80:5cyc, R800:1cyc" );	//	AND A, r
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_a_n( info, 0xE6 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			log.push_back( "[\t" + get_line() + "] Z80:8cyc, R800:2cyc" );	//	AND A, n
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_a_ref_hl( info, 0xA6 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			if( words[4] == "HL" ) {
				log.push_back( "[\t" + get_line() + "] Z80:8cyc, R800:4cyc" );	//	AND A, [HL]
			}
			else {
				log.push_back( "[\t" + get_line() + "] Z80:21cyc, R800:7cyc" );	//	AND A, [IX+o]
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	put_error( "Illegal operand" );
	return false;
}


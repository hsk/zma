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
bool CZMA_PARSE_INC::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( this->opecode_ddd( info, 0x04 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( data.size() == 2 ) {
				log.write_cycle_information( 10, 2 );		//	INC IXh
			}
			else {
				log.write_cycle_information( 5, 1 );		//	INC r
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_rp( info, 0x03 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			if( words[1] == "IX" || words[1] == "IY" ) {
				log.write_cycle_information( 12, 1 );		//	INC IX
			}
			else {
				log.write_cycle_information( 7, 1 );		//	INC rr
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_ref_hl( info, 0x34 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			if( words[2] == "HL" ) {
				log.write_cycle_information( 12, 7 );		//	INC [HL]
			}
			else {
				log.write_cycle_information( 25, 10 );		//	INC [IX+o]
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	put_error( "Illegal operand" );
	return false;
}


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

#include "zma_parse_process_srl.hpp"

// --------------------------------------------------------------------
bool CZMA_PARSE_SRL::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( this->opecode_source8( info, 0xCB, 0x38 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( data.size() == 2 ) {
				if( this->data[1] == 0x3E ) {
					log.write_cycle_information( 17, 8 );		//	SRL [HL]
				}
				else {
					log.write_cycle_information( 10, 2 );		//	SRL	r
				}
			}
			else {
				log.write_cycle_information( 25, 10 );		//	SRL	[IX+d]
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::ILLEGAL_OPERAND ) );
	return false;
}


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

#include "zma_parse_process_cp.hpp"

// --------------------------------------------------------------------
bool CZMA_PARSE_CP::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( this->opecode_a_source8( info, 0xB8 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( data.size() == 2 ) {
				log.write_cycle_information( 10, 2 );	//	CP A, IXh
			}
			else {
				log.write_cycle_information( 5, 1 );	//	CP A, r
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_a_n8( info, 0xFE ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			log.write_cycle_information( 8, 2 );	//	CP A, n
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_a_memory_hl( info, 0xBE ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( words[4] == "HL" ) {
				log.write_cycle_information( 8, 4 );	//	CP A, [HL]
			}
			else {
				log.write_cycle_information( 21, 7 );	//	CP A, [IX+o]
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_source8( info, 0xB8 ) ){
		//	log
		if( !this->is_analyze_phase ){
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			if( data.size() == 2 ){
				log.write_cycle_information( 10, 2 );	//	CP IXh
			}
			else{
				log.write_cycle_information( 5, 1 );	//	CP r
			}
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	if( this->opecode_n8( info, 0xFE ) ){
		//	log
		if( !this->is_analyze_phase ){
			log.write_line_infomation( this->line_no, this->code_address, this->file_address, get_line() );
			log.write_cycle_information( 8, 2 );	//	CP n
			log.write_dump( this->code_address, this->file_address, this->data );
			log.write_separator();
		}
		return check_all_fixed();
	}
	put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::ILLEGAL_OPERAND ) );
	return false;
}


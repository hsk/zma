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
bool CZMA_PARSE_IN::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	update_flags( &info, p_last_line );
	if( opecode_ddd_c( info, 0xDB, 0xED, 0x40 ) ) {
		//	log
		if( !this->is_analyze_phase ) {
			if( data[0] == 0xDB ) {
				log.push_back( "[\t" + get_line() + "] Z80:12cyc, R800:10 or 9cyc" );		//	IN A,[n]
			}
			else {
				log.push_back( "[\t" + get_line() + "] Z80:14cyc, R800:10 or 9cyc" );		//	IN A,[C]
			}
			this->log_data_dump();
			log.push_back( "" );
		}
		return check_all_fixed();
	}
	put_error( "Illegal operand" );
	return false;
}


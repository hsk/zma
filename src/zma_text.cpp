// --------------------------------------------------------------------
//	ZMA: CZMA_TEXT
// ====================================================================
//	2019/05/02	t.hara
// --------------------------------------------------------------------

#include "zma_text.hpp"
#include <fstream>
#include <iostream>

// ----------------------------------------------------------------
//	Constructor
// ----------------------------------------------------------------
CZMA_TEXT::CZMA_TEXT() {
	code_size = 0;
	next_code_address = 0;
}

// ----------------------------------------------------------------
//	Destructor
// ----------------------------------------------------------------
CZMA_TEXT::~CZMA_TEXT() {
	for( auto p: m_text ) { 
		delete p; 
	}
}

// ----------------------------------------------------------------
//	File Load
// ----------------------------------------------------------------
bool CZMA_TEXT::load( CZMA_INFORMATION &info, const char* p_file_name ) {
	m_text.clear();
	std::string s;
	CZMA_PARSE *p_parse;
	int line_no;
	std::vector< std::string > words, labels;

	std::ifstream file;
	file.open( p_file_name );
	if( !file ) {
		return false;
	}
	line_no = 1;
	for( ;; line_no++) {
		if(file.bad()) {
			break;
		}
		if(!std::getline( file, s )) {
			break;
		}
		words = CZMA_PARSE::get_word_split( s );
		if( words.size() > 2 && ( (words[ 1 ] == ":") || (words[ 1 ] == "::") ) ){
			labels.resize( 2 );
			labels[ 0 ] = words[ 0 ];
			labels[ 1 ] = words[ 1 ];
			p_parse = CZMA_PARSE::create( info, labels, p_file_name, line_no );
			m_text.push_back( p_parse );
			words.erase( words.begin() );
			words.erase( words.begin() );
			p_parse = CZMA_PARSE::create( info, words, p_file_name, line_no );
			m_text.push_back( p_parse );
		}
		else{
			p_parse = CZMA_PARSE::create( info, words, p_file_name, line_no );
			m_text.push_back( p_parse );
		}
	}
	file.close();
	return true;
}

// --------------------------------------------------------------------
CZMA_PARSE *CZMA_TEXT::process( CZMA_INFORMATION &info, unsigned int &success_count, CZMA_PARSE *p_prev_line, bool output_mode ) {
	CZMA_PARSE* p_prev;
	bool result;
	int nest_count = 0;
	p_prev = p_prev_line;
	result = true;
	success_count = 0;
	std::vector<std::string> words;

	for( auto p = m_text.begin(); p != m_text.end(); ) {
		if( output_mode ) {
			(*p)->set_output_mode();
		}
		if( !info.is_block_processing ){
			//	パースエラーを起こした行の場合、再パースを試みる
			if( ( *p )->is_parse_error() ){
				words = CZMA_PARSE::get_word_split( ( *p )->get_line() );
				CZMA_PARSE *p_parse = CZMA_PARSE::create( info, words, ( *p )->get_file_name(), ( *p )->get_line_no() );
				if( !( p_parse->is_parse_error() ) ){
					p = m_text.erase( p );
					p = m_text.insert( p, p_parse );
				}
				else{
					delete p_parse;
				}
			}
			//	ブロックの外側の処理
			if( ( *p )->process( info, p_prev ) ){
				success_count++;
			}
			p_prev = ( *p );
			p++;
		}
		else {
			//	ブロックの中の処理
			if( (*p)->words.size() >= 1 && info.block_end_table.count( (*p)->words[0] ) ) {
				//	ブロックを閉じる記号を発見
				if( (*p)->words[0] == "ENDR" || (*p)->words[0] == "ENDM" || (*p)->words[0] == "ENDIF" ) {
					//	特に ENDR, ENDM, ENDIF の処理
					if( nest_count ) {
						//	ブロック内のブロックだったのでネスト数を減らすだけ
						nest_count--;
						info.p_text->push_back( *p );
						p = m_text.erase( p );
						continue;
					}
				}
				if( nest_count == 0 ) {
					if( info.block_type != info.block_end_table[(*p)->words[0]] ) {
						//	着目ブロックの種類と異なる「閉じる記号」を発見
						(*p)->put_structure_error( "Invalid block pair." );
						p_prev = (*p);
						p++;
						continue;
					}
					//	「閉じる記号」の処理
					if( (*p)->process( info, p_prev ) ) {
						success_count++;
					}
					p_prev = (*p);
					p++;
				}
				else {
					info.p_text->push_back( *p );
					p = m_text.erase( p );
					continue;
				}
			}
			else {
				if( (*p)->words.size() >= 1 && info.block_begin_table.count( (*p)->words[0] ) ) {
					//	ブロックの内側にブロックの開始を発見
					if( (*p)->words[0] == "REPEAT" || (*p)->words[0] == "IF" ) {
						nest_count++;
					}
				}
				else if( (*p)->words.size() >= 2 && (*p)->words[1] == "MACRO" ) {
					//	ブロックの内側にマクロ宣言を発見
					(*p)->put_structure_error( "MACRO cannot define in MACRO/REPEAT/IF block." );
					p_prev = (*p);
					p++;
					continue;
				}
				info.p_text->push_back( *p );
				p = m_text.erase( p );
			}
		}
	}
	return p_prev;
}

// --------------------------------------------------------------------
bool CZMA_TEXT::all_process( CZMA_INFORMATION& info ) {
	unsigned int success_count;
	CZMA_PARSE* p_last_line;

	for( ; ; ) {
		info.clear();
		p_last_line = this->process( info, success_count, nullptr, false );
		if( info.is_block_processing ) {
			p_last_line->set_output_mode();
			p_last_line->put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::BLOCK_PROCESSING_IS_NOT_CLOSED ) );
			break;
		}
		if( !info.is_updated ) {
			break;
		}
	}
	info.clear();
	p_last_line = this->process( info, success_count, nullptr, true );
	if( p_last_line == nullptr || ((p_last_line->get_file_address() == 0) && (p_last_line->get_code_size() == 0)) ) {
		std::cerr << "Code is not found.\n";
		return false;
	}
	if( info.scope.size() > 0 ) {
		p_last_line->put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::SCOPE_IS_NOT_CLOSED ) );
		return false;
	}
	if( CZMA_PARSE::get_number_of_errors() ) {
		std::cerr << "Found " << CZMA_PARSE::get_number_of_errors() << " error(s).\n";
		return false;
	}
	return true;
}

// --------------------------------------------------------------------
bool CZMA_TEXT::write( CZMA_INFORMATION& info, std::ofstream* f ) {
	bool result;

	result = true;
	for( auto &p: m_text ) {
		result = result && p->write_output_and_log( info, f );
	}
	if( info.output_type == CZMA_INFORMATION::OUTPUT_TYPE::CZMA_INTELHEX ){
		info.hexfile.flush( *f );
	}
	return result;
}

// --------------------------------------------------------------------
bool CZMA_TEXT::save( CZMA_INFORMATION& info, const char* p_file_name ) {
	std::ofstream f;

	f.open( p_file_name, std::ios::out | std::ios::binary );
	bool result = this->write( info, &f );
	info.write();
	return result;
}

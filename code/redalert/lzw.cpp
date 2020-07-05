//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header: /CounterStrike/LZW.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : LZW.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 08/28/96                                                     *
 *                                                                                             *
 *                  Last Update : August 28, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Find_Child_Node -- Find a matching dictionary entry.                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include	"xstraw.h"
#include	"xpipe.h"
#include	"buff.h"
#include	"lzw.h"


LZWEngine::LZWEngine(void)
{
	Reset();
}


void LZWEngine::Reset(void)
{
	for (int i = 0; i < TABLE_SIZE; i++) {
		dict[i].Make_Unused();
	}
}

int LZWEngine::Compress(Buffer const & input, Buffer const & output)
{
	BufferStraw instraw(input);
	BufferPipe outpipe(output);

	int outcount = 0;
	CodeType string_code = END_OF_STREAM;
	CodeType next_code = FIRST_CODE;

	string_code = 0;
	if (instraw.Get(&string_code, sizeof(char)) == 0) {
		string_code = END_OF_STREAM;
	}

	for (;;) {

		/*
		**	Fetch a character from the source data stream. If exhausted,
		**	then break out of the process loop so that the final code
		**	can be written out.
		*/
		unsigned char character;
		if (instraw.Get(&character, sizeof(character)) == 0) break;

		/*
		**	See if there is a match for the current code and current
		**	character. A match indicates that there is already a
		**	dictionary entry that fully represents the character
		**	sequence.
		*/
		int index = Find_Child_Node(string_code, character);

		/*
		**	If a code match was found, then set the current code
		**	value to this code value that represents the concatenation
		**	of the previous code value and the current character.
		*/
		if (index != -1 && dict[index].CodeValue != -1) {
			string_code = dict[index].CodeValue;
		} else {

			/*
			**	Since no exact match was found, then create a new code
			**	entry that represents the current code and character
			**	value concatenated. This presumes there is room in the
			**	code table.
			*/
			if (index != -1 && next_code <= MAX_CODE) {
				dict[index] = CodeClass(next_code, string_code, character);
				next_code++;
			}

			/*
			**	Output the code to the compression stream and reset the
			**	current code value to match the current character. This
			**	has the effect of clearing out the current character
			**	sequence scan in preparation for building a new one. It
			**	also ensures that the character will be written out.
			*/
			outcount += outpipe.Put(&string_code, sizeof(string_code));
			string_code = character;
		}
	}

	outcount += outpipe.Put(&string_code, sizeof(string_code));
	if (string_code != END_OF_STREAM) {
		string_code = END_OF_STREAM;
		outcount += outpipe.Put(&string_code, sizeof(string_code));
	}

	return(outcount);
}


int LZWEngine::Uncompress(Buffer const & input, Buffer const & output)
{
	int outcount = 0;
	BufferStraw instraw(input);
	BufferPipe outpipe(output);

	CodeType old_code;
	if (instraw.Get(&old_code, sizeof(old_code)) == 0) {
		return(outcount);
	}

	unsigned char character = (unsigned char)old_code;
	outcount += outpipe.Put(&character, sizeof(character));

	unsigned int count;
	CodeType new_code;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		if (instraw.Get(&new_code, sizeof(new_code)) == 0) break;

		if (new_code == END_OF_STREAM) break;

		/*
		** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
		** case which generates an undefined code.  It handles it by decoding
		** the last code, and adding a single character to the end of the decode string.
		*/
		if (new_code >= next_code) {
			decode_stack[0] = character;
			count = 1;
			count += Decode_String(&decode_stack[1], old_code);
		} else {
			count = Decode_String(decode_stack, new_code);
		}

		character = decode_stack[count-1];
		while (count > 0) {
			--count;
			outcount += outpipe.Put(&decode_stack[count], sizeof(decode_stack[0]));
		}

		/*
		**	Add the new code sequence to the dictionary (presuming there is still
		**	room).
		*/
		if (next_code <= MAX_CODE) {
			dict[next_code] = CodeClass(next_code, old_code, character);
			next_code++;
		}
		old_code = new_code;
	}

	return(outcount);
}


int LZWEngine::Make_LZW_Hash(CodeType code, char character)
{
	return((((int)(unsigned char)character) << ( BITS - 8 ) ) ^ (int)code);
}


int LZWEngine::Find_Child_Node(CodeType parent_code, char child_character)
{
	/*
	**	Fetch the first try index for the code and character.
	*/
	int hash_index = Make_LZW_Hash(parent_code, child_character);

	/*
	**	Base the hash-miss-try-again-displacement value on the current
	**	index. [Shouldn't the value be some large prime number???].
	*/
	int offset = 1;
	if (hash_index != 0) {
		offset = TABLE_SIZE - hash_index;
	}

	/*
	**	Keep offsetting through the dictionary until an exact match is
	**	found for the code and character specified.
	*/
	int initial = hash_index;
	while (!dict[hash_index].Is_Matching(parent_code, child_character)) {

		/*
		**	Stop searching if an unused index is found since this means that
		**	a match doesn't exist in the table at all.
		*/
		if (dict[hash_index].Is_Unused()) break;

		/*
		**	Bump the hash index to another value such that sequential bumps
		**	will not result in the same index value until all of the table
		**	has been scanned.
		*/
		hash_index -= offset;
		if (hash_index < 0) {
			hash_index += TABLE_SIZE;
		}

		/*
		**	If the entire table has been scanned and no match or unused
		**	entry was found, then return a special value indicating this
		**	condition.
		*/
		if (initial == hash_index) {
			hash_index = -1;
			break;
		}
	}
	return(hash_index);
}


int LZWEngine::Decode_String(char * ptr, CodeType code)
{
	int count = 0;
	while (code > 255) {
		*ptr++ = dict[code].CharValue;
		count++;
		code = dict[code].ParentCode;
	}
	*ptr = (char)code;
	count++;
	return(count);
}


int LZW_Uncompress(Buffer const & inbuff, Buffer const & outbuff)
{
	LZWEngine lzw;
	return(lzw.Uncompress(inbuff, outbuff));
}


int LZW_Compress(Buffer const & inbuff, Buffer const & outbuff)
{
	LZWEngine lzw;
	return(lzw.Compress(inbuff, outbuff));
}





#ifdef NEVER


/*
 * Constants used throughout the program.  BITS defines how many bits
 * will be in a code.  TABLE_SIZE defines the size of the dictionary
 * table.
 */
#define BITS                       12
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 5021
#define END_OF_STREAM              256
#define FIRST_CODE                 257
#define UNUSED                     -1

typedef unsigned short CodeType;

/*
 * This data structure defines the dictionary.  Each entry in the dictionary
 * has a code value.  This is the code emitted by the compressor.  Each
 * code is actually made up of two pieces:  a parent_code, and a
 * character.  Code values of less than 256 are actually plain
 * text codes.
 */
struct CodeClass
{
	CodeType CodeValue;
	CodeType ParentCode;
	char CharValue;

	CodeClass(void) {}
	CodeClass(CodeType code, CodeType parent, char c) : CodeValue(code), ParentCode(parent), CharValue(c) {}

	void Make_Unused(void) {CodeValue = UNUSED;}
	bool Is_Unused(void) const {return(CodeValue == UNUSED);}
	bool Is_Matching(CodeType code, char c) const {return(ParentCode == code && CharValue == c);}
};
CodeClass dict[TABLE_SIZE];

char decode_stack[TABLE_SIZE];

inline int Make_LZW_Hash(CodeType code, char character)
{
	return((((int)(unsigned char)character) << ( BITS - 8 ) ) ^ (int)code);
}


/***********************************************************************************************
 * Find_Child_Node -- Find a matching dictionary entry.                                        *
 *                                                                                             *
 *    This hashing routine is responsible for finding the table location                       *
 *    for a string/character combination.  The table index is created                          *
 *    by using an exclusive OR combination of the prefix and character.                        *
 *    This code also has to check for collisions, and handles them by                          *
 *    jumping around in the table.                                                             *
 *                                                                                             *
 * INPUT:   parent_code -- The code of the parent string sequence.                             *
 *                                                                                             *
 *          character   -- The current character.                                              *
 *                                                                                             *
 * OUTPUT:  Returns with the index for the matching dictionary entry. If no matching index     *
 *          could be found, then it returns with the index to an unused dictionary entry. If   *
 *          there are also no unused entries in the dictionary, then -1 is returned.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/28/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static int Find_Child_Node(CodeType parent_code, char child_character)
{
	/*
	**	Fetch the first try index for the code and character.
	*/
	int hash_index = Make_LZW_Hash(parent_code, child_character);

	/*
	**	Base the hash-miss-try-again-displacement value on the current
	**	index. [Shouldn't the value be some large prime number???].
	*/
	int offset = 1;
	if (hash_index != 0) {
		offset = TABLE_SIZE - hash_index;
	}

	/*
	**	Keep offsetting through the dictionary until an exact match is
	**	found for the code and character specified.
	*/
	int initial = hash_index;
	while (!dict[hash_index].Is_Matching(parent_code, child_character)) {

		/*
		**	Stop searching if an unused index is found since this means that
		**	a match doesn't exist in the table at all.
		*/
		if (dict[hash_index].Is_Unused()) break;

		/*
		**	Bump the hash index to another value such that sequential bumps
		**	will not result in the same index value until all of the table
		**	has been scanned.
		*/
		hash_index -= offset;
		if (hash_index < 0) {
			hash_index += TABLE_SIZE;
		}

		/*
		**	If the entire table has been scanned and no match or unused
		**	entry was found, then return a special value indicating this
		**	condition.
		*/
		if (initial == hash_index) {
			hash_index = -1;
			break;
		}
	}
	return(hash_index);
}


/*
 * This routine decodes a string from the dictionary, and stores it
 * in the decode_stack data structure.  It returns a count to the
 * calling program of how many characters were placed in the stack.
 */
static int Decode_String(char * ptr, CodeType code)
{
	int count = 0;
	while (code > 255) {
		*ptr++ = dict[code].CharValue;
		count++;
		code = dict[code].ParentCode;
	}
	*ptr = (char)code;
	count++;
	return(count);
}


/*
 * The compressor is short and simple.  It reads in new symbols one
 * at a time from the input file.  It then  checks to see if the
 * combination of the current symbol and the current code are already
 * defined in the dictionary.  If they are not, they are added to the
 * dictionary, and we start over with a new one symbol code.  If they
 * are, the code for the combination of the code and character becomes
 * our new code.
 */

int LZW_Compress(Buffer & inbuff, Buffer & outbuff)
{
	BufferStraw input(inbuff);
	BufferPipe output(outbuff);

	for (int i = 0; i < TABLE_SIZE; i++) {
		dict[i].Make_Unused();
//		dict[i].code_value = UNUSED;
	}

	int outcount = 0;
	CodeType string_code = END_OF_STREAM;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		char character;

		if (input.Get(&character, sizeof(character)) == 0) break;

		int index = Find_Child_Node(string_code, character);

		if (index == -1) {
			string_code = character;
			outcount += output.Put(&string_code, sizeof(string_code));
		} else {

			if (dict[index].CodeValue != -1) {
				string_code = dict[ index ].CodeValue;
			} else {
				if (next_code <= MAX_CODE) {
					dict[index] = CodeClass(next_code++, string_code, character);
				}
				outcount += output.Put(&string_code, sizeof(string_code));
				string_code = character;
			}
		}
	}

	outcount += output.Put(&string_code, sizeof(string_code));
	string_code = END_OF_STREAM;
	outcount += output.Put(&string_code, sizeof(string_code));

	return(outcount);
}


/*
 * The file expander operates much like the encoder.  It has to
 * read in codes, the convert the codes to a string of characters.
 * The only catch in the whole operation occurs when the encoder
 * encounters a CHAR+STRING+CHAR+STRING+CHAR sequence.  When this
 * occurs, the encoder outputs a code that is not presently defined
 * in the table.  This is handled as an exception.
 */
int LZW_Uncompress(Buffer & inbuff, Buffer & outbuff)
{
	int outcount = 0;
	BufferStraw input(inbuff);
	BufferPipe output(outbuff);

	CodeType old_code;
	if (input.Get(&old_code, sizeof(old_code)) == 0) {
		return(outcount);
	}

	char character = (char)old_code;
	outcount += output.Put(&character, sizeof(character));

	unsigned int count;
	CodeType new_code;
	CodeType next_code = FIRST_CODE;
	for (;;) {
		if (input.Get(&new_code, sizeof(new_code)) == 0) break;

		/*
		** This code checks for the CHARACTER+STRING+CHARACTER+STRING+CHARACTER
		** case which generates an undefined code.  It handles it by decoding
		** the last code, and adding a single character to the end of the decode string.
		*/
		if (new_code >= next_code) {
			decode_stack[0] = character;
			count = 1;
			count += Decode_String(&decode_stack[1], old_code);
		} else {
			count = Decode_String(decode_stack, new_code);
		}

		character = decode_stack[count-1];
		while (count > 0) {
			--count;
			outcount += output.Put(&decode_stack[count], sizeof(decode_stack[0]));
		}

		/*
		**	Add the new code sequence to the dictionary (presuming there is still
		**	room).
		*/
		if (next_code <= MAX_CODE) {
			dict[next_code] = CodeClass(next_code, old_code, character);
			next_code++;
		}
		old_code = new_code;
	}

	return(outcount);
}

#endif

// jmarshall: This function is from OmniBlade at Chronoshift
// https://github.com/TheAssemblyArmada/Chronoshift/blob/develop/src/game/common/lcw.cpp
/**
 * @brief Compresses data to the proprietary LCW format used in many games developed by Westwood Studios.
 * @warning Worst case can have the compressed data larger than the original.
 */
extern "C" int LCW_Comp(const void* src, void* dst, unsigned int bytes)
{
	//captainslog_assert(src != nullptr);
	//captainslog_assert(dst != nullptr);

	if (!bytes) {
		return 0;
	}

	// Decide if we are going to do relative offsets for 3 and 5 byte commands
	bool relative = bytes > UINT16_MAX;

	const uint8_t* getp = static_cast<const uint8_t*>(src);
	uint8_t* putp = static_cast<uint8_t*>(dst);
	const uint8_t* getstart = getp;
	const uint8_t* getend = getp + bytes;
	uint8_t* putstart = putp;

	// captainslog_debug("LCW Compression... ");

	// relative LCW starts with 0 to flag as relative for decoder
	if (relative) {
		// captainslog_debug("LCW Relative compressor.");
		*putp++ = 0;
	}

	// Write a starting cmd1 and set bool to have cmd1 in progress
	uint8_t* cmd_onep = putp;
	*putp++ = 0x81;
	*putp++ = *getp++;
	bool cmd_one = true;

	// Compress data
	while (getp < getend) {
		// Is RLE encode (4bytes) worth evaluating?
		if (getend - getp > 64 && *getp == *(getp + 64)) {
			// RLE run length is encoded as a short so max is UINT16_MAX
			const uint8_t* rlemax = (getend - getp) < UINT16_MAX ? getend : getp + UINT16_MAX;
			const uint8_t* rlep;

			for (rlep = getp + 1; *rlep == *getp && rlep < rlemax; ++rlep)
				;

			uint16_t run_length = rlep - getp;

			// If run length is long enough, write the command and start loop again
			if (run_length >= 0x41) {
				// captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, putp - putstart,
				// run_length);  write 4byte command 0b11111110
				cmd_one = false;
				*putp++ = 0xFE;
				*putp++ = run_length;
				*putp++ = run_length >> 8;
				*putp++ = *getp;
				getp = rlep;
				continue;
			}
		}

		// current block size for an offset copy
		int block_size = 0;
		const uint8_t* offstart;

		// Set where we start looking for matching runs.
		if (relative) {
			offstart = (getp - getstart) < UINT16_MAX ? getstart : getp - UINT16_MAX;
		}
		else {
			offstart = getstart;
		}

		// Look for matching runs
		const uint8_t* offchk = offstart;
		const uint8_t* offsetp = getp;
		while (offchk < getp) {
			// Move offchk to next matching position
			while (offchk < getp && *offchk != *getp) {
				++offchk;
			}

			// If the checking pointer has reached current pos, break
			if (offchk >= getp) {
				break;
			}

			// find out how long the run of matches goes for
			//<= because it can consider the current pixel as part of a run
			int i;
			for (i = 1; &getp[i] < getend; ++i) {
				if (offchk[i] != getp[i]) {
					break;
				}
			}

			if (i >= block_size) {
				block_size = i;
				offsetp = offchk;
			}

			++offchk;
		}

		// decide what encoding to use for current run
		if (block_size <= 2) {
			// short copy 0b10??????
			// check we have an existing 1 byte command and if its value is still
			// small enough to handle additional bytes
			// start a new command if current one doesn't have space or we don't
			// have one to continue
			if (cmd_one && *cmd_onep < 0xBF) {
				// increment command value
				++* cmd_onep;
				// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, cmd_onep - putstart,
				// *cmd_onep - 0x80);
				*putp++ = *getp++;
			}
			else {
				cmd_onep = putp;
				// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", getp - getstart, cmd_onep - putstart, 1);
				*putp++ = 0x81;
				*putp++ = *getp++;
				cmd_one = true;
			}
		}
		else {
			uint16_t offset;
			uint16_t rel_offset = getp - offsetp;
			if (block_size > 0xA || (rel_offset > 0xFFF)) {
				// write 5 byte command 0b11111111
				if (block_size > 0x40) {
					// captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
					// block_size);
					*putp++ = 0xFF;
					*putp++ = block_size;
					*putp++ = block_size >> 8;
					// write 3 byte command 0b11??????
				}
				else {
					// captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
					// block_size);
					*putp++ = (block_size - 3) | 0xC0;
				}

				offset = relative ? rel_offset : offsetp - getstart;
				// captainslog_debug("Offset %d", offset);
				// write 2 byte command? 0b0???????
			}
			else {
				// captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, ", getp - getstart, putp - putstart,
				// block_size);  uint8_t cmd_two_val = 16 * (block_size - 3) + (rel_offset >> 8);  captainslog_debug("Offset %d",
				// rel_offset);
				offset = rel_offset << 8 | (16 * (block_size - 3) + (rel_offset >> 8));
			}
			*putp++ = offset;
			*putp++ = offset >> 8;
			getp += block_size;
			cmd_one = false;
		}
	}

	// write final 0x80, this is why its also known as format80 compression
	*putp++ = 0x80;
	return putp - putstart;
}
// jmarshall end

extern "C" int LCW_Uncomp_CS(const void* src, void* dst, unsigned int bytes)
{
	//captainslog_assert(src != nullptr);
	//captainslog_assert(dst != nullptr);

	uint8_t* putstart = static_cast<uint8_t*>(dst);
	uint8_t* putend = putstart + bytes;
	uint8_t* putp = putstart;
	const uint8_t* getp = static_cast<const uint8_t*>(src);
	// const uint8_t *getstart = getp;

	// If first byte is 0, the all offsets are relative to current position. Otherwise some are absolute to the start of the
	// buffer, meaning only ~64KB can be compressed effectively. Compressor implemented in this file uses size to determine
	// compression scheme used.
	if (*getp == 0) {
		// captainslog_debug("LCW Relative Decompression... ");
		getp++;

		while (putp < putend) {
			uint8_t flag;
			uint16_t cpysize;
			uint16_t offset;

			flag = *getp++;

			if (flag & 0x80) {
				if (flag & 0x40) {
					cpysize = (flag & 0x3F) + 3;
					// long set 0b11111110
					if (flag == 0xFE) {
						cpysize = *getp++;
						cpysize += (*getp++) << 8;

						if (cpysize > putend - putp) {
							cpysize = putend - putp;
						}

						// captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 3, dest -
						// start, cpysize);
						memset(putp, (*getp++), cpysize);
						putp += cpysize;
					}
					else {
						uint8_t* s;
						// long move, abs 0b11111111
						if (flag == 0xFF) {
							cpysize = *getp++;
							cpysize += (*getp++) << 8;

							if (cpysize > putend - putp) {
								cpysize = putend - putp;
							}

							offset = *getp++;
							offset += (*getp++) << 8;

							// extended format for VQA32 and large WSA files.
							s = putp - offset;

							// captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
							// 5, dest - start, cpysize, offset);
							for (; cpysize > 0; --cpysize) {
								*putp++ = *s++;
							}
							// short move abs 0b11??????
						}
						else {
							if (cpysize > putend - putp) {
								cpysize = putend - putp;
							}

							offset = *getp++;
							offset += (*getp++) << 8;

							// extended format for VQA32 and large WSA files.
							s = putp - offset;

							// captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
							// 3, dest - start, cpysize, offset);
							for (; cpysize > 0; --cpysize) {
								*putp++ = *s++;
							}
						}
					}
					// short copy 0b10??????
				}
				else {
					if (flag == 0x80) {
						// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest -
						// start, 0);
						return putp - putstart;
					}

					cpysize = flag & 0x3F;

					if (cpysize > putend - putp) {
						cpysize = putend - putp;
					}

					// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest - start,
					// cpysize);
					for (; cpysize > 0; --cpysize) {
						*putp++ = *getp++;
					}
				}
				// short move rel 0b0???????
			}
			else {
				cpysize = (flag >> 4) + 3;

				if (cpysize > putend - putp) {
					cpysize = putend - putp;
				}

				offset = ((flag & 0xF) << 8) + (*getp++);
				// captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart - 2, dest -
				// start, cpysize, offset);
				for (; cpysize > 0; --cpysize) {
					*putp = *(putp - offset);
					putp++;
				}
			}
		}
	}
	else {
		// captainslog_debug("LCW Decompression... ");

		while (putp < putend) {
			uint8_t flag;
			uint16_t cpysize;
			uint16_t offset;

			flag = *getp++;

			if (flag & 0x80) {
				if (flag & 0x40) {
					cpysize = (flag & 0x3F) + 3;
					// long set 0b11111110
					if (flag == 0xFE) {
						cpysize = *getp++;
						cpysize += (*getp++) << 8;

						if (cpysize > putend - putp) {
							cpysize = putend - putp;
						}

						// captainslog_debug("0b11111110 Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 3, dest -
						// start, cpysize);
						memset(putp, (*getp++), cpysize);
						putp += cpysize;
					}
					else {
						uint8_t* s;
						// long move, abs 0b11111111
						if (flag == 0xFF) {
							cpysize = *getp++;
							cpysize += (*getp++) << 8;

							if (cpysize > putend - putp) {
								cpysize = putend - putp;
							}

							offset = *getp++;
							offset += (*getp++) << 8;
							s = putstart + offset;

							// captainslog_debug("0b11111111 Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
							// 5, dest - start, cpysize, offset);
							for (; cpysize > 0; --cpysize) {
								*putp++ = *s++;
							}
							// short move abs 0b11??????
						}
						else {
							if (cpysize > putend - putp) {
								cpysize = putend - putp;
							}

							offset = *getp++;
							offset += (*getp++) << 8;
							s = putstart + offset;

							// captainslog_debug("0b11?????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart -
							// 3, dest - start, cpysize, offset);
							for (; cpysize > 0; --cpysize) {
								*putp++ = *s++;
							}
						}
					}
					// short copy 0b10??????
				}
				else {
					if (flag == 0x80) {
						// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest -
						// start, 0);
						return putp - putstart;
					}

					cpysize = flag & 0x3F;

					if (cpysize > putend - putp) {
						cpysize = putend - putp;
					}

					// captainslog_debug("0b10?????? Source Pos %ld, Dest Pos %ld, Count %d", source - sstart - 1, dest - start,
					// cpysize);
					for (; cpysize > 0; --cpysize) {
						*putp++ = *getp++;
					}
				}
				// short move rel 0b0???????
			}
			else {
				cpysize = (flag >> 4) + 3;

				if (cpysize > putend - putp) {
					cpysize = putend - putp;
				}

				offset = ((flag & 0xF) << 8) + (*getp++);
				// captainslog_debug("0b0??????? Source Pos %ld, Dest Pos %ld, Count %d, Offset %d", source - sstart - 2, dest -
				// start, cpysize, offset);
				for (; cpysize > 0; --cpysize) {
					*putp = *(putp - offset);
					putp++;
				}
			}
		}
	}

	return putp - putstart;
}
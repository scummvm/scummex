/* DeScumm - Scumm Script Disassembler
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2002, 2003  The ScummVM Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/descumm.h,v 1.2 2003/09/19 19:57:07 yoshizf Exp $
 *
 */

#ifndef DESCUMM_H
#define DESCUMM_H

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#include <process.h>
#endif


#define ARRAYSIZE(x) ((int)(sizeof(x) / sizeof(x[0])))

uint32 inline SWAP_32(uint32 a)
{
	return ((a >> 24) & 0xFF) + ((a >> 8) & 0xFF00) + ((a << 8) & 0xFF0000) +
		((a << 24) & 0xFF000000);
}

uint16 inline SWAP_16(uint16 a)
{
	return ((a >> 8) & 0xFF) + ((a << 8) & 0xFF00);
}

//
// The block stack records jump instructions
//
struct BlockStack {
	unsigned short from;	// From which offset...
	unsigned short to;		// ...to which offset
	bool isWhile;			// Set to true if we think this jump is part of a while loop
};

extern BlockStack *block_stack;
extern int num_block_stack;

//
// Jump decoding auxillaries (used by the code which tries to translate jumps
// back into if / else / while / etc. constructs).
//
extern bool pendingElse, haveElse;
extern int pendingElseTo;
extern int pendingElseOffs;
extern int pendingElseOpcode;
extern int pendingElseIndent;

//
// The opcode of an unconditional jump instruction.
//
extern int g_jump_opcode;

//
// Command line options
//
extern bool alwaysShowOffs;
extern bool dontOutputIfs;
extern bool dontOutputElse;
extern bool dontOutputElseif;
extern bool dontOutputWhile;
extern bool dontShowOpcode;
extern bool dontShowOffsets;
extern bool haltOnError;

//
// The SCUMM version used for the script we are descumming.
//
extern byte scriptVersion;

//
// Various positions / offsets
//
extern byte *cur_pos, *org_pos;
extern int offs_of_line;

//
// Total size of the currently loaded script
//
extern uint size_of_code;

struct StackEnt {
	byte type;
	long data;
	StackEnt *left, *right;
	char *str;
	StackEnt **list;
};

class DeScumm {
private:
	GUI_wxWindows *_gui;

	char *strecpy(char *buf, const char *src);
	int get_curoffs();
	int get_byte();
	uint get_word();
	int get_signed_word();
	char *getIndentString(int level);
	void outputLine(char *buf, int curoffs, int opcode, int indent);
	bool indentBlock(unsigned int cur);
	BlockStack *pushBlockStackItem();
	bool maybeAddIf(uint cur, uint to);
	bool maybeAddElse(uint cur, uint to);
	bool maybeAddElseIf(uint cur, uint elseto, uint to);
	void writePendingElse();

	const char *get_num_string(int i);
	char *get_var(char *buf);
	char *get_var_or_word(char *buf, char condition);
	char *get_var_or_byte(char *buf, char condition);
	char *get_var_until_0xff(char *buf);
	char *putascii(char *buf, int i);
	char *get_ascii(char *buf);
	char *add_a_tok(char *buf, int type);
	char *do_tok(char *buf, const char *text, int args);
	void do_decodeparsestring_v2(char *buf, byte opcode);
	void do_actorset_v12(char *buf, byte opcode);
	void do_actorset(char *buf, byte opcode);
	void AddToExprStack(char *s);
	char *GetFromExprStack(char *buf);
	void do_expr_code(char *buf);
	void do_load_code_to_string(char *buf, byte opcode);
	void do_resource_v2(char *buf, byte opcode);
	void do_resource(char *buf, byte opco);
	void do_pseudoRoom(char *buf);
	void do_room_ops(char *buf);
	void do_room_ops_old(char *buf, byte master_opcode);
	void do_cursor_command(char *buf);
	void do_verbops_v2(char *buf, byte opcode);
	void do_verbops(char *buf, byte opcode);
	void do_print_ego(char *buf, byte opcode);
	void do_unconditional_jump(char *buf);
	void emit_if(char *buf, char *condition);
	void do_if_code(char *buf, byte opcode);
	void do_if_state_code(char *buf, byte opcode);
	void do_varset_code(char *buf, byte opcode);
	void do_matrix_ops(char *buf, byte opcode);
	void get_tok_V12(char *buf);
	void get_tok_V345(char *buf);
	int skipVerbHeader_V12(byte *p);
	int skipVerbHeader_V34(byte *p);
	byte *skipVerbHeader_V5(byte *p);

public:
	DeScumm(File& _input, int size, int scummVersion);

private:
	char *output;

	const char *getVarName(uint var);
	void push(StackEnt * se);
	void invalidop(const char *cmd, int op);
	byte *skipVerbHeader_V8(byte *p);
	int skipVerbHeader_V67(byte *p);
	StackEnt *se_new(int type);
	void se_free(StackEnt * se);
	StackEnt *se_neg(StackEnt * se);
	StackEnt *se_int(int i);
	StackEnt *se_var(int i);
	StackEnt *se_array(int i, StackEnt * dim2, StackEnt * dim1);
	StackEnt *se_oper(StackEnt * a, int op);
	StackEnt *se_oper(StackEnt * a, int op, StackEnt * b);
	StackEnt *se_complex(const char *s);
	char *se_astext(StackEnt * se, char *where, bool wantparens = true);
	StackEnt *pop();
	void kill(StackEnt * se);
	void doAssign(StackEnt * dst, StackEnt * src);
	void doAdd(StackEnt * se, int val);
	StackEnt *dup(StackEnt * se);
	void writeArray(int i, StackEnt * dim2, StackEnt * dim1, StackEnt * value);
	void writeVar(int i, StackEnt * value);
	void addArray(int i, StackEnt * dim1, int val);
	void addVar(int i, int val);
	StackEnt *se_get_string();
	StackEnt *se_get_list();
	void ext(const char *fmt);
	void jump();
	void jumpif(StackEnt * se, bool negate);
	void next_line_V8();
	void next_line_V67();
	
};

#endif

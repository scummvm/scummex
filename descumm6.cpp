/* DeScumm - Scumm Script Disassembler (version 6-8 scripts)
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
 * $Header: /Users/sev/projects/sc/s/scummvm/scummex/descumm6.cpp,v 1.1 2003/09/19 19:57:07 yoshizf Exp $
 *
 */

#include "file.h"
#include "wxwindows.h"
#include "descumm.h"

enum {
	seInt = 1,
	seVar = 2,
	seArray = 3,
	seBinary = 4,
	seUnary = 5,
	seComplex = 6,
	seStackList = 7,
	seDup = 8,
	seNeg = 9
};

enum {
	isZero,
	isEqual,
	isNotEqual,
	isGreater,
	isLess,
	isLessEqual,
	isGreaterEqual,
	operAdd,
	operSub,
	operMul,
	operDiv,
	operLand,
	operLor,
	operBand,
	operBor,
	operMod
};

static const char *oper_list[] = {
	"0==",
	"==",
	"!=",
	">",
	"<",
	"<=",
	">=",
	"+",
	"-",
	"*",
	"/",
	"&&",
	"||",
	"&",
	"|",
	"%"
};

StackEnt *stack[128];
int num_stack;
bool HumongousFlag = false;

const char *var_names6[] = {
	/* 0 */
	NULL,
	"g_ego",
	"g_camera_cur_pos",
	"g_have_msg",
	/* 4 */
	"g_room",
	"g_override",
	NULL,
	NULL,
	/* 8 */
	"g_num_actor",
	NULL,
	"g_drive_number",
	"g_timer_1",
	/* 12 */
	"g_timer_2",
	"g_timer_3",
	NULL,
	NULL,
	/* 16 */
	NULL,
	"g_camera_min",
	"g_camera_max",
	"g_timer_next",
	/* 20 */
	"g_virtual_mouse_x",
	"g_virtual_mouse_y",
	"g_room_resource",
	"g_last_sound",
	/* 24 */
	"g_cutsceneexit_key",
	"g_talk_actor",
	"g_camera_fast",
	"g_scroll_script",
	/* 28 */
	"g_entry_script",
	"g_entry_script_2",
	"g_exit_script",
	"g_exit_script_2",
	/* 32 */
	"g_verb_script",
	"g_sentence_script",
	"g_hook_script",
	"g_begin_cutscene_script",
	/* 36 */
	"g_end_cutscene_script",
	"g_char_inc",
	"g_walkto_obj",
	"g_debug_mode",
	/* 40 */
	"g_heap_space",
	"g_scr_width",
	"g_restart_key",
	"g_pause_key",
	/* 44 */
	"g_mouse_x",
	"g_mouse_y",
	"g_timer",
	"g_timer_4",
	/* 48 */
	NULL,
	"g_video_mode",
	"g_save_load_key",
	"g_fixed_disk",
	/* 52 */
	"g_cursor_state",
	"g_user_put",
	"g_scr_height",
	NULL,
	/* 56 */
	"g_sound_thing",
	"g_talkstop_key",
	NULL,
	NULL,
	/* 60 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 64 */
	"g_sound_param",
	"g_sound_param_2",
	"g_sound_param_3",
	"g_mouse_present",
	/* 68 */
	"g_performance_1",
	"g_performance_2",
	NULL,
	"g_save_load_thing",
	/* 72 */
	"g_new_room",
	NULL,
	NULL,
	NULL,
	/* 76 */
	"g_ems_space"
};

const char *var_names7[] = {
	/* 0 */
	NULL,
	"VAR_MOUSE_X",
	"VAR_MOUSE_Y",
	"VAR_VIRT_MOUSE_X",
	/* 4 */
	"VAR_VIRT_MOUSE_Y",
	"VAR_V6_SCREEN_WIDTH",
	"VAR_V6_SCREEN_HEIGHT",
	"VAR_CAMERA_POS_X",
	/* 8 */
	"VAR_CAMERA_POS_Y",
	"VAR_OVERRIDE",
	"VAR_ROOM",
	"VAR_ROOM_RESOURCE",
	/* 12 */
	"VAR_TALK_ACTOR",
	"VAR_HAVE_MSG",
	"VAR_TIMER",
	"VAR_TMR_4",
	/* 16 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 20 */
	NULL,
	NULL,
	"VAR_LEFTBTN_DOWN",
	"VAR_RIGHTBTN_DOWN",
	/* 24 */
	"VAR_LEFTBTN_HOLD",
	"VAR_RIGHTBTN_HOLD",
	"VAR_PERFORMANCE_1",
	"VAR_PERFORMANCE_2",
	/* 28 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 32 */
	"VAR_V6_EMSSPACE",
	NULL,
	"VAR_V6_RANDOM_NR",
	"VAR_NEW_ROOM",
	/* 36 */
	"VAR_WALKTO_OBJ",
	NULL,
	"VAR_CAMERA_DEST_X",
	"VAR_CAMERA_DEST_>",
	/* 40 */
	"VAR_CAMERA_FOLLOWED_ACTOR",
	NULL,
	NULL,
	NULL,
	/* 44 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 48 */
	NULL,
	NULL,
	"VAR_SCROLL_SCRIPT",
	"VAR_ENTRY_SCRIPT",
	/* 52 */
	"VAR_ENTRY_SCRIPT2",
	"VAR_EXIT_SCRIPT",
	"VAR_EXIT_SCRIPT2",
	"VAR_VERB_SCRIPT",
	/* 56 */
	"VAR_SENTENCE_SCRIPT",
	"VAR_HOOK_SCRIPT",
	"VAR_CUTSCENE_START_SCRIPT",
	"VAR_CUTSCENE_END_SCRIPT",
	/* 60 */
	"VAR_UNK_SCRIPT",
	"VAR_UNK_SCRIPT2",
	"VAR_CUTSCENEEXIT_KEY",
	"VAR_RESTART_KEY",			// ???
	/* 64 */
	"VAR_PAUSE_KEY",
	"VAR_SAVELOADDIALOG_KEY",	// ???
	NULL,
	"VAR_TALKSTOP_KEY",			// ???
	/* 68 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 72 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 76 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 80 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 84 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 88 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 92 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 96 */
	NULL,
	"VAR_TIMER_NEXT",
	"VAR_TMR_1",
	"VAR_TMR_2",
	/* 100 */
	"VAR_TMR_3",
	"VAR_CAMERA_MIN_X",
	"VAR_CAMERA_MAX_X",
	"VAR_CAMERA_MIN_Y",
	/* 104 */
	"VAR_CAMERA_MAX_Y",
	"VAR_CAMERA_THRESHOLD_X",
	"VAR_CAMERA_THRESHOLD_Y",
	"VAR_CAMERA_SPEED_X",
	/* 108 */
	"VAR_CAMERA_SPEED_Y",
	"VAR_CAMERA_ACCEL_X",
	"VAR_CAMERA_ACCEL_Y",
	"VAR_EGO",
	/* 112 */
	"VAR_CURSORSTATE",
	"VAR_USERPUT",
	"VAR_DEFAULT_TALK_DELAY",
	"VAR_CHARINC",
	/* 116 */
	"VAR_DEBUGMODE",
	NULL,
	NULL,
	"VAR_CHARSET_MASK",
	/* 120 */
	NULL,
	NULL,
	NULL,
	"VAR_VIDEONAME",
	/* 124 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 128 */
	NULL,
	NULL,
	"VAR_STRING2DRAW",
	"VAR_CUSTOMSCALETABLE",
};

const char *var_names8[] = {
	/* 0 */
	NULL,
	"room_width?",
	"room_height?",
	"VAR_MOUSE_X",
	/* 4 */
	"VAR_MOUSE_Y",
	"VAR_VIRT_MOUSE_X",
	"VAR_VIRT_MOUSE_Y",
	"VAR_CURSORSTATE",
	/* 8 */
	"VAR_USERPUT",
	"VAR_CAMERA_POS_X",
	"VAR_CAMERA_POS_Y",
	"VAR_CAMERA_DEST_X",
	/* 12 */
	"VAR_CAMERA_DEST_Y",
	"VAR_CAMERA_FOLLOWED_ACTOR",
	"VAR_TALK_ACTOR",
	"VAR_HAVE_MSG",
	/* 16 */
	"VAR_MOUSE_BUTTONS",
	NULL,
	"VAR_MOUSE_HOLD",
	NULL,
	/* 20 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 24 */
	"VAR_TIMEDATE_YEAR",
	"VAR_TIMEDATE_MONTH",
	"VAR_TIMEDATE_DAY",
	"VAR_TIMEDATE_HOUR",
	/* 28 */
	"VAR_TIMEDATE_MINUTE",
	"VAR_TIMEDATE_SECOND",
	"VAR_OVERRIDE",
	"VAR_ROOM",
	/* 32 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 36 */
	NULL,
	NULL,
	NULL,
	"voice_text_mode",
	/* 40 */
	"VAR_GAME_LOADED",
	"VAR_LANGUAGE",
	"VAR_CURRENTDISK",
	NULL,
	/* 44 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 48 */
	NULL,
	NULL,
	NULL,
	"VAR_ENTRY_SCRIPT",
	/* 52 */
	"VAR_ENTRY_SCRIPT2",
	"VAR_EXIT_SCRIPT",
	"VAR_EXIT_SCRIPT2",
	"VAR_VERB_SCRIPT",
	/* 56 */
	"VAR_SENTENCE_SCRIPT",
	"VAR_HOOK_SCRIPT",
	"VAR_CUTSCENE_START_SCRIPT",
	"VAR_CUTSCENE_END_SCRIPT",
	/* 60 */
	NULL,
	NULL,
	"VAR_CUTSCENEEXIT_KEY",
	"VAR_RESTART_KEY",
	/* 64 */
	"VAR_PAUSE_KEY",
	"VAR_SAVELOADDIALOG_KEY",
	NULL,
	"VAR_TALKSTOP_KEY",
	/* 68 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 72 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 76 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 80 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 84 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 88 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 92 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 96 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 100 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 104 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 108 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 112 */
	"VAR_TIMER_NEXT",
	"VAR_TMR_1",
	"VAR_TMR_2",
	"VAR_TMR_3",
	/* 116 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 120 */
	NULL,
	NULL,
	NULL,
	NULL,
	/* 124 */
	NULL,
	NULL,
	"VAR_EGO",
	NULL,
	/* 128 */
	NULL,
	"text_delay?",
	"VAR_DEBUGMODE",
	NULL,
	/* 132 */
	"VAR_KEYPRESS",
	NULL,
	NULL,
	NULL,
};

const char *DeScumm::getVarName(uint var)
{
	if (scriptVersion == 8) {
		if (var >= sizeof(var_names8) / sizeof(var_names8[0]))
			return NULL;
		return var_names8[var];
	} else if (scriptVersion == 7) {
		if (var >= sizeof(var_names7) / sizeof(var_names7[0]))
			return NULL;
		return var_names7[var];
	} else {
		if (var >= sizeof(var_names6) / sizeof(var_names6[0]))
			return NULL;
		return var_names6[var];
	}
}

void DeScumm::push(StackEnt * se)
{
	assert(se);
	stack[num_stack++] = se;
}

void DeScumm::invalidop(const char *cmd, int op)
{
	if (cmd)
		printf("ERROR: Unknown opcode %s:0x%x (stack count %d)\n", cmd, op, num_stack);
	else
		printf("ERROR: Unknown opcode 0x%x (stack count %d)\n", op, num_stack);
	exit(1);
}

byte *DeScumm::skipVerbHeader_V8(byte *p)
{
	uint32 *ptr;
	uint32 code;
	int hdrlen;
	
	ptr = (uint32 *)p;
	while (TO_LE_32(*ptr++) != 0) {
		ptr++;
	}
	hdrlen = (byte *)ptr - p;
	
	ptr = (uint32 *)p;
	while ((code = TO_LE_32(*ptr++)) != 0) {
		printf("  %2d - %.4X\n", code, TO_LE_32(*ptr++) - hdrlen);
	}

	return (byte *)ptr;
}

int DeScumm::skipVerbHeader_V67(byte *p)
{
	byte code;
	int offset = 8;
	int minOffset = 255;
	p += offset;

	printf("Events:\n");

	while ((code = *p++) != 0) {
		offset = TO_LE_16(*(uint16 *)p);
		p += 2;
		printf("  %2X - %.4X\n", code, offset);
		if (minOffset > offset)
			minOffset = offset;
	}
	return minOffset;
}

StackEnt *DeScumm::se_new(int type)
{
	StackEnt *se = (StackEnt *) malloc(sizeof(StackEnt));
	se->type = type;
	return se;
}

void DeScumm::se_free(StackEnt * se)
{
	free(se);
}

StackEnt *DeScumm::se_neg(StackEnt * se)
{
	StackEnt *s = se_new(seNeg);
	s->left = se;
	return s;
}

StackEnt *DeScumm::se_int(int i)
{
	StackEnt *se = se_new(seInt);
	se->data = i;
	return se;
}

StackEnt *DeScumm::se_var(int i)
{
	StackEnt *se = se_new(seVar);
	se->data = i;
	return se;
}

StackEnt *DeScumm::se_array(int i, StackEnt * dim2, StackEnt * dim1)
{
	StackEnt *se = se_new(seArray);
	se->left = dim2;
	se->right = dim1;
	se->data = i;
	return se;
}

StackEnt *DeScumm::se_oper(StackEnt * a, int op)
{
	StackEnt *se = se_new(seUnary);
	se->data = op;
	se->left = a;
	return se;
}

StackEnt *DeScumm::se_oper(StackEnt * a, int op, StackEnt * b)
{
	StackEnt *se = se_new(seBinary);
	se->data = op;
	se->left = a;
	se->right = b;
	return se;
}

StackEnt *DeScumm::se_complex(const char *s)
{
	StackEnt *se = se_new(seComplex);
	se->str = strdup(s);
	return se;
}

char *DeScumm::se_astext(StackEnt * se, char *where, bool wantparens)
{
	int i;
	int var;
	const char *s;

	switch (se->type) {
	case seInt:
		where += sprintf(where, "%ld", se->data);
		break;
	case seVar:
		if (scriptVersion == 8) {
			if (!(se->data & 0xF0000000)) {
				var = se->data & 0xFFFFFFF;
				if ((s = getVarName(var)) != NULL)
					where = strecpy(where, s);
				else
					where += sprintf(where, "var%ld", se->data & 0xFFFFFFF);
			} else if (se->data & 0x80000000) {
				where += sprintf(where, "bitvar%ld", se->data & 0x7FFFFFFF);
			} else if (se->data & 0x40000000) {
				where += sprintf(where, "localvar%ld", se->data & 0xFFFFFFF);
			} else {
				where += sprintf(where, "?var?%ld", se->data);
			}
		} else {
			if (!(se->data & 0xF000)) {
				var = se->data & 0xFFF;
				if ((s = getVarName(var)) != NULL)
					where = strecpy(where, s);
				else
					where += sprintf(where, "var%ld", se->data & 0xFFF);
			} else if (se->data & 0x8000) {
				where += sprintf(where, "bitvar%ld", se->data & 0x7FFF);
			} else if (se->data & 0x4000) {
				where += sprintf(where, "localvar%ld", se->data & 0xFFF);
			} else {
				where += sprintf(where, "?var?%ld", se->data);
			}
		}
		break;
	case seArray:
		if (se->left) {
			where += sprintf(where, "array-%ld[", se->data);
			where = se_astext(se->left, where);
			where = strecpy(where, "][");
			where = se_astext(se->right, where);
			where = strecpy(where, "]");
		} else {
			where += sprintf(where, "array-%ld[", se->data);
			where = se_astext(se->right, where);
			where = strecpy(where, "]");
		}
		break;
	case seUnary:
		where += sprintf(where, "%s ", oper_list[se->data]);
		where = se_astext(se->left, where);
		break;
	case seBinary:
		if (wantparens)
			*where++ = '(';
		where = se_astext(se->left, where);
		where += sprintf(where, " %s ", oper_list[se->data]);
		where = se_astext(se->right, where);
		if (wantparens)
			*where++ = ')';
		*where = 0;
		break;
	case seComplex:
		where = strecpy(where, se->str);
		break;
	case seStackList:
		*where++ = '[';
		for (i = se->data; --i >= 0;) {
			where = se_astext(se->list[i], where);
			if (i)
				*where++ = ',';
		}
		*where++ = ']';
		*where = 0;
		break;
	case seDup:
		where += sprintf(where, "dup[%ld]", se->data);
		break;
	case seNeg:
		*where++ = '!';
		where = se_astext(se->left, where);
		break;
	}
	return where;
}

StackEnt *DeScumm::pop()
{
	if (num_stack == 0) {
		printf("ERROR: No items on stack to pop!\n");

		if (!haltOnError)
			return se_complex("**** INVALID DATA ****");
 		exit(1);
	}
	return stack[--num_stack];
}


void DeScumm::kill(StackEnt * se)
{
	if (se->type != seDup) {
		char *e = strecpy(output, "pop(");
		e = se_astext(se, e);
		strcpy(e, ")");
		se_free(se);
	} else {
		push(se);
	}
}

void DeScumm::doAssign(StackEnt * dst, StackEnt * src)
{
	if (src->type == seDup && dst->type == seDup) {
		dst->data = src->data;
		return;
	}
	char *e = se_astext(dst, output);
	e = strecpy(e, " = ");
	se_astext(src, e);
}

void DeScumm::doAdd(StackEnt * se, int val)
{
	char *e = se_astext(se, output);
	sprintf(e, " += %d", val);
}

StackEnt *DeScumm::dup(StackEnt * se)
{
	static int dupindex = 0;

	if (se->type == seInt)
		return se;

	StackEnt *dse = se_new(seDup);
	dse->data = ++dupindex;
	doAssign(dse, se);
	return dse;
}

void DeScumm::writeArray(int i, StackEnt * dim2, StackEnt * dim1, StackEnt * value)
{
	StackEnt *array = se_array(i, dim2, dim1);
	doAssign(array, value);
	se_free(array);
}

void DeScumm::writeVar(int i, StackEnt * value)
{
	StackEnt *se = se_var(i);
	doAssign(se, value);
	se_free(se);
}

void DeScumm::addArray(int i, StackEnt * dim1, int val)
{
	StackEnt *array = se_array(i, NULL, dim1);
	doAdd(array, val);
	se_free(array);
}

void DeScumm::addVar(int i, int val)
{
	StackEnt *se = se_var(i);
	doAdd(se, val);
	se_free(se);
}

StackEnt *DeScumm::se_get_string()
{
	byte cmd;
	char buf[1024];
	char *e = buf;
	bool in = false;
	int i;

	while ((cmd = get_byte()) != 0) {
		if (cmd == 0xFF || cmd == 0xFE) {
			if (in) {
				*e++ = '"';
				in = false;
			}
			i = get_byte();
			switch (i) {
			case 1:
				e += sprintf(e, ":newline:");
				break;
			case 2:
				e += sprintf(e, ":keeptext:");
				break;
			case 3:
				e += sprintf(e, ":wait:");
				break;
			case 4:		// addIntToStack
			case 5:		// addVerbToStack
			case 6:		// addNameToStack
			case 7:		// addStringToStack
				{
				StackEnt foo;
				foo.type = seVar;
				foo.data = get_word();
				e += sprintf(e, ":");
				e = se_astext(&foo, e);
				e += sprintf(e, ":");
				}
				break;
			case 9:
				e += sprintf(e, ":startanim=%d:", get_word());
				break;
			case 10:
				e += sprintf(e, ":sound:");
				cur_pos += 14;
				break;
			case 14:
				e += sprintf(e, ":setfont=%d:", get_word());
				break;
			case 12:
				e += sprintf(e, ":setcolor=%d:", get_word());
				break;
			case 13:
				e += sprintf(e, ":unk2=%d:", get_word());
				break;
			default:
				e += sprintf(e, ":unk%d=%d:", i, get_word());
			}
		} else {
			if (!in) {
				*e++ = '"';
				in = true;
			}
			*e++ = cmd;
		}
	}
	if (in)
		*e++ = '"';
	*e = 0;
	return se_complex(buf);
}

StackEnt *DeScumm::se_get_list()
{
	StackEnt *se = se_new(seStackList);
	StackEnt *senum = pop();
	int num, i;

	if (senum->type != seInt) {
		printf("ERROR: stackList with variable number of arguments, cannot disassemble\n");
		exit(1);
	}
	se->data = num = senum->data;
	se->list = (StackEnt **) calloc(num, sizeof(StackEnt *));

	for(i = 0; i < num; i++) {
		se->list[i] = pop();
	}
	return se;
}

void DeScumm::ext(const char *fmt)
{
	bool wantresult;
	byte cmd, extcmd;
	const char *extstr = NULL;
	const char *prep = NULL;
	StackEnt *args[10];
	int numArgs = 0;
	char *e = (char *)output;

	/* return the result? */
	wantresult = false;
	if (*fmt == 'r') {
		wantresult = true;
		fmt++;
	}

	while ((cmd = *fmt++) != '|') {
		if (cmd == 'x' && !extstr) {
			/* Sub-op: next byte specifies which one */
			extstr = fmt;
			while (*fmt++)
				;
			e += sprintf(e, "%s.", extstr);

			/* extended thing */
			extcmd = get_byte();

			/* locate our extended item */
			while ((cmd = *fmt++) != extcmd) {
				/* scan until we find , or \0 */
				while ((cmd = *fmt++) != ',') {
					if (cmd == 0) {
						invalidop(extstr, extcmd);
					}
				}
			}
			/* found a command, continue at the beginning */
			continue;
		}
		if (cmd == 'y' && !extstr) {
			/* Sub-op: parameters are in a list, first element of the list specified the command */
			StackEnt *se;
			extstr = fmt;
			while (*fmt++)
				;
			e += sprintf(e, "%s.", extstr);
			
			args[numArgs++] = se_get_list();
			
			/* extended thing */
			se = args[numArgs - 1];
			se->data--;
			extcmd = (byte) se->list[se->data]->data;

			/* locate our extended item */
			while ((cmd = *fmt++) != extcmd) {
				/* scan until we find , or \0 */
				while ((cmd = *fmt++) != ',') {
					if (cmd == 0) {
						se->data++;
						fmt = "Unknown";
						goto output_command;
					}
				}
			}

			/* found a command, continue at the beginning */
			continue;
		}

		if (cmd == 'p') {
			args[numArgs++] = pop();
		} else if (cmd == 'z') {	// = popRoomAndObj()
			args[numArgs++] = pop();
			if (scriptVersion < 7)
				args[numArgs++] = pop();
		} else if (cmd == 's') {
			args[numArgs++] = se_get_string();
		} else if (cmd == 'w') {
			args[numArgs++] = se_int(get_word());
		} else if (cmd == 'l') {
			args[numArgs++] = se_get_list();
		} else if (cmd == 'j') {
			args[numArgs++] = se_int(get_word());
		} else {
			printf("error in argument string '%s', character '%c' unknown\n", fmt, cmd);
		}
	}

output_command:

	/* create a string from the arguments */
	if (prep)
		e = strecpy(e, prep);
	while (*fmt != 0 && *fmt != ',')
		*e++ = *fmt++;
	*e++ = '(';
	while (--numArgs >= 0) {
		e = se_astext(args[numArgs], e);
		if (numArgs)
			*e++ = ',';
	}
	*e++ = ')';
	*e = 0;

	if (wantresult) {
		push(se_complex((char *)output));
		output[0] = 0;
		return;
	}
}

void DeScumm::jump()
{
	int offset = get_signed_word();
	int cur = get_curoffs();
	int to = cur + offset;

	if (offset == 1) {
		// Sometimes, jumps with offset 1 occur. I used to suppress those.
		// But it turns out that's not quite correct in some cases. With this
		// code, it can sometimes happens that you get an empty 'else' branch;
		// but in many other cases, an otherwis hidden instruction is revealed,
		// or an instruction is placed into an else branch instead of being
		// (incorrectly) placed inside the body of the 'if' itself.
		sprintf(output, "/* jump %x; */", to);
	} else if (!dontOutputElse && maybeAddElse(cur, to)) {
		pendingElse = true;
		pendingElseTo = to;
		pendingElseOffs = cur - 1;
		pendingElseOpcode = g_jump_opcode;
		pendingElseIndent = num_block_stack;
	} else {
		if (num_block_stack && !dontOutputWhile) {
			BlockStack *p = &block_stack[num_block_stack - 1];
			if (p->isWhile && cur == p->to)
				return;		// A 'while' ends here.
		}
		sprintf(output, "jump %x", to);
	}
}

void DeScumm::jumpif(StackEnt * se, bool negate)
{
	int offset = get_signed_word();
	int cur = get_curoffs();
	int to = cur + offset;
	char *e = output;

	if (!dontOutputElseif && pendingElse) {
		if (maybeAddElseIf(cur, pendingElseTo, to)) {
			pendingElse = false;
			haveElse = true;
			e = strecpy(e, "} else if (");
			if (negate)
				se = se_neg(se);
			e = se_astext(se, e, false);
			sprintf(e, alwaysShowOffs ? ") /*%.4X*/ {" : ") {", to);
			return;
		}
	}

	if (!dontOutputIfs && maybeAddIf(cur, to)) {
		if (!dontOutputWhile && block_stack[num_block_stack - 1].isWhile) {
			e = strecpy(e, "while (");
		} else
			e = strecpy(e, "if (");
		if (negate)
			se = se_neg(se);
		e = se_astext(se, e, false);
		sprintf(e, alwaysShowOffs ? ") /*%.4X*/ {" : ") {", to);
		return;
	}

	e = strecpy(e, negate ? "if (" : "unless (");
	e = se_astext(se, e);
	sprintf(e, ") jump %x", to);
}


#define PRINT_V8(name)            \
	do {                          \
		ext("x" name "\0"         \
				"\xC8|baseop,"    \
				"\xC9|end,"       \
				"\xCApp|XY,"      \
				"\xCBp|color,"    \
				"\xCC|center,"    \
				"\xCDp|charset,"  \
				"\xCE|left,"      \
				"\xCF|overhead,"  \
				"\xD0|mumble,"    \
				"\xD1s|msg,"      \
				"\xD2|wrap"       \
				);                \
	} while(0)


void DeScumm::next_line_V8()
{
	byte code = get_byte();
	StackEnt *se_a, *se_b;

	switch (code) {
	case 0x1:
		push(se_int(get_word()));
		break;
	case 0x2:
		push(se_var(get_word()));
		break;
	case 0x3:
		push(se_array(get_word(), NULL, pop()));
		break;
	case 0x4:
		se_a = pop();
		push(se_array(get_word(), pop(), se_a));
		break;
	case 0x5:
		se_a = dup(pop());
		push(se_a);
		push(se_a);
		break;
	case 0x6:
		kill(pop());
		break;
	case 0x7:
		push(se_oper(pop(), isZero));
		break;
	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
		se_a = pop();
		se_b = pop();
		push(se_oper(se_b, (code - 0x8) + isEqual, se_a));
		break;
		
	case 0x64:
		jumpif(pop(), true);
		break;
	case 0x65:
		jumpif(pop(), false);
		break;
	case 0x66:
		jump();
		break;
	case 0x67:
		ext("|breakHere");
		break;
	case 0x68:
		ext("p|breakHereVar");
		break;
	case 0x69:
		ext("x" "wait\0"
				"\x1Epj|waitForActor,"
				"\x1F|waitForMessage,"
				"\x20|waitForCamera,"
				"\x21|waitForSentence,"
				"\x22pj|waitUntilActorDrawn,"
				"\x23pj|waitUntilActorTurned,"
				);
		break;
	case 0x6A:
		ext("p|delay");
		break;
	case 0x6B:
		ext("p|delaySeconds");
		break;
	case 0x6C:
		ext("p|delayMinutes");
		break;
	case 0x6D:
		writeVar(get_word(), pop());
		break;
	case 0x6E:
		addVar(get_word(), +1);
		break;
	case 0x6F:
		addVar(get_word(), -1);
		break;
	case 0x70:
		// FIXME - is this correct?!? Also, make the display nicer...
		ext("x" "dim\0"
				"\x0Apw|dim-scummvar,"
				"\x0Bpw|dim-string,"
				"\xCAw|undim"
				);
		break;
	case 0x71:
		se_a = pop();
		writeArray(get_word(), NULL, pop(), se_a);
		break;

	case 0x74:
		// FIXME - is this correct?!? Also, make the display nicer...
		ext("x" "dim2\0"
				"\x0Appw|dim2-scummvar,"
				"\x0Bppw|dim2-string,"
				"\xCAw|undim2"
				);
		break;
	case 0x75:
		se_a = pop();
		se_b = pop();
		writeArray(get_word(), pop(), se_b, se_a);
		break;
	case 0x76:
		switch (get_byte()) {
		case 0x14:{
			int array = get_word();
			writeArray(array, NULL, pop(), se_get_string());
			}
			break;
		case 0x15:
			se_a = pop();
			se_b = se_get_list();
			writeArray(get_word(), NULL, se_a, se_b);
			break;
		case 0x16:
			se_a = pop();
			se_b = se_get_list();
			writeArray(get_word(), pop(), se_a, se_b);
			break;
		}
		break;

	case 0x79:
		ext("lpp|startScriptEx");
		break;
	case 0x7A:
		ext("lp|startScript");
		break;
	case 0x7B:
		ext("|stopObjectCode");
		break;
	case 0x7C:
		ext("p|stopScript");
		break;
	case 0x7D:
		ext("lpp|jumpToScript");
		break;
	case 0x7E:
		ext("p|return");
		break;
	case 0x7F:
		ext("lppp|startObjectEx");
		break;

	case 0x81:
		ext("l|beginCutscene");
		break;
	case 0x82:
		ext("|endCutscene");
		break;
	case 0x83:
		ext("p|freezeUnfreeze");
		break;
	case 0x84:
		ext("|beginOverride");
		break;
	case 0x85:
		ext("|endOverride");
		break;
	case 0x86:
		ext("|stopSentence");
		break;

	case 0x89:
		ext("lp|setClassOf?");
		break;
	case 0x8A:
		ext("pp|setState");
		break;
	case 0x8B:
		ext("pp|setOwner");
		break;

	case 0x8C:
		ext("pp|panCameraTo");
		break;
	case 0x8D:
		ext("p|actorFollowCamera");
		break;
	case 0x8E:
		ext("pp|setCameraAt");
		break;
	case 0x8F:
		ext("ps|printActor");
		break;
	case 0x90:
		PRINT_V8("printEgo");
		break;
	case 0x91:
		ext("ps|talkActor");
		break;
	case 0x92:
		ext("s|talkEgo");
		break;
	case 0x93:
		PRINT_V8("printLine");
		break;
	case 0x94:
		PRINT_V8("printCursor");
		break;
	case 0x95:
		PRINT_V8("printDebug");
		break;
	case 0x96:
		PRINT_V8("printSystem");
		break;
	case 0x97:
		PRINT_V8("blastText");
		break;

	case 0x9C:
		ext("x" "cursorCommand\0"
				"\xDC|cursorOn,"
				"\xDD|cursorOff,"
				"\xDE|userPutOn,"
				"\xDF|userPutOff,"
				"\xE0|softCursorOn,"
				"\xE1|softCursorOff,"
				"\xE2|softUserputOn,"
				"\xE3|softUserputOff,"
				"\xE4pp|setCursorImg,"
				"\xE5pp|setCursorHotspot,"
				"\xE6p|makeCursorColorTransparent,"
				"\xE7p|initCharset,"
				"\xE8l|charsetColors,"
		    		"\xE9pp|setCursorPosition");
		break;
	case 0x9D:
		ext("p|loadRoom");
		break;
	case 0x9E:
		ext("ppz|loadRoomWithEgo");
		break;
	case 0x9F:
		ext("ppp|walkActorToObj");
		break;
	case 0xA0:
		ext("ppp|walkActorTo");
		break;
	case 0xA1:
		ext("pppp|putActorInRoom");
		break;
	case 0xA2:
		ext("zp|putActorAtObject");
		break;
	case 0xA3:
		ext("pp|faceActor");
		break;
	case 0xA4:
		ext("pp|animateActor");
		break;
	case 0xA5:
		ext("ppp|doSentence");
		break;
	case 0xA6:
		ext("z|pickupObject");
		break;
	case 0xA7:
		ext("pl|setBoxFlags");
		break;
	case 0xA8:
		ext("|createBoxMatrix");
		break;

	case 0xAA:
		ext("x" "resourceRoutines\0"
				"\x3Cp|loadCharset,"
				"\x3Dp|loadCostume,"
				"\x3Ep|loadObject,"
				"\x3Fp|loadRoom,"
				"\x40p|loadScript,"
				"\x41p|loadSound,"
				"\x42p|lockCostume,"
				"\x43p|lockRoom,"
				"\x44p|lockScript,"
				"\x45p|lockSound,"
				"\x46p|unlockCostume,"
				"\x47p|unlockRoom,"
				"\x48p|unlockScript,"
				"\x49p|unlockSound,"
				"\x4Ap|nukeCostume,"
				"\x4Bp|nukeRoom,"
				"\x4Cp|nukeScript,"
				"\x4Dp|nukeSound"
				);
		break;
	case 0xAB:
		// FIXME - not sure how much stuff each subopcode pops
		ext("x" "roomOps\0"
				"\x52|setRoomPalette,"
				"\x55|setRoomIntensity,"
				"\x57p|fade,"
				"\x58ppppp|setRoomRBGIntensity,"
				"\x59|transformRoom,"
				"\x5A|colorCycleDelay,"
				"\x5B|copyPalette,"
				"\x5C|newPalette,"
				"\x5D|saveGame,"
				"\x5E|LoadGame,"
				"\x5F|setRoomSaturation"
				);
		break;
	case 0xAC:
		// Note: these are guesses and may partially be wrong
		ext("x" "actorOps\0"
				"\x64p|setActorCostume,"
				"\x65pp|setActorWalkSpeed,"
				"\x67|setActorDefAnim,"
				"\x68p|setActorInitFrame,"
				"\x69pp|setActorTalkFrame,"
				"\x6Ap|setActorWalkFrame,"
				"\x6Bp|setActorStandFrame,"
				"\x6C|setActorAnimSpeed,"
				"\x6D|setActorDefault,"	// = initActorLittle ?
				"\x6Ep|setActorElevation,"
				"\x6Fpp|setActorPalette,"
				"\x70p|setActorTalkColor,"
				"\x71s|setActorName,"
				"\x72p|setActorWidth,"
				"\x73p|setActorScale,"
				"\x74|setActorNeverZClip,"
				"\x75p|setActorAlwayZClip?,"
				"\x76|setActorIgnoreBoxes,"
				"\x77|setActorFollowBoxes,"
				"\x78p|actorSpecialDraw,"
				"\x79pp|setActorTalkPos,"
				"\x7Ap|setCurActor,"
				"\x7Bpp|setActorAnimVar,"
				"\x7C|setActorIgnoreTurnsOn,"
				"\x7D|setActorIgnoreTurnsOff,"
				"\x7E|newActor,"
				"\x7Fp|setActorLayer,"
				"\x80|setActorStanding,"
				"\x81p|setActorDirection,"
				"\x82p|actorTurnToDirection,"
				"\x83p|setActorWalkScript,"
				"\x84p|setTalkScript,"
				"\x85|freezeActor,"
				"\x86|unfreezeActor,"
				"\x87p|setActorVolume,"
				"\x88p|setActorFrequency,"
				"\x89p|setActorPan"
				);
		break;
	case 0xAD:
		ext("x" "cameraOps\0"
				"\x32|freezeCamera,"
				"\x33|unfreezeCamera"
				);
		break;
	case 0xAE:
		ext("x" "verbOps\0"
				"\x96p|verbInit,"
				"\x97|verbNew,"
				"\x98|verbDelete,"
				"\x99s|verbLoadString,"
				"\x9App|verbSetXY,"
				"\x9B|verbOn,"
				"\x9C|verbOff,"
				"\x9Dp|verbSetColor,"
				"\x9Ep|verbSetHiColor,"

				"\xA0p|verbSetDimColor,"
				"\xA1|verbSetDim,"
				"\xA2p|verbSetKey,"
				"\xA3p|verbLoadImg,"
				"\xA4p|verbSetToString,"
				"\xA5|verbSetCenter,"
				"\xA6p|verbSetCharset,"
				"\xA7p|verbSetLineSpacing"
				);
		break;
	case 0xAF:
		ext("p|startSound");
		break;

	case 0xB1:
		ext("p|stopSound");
		break;
	case 0xB2:
		ext("l|soundKludge");
		break;
	case 0xB3:
		ext("x" "system\0"
				"\x28|restart,"
				"\x29|quit");
		break;
	case 0xB4:
		ext("x" "saveRestoreVerbs\0"
				"\xB4ppp|saveVerbs,"
				"\xB5ppp|restoreVerbs,"
				"\xB6ppp|deleteVerbs");
		break;
	case 0xB5:
		ext("ps|setObjectName");
		break;
	case 0xB6:
		ext("|getDateTime");
		break;
	case 0xB7:
		ext("ppppp|drawBox");
		break;

	case 0xB9:
		ext("s|startVideo");
		break;
	case 0xBA:
		ext("y" "kernelSetFunctions\0"
				"\xB|lockObject,"
				"\xC|unlockObject,"
				"\xD|remapCostume,"
				"\xE|remapCostumeInsert,"
				"\xF|setVideoFrameRate,"

				"\x14|setBoxScale,"
				"\x15|setScaleSlot,"
				"\x16|setBannerColors,"
				"\x17|setActorChoreLimbFrame,"
				"\x18|clearTextQueue,"
				"\x19|saveGameWrite,"
				"\x1A|saveGameRead,"
				"\x1B|saveGameReadName,"
				"\x1C|saveGameStampScreenshot,"
				"\x1D|setKeyScript,"
				"\x1E|killAllScriptsButMe,"
				"\x1F|stopAllVideo,"
				"\x20|writeRegistryValue,"
				"\x21|paletteSetIntensity,"
				"\x22|queryQuit,"
				
				"\x6C|buildPaletteShadow,"
				"\x6D|setPaletteShadow,"

				"\x76|blastShadowObject,"
				"\x77|superBlastObject"
				);
		break;

	case 0xC8:
		ext("rlp|startScriptQuick");
		break;
	case 0xC9:
		ext("lppp|startObjectQuick");
		break;
	case 0xCA:
		ext("rlp|pickOneOf");
		break;
	case 0xCB:
		ext("rplp|pickOneOfDefault");
		break;

	case 0xCD:
		ext("rlp|isAnyOf");
		break;
	case 0xCE:
		ext("rp|getRandomNumber");
		break;
	case 0xCF:
		ext("rpp|getRandomNumberRange");
		break;
	case 0xD0:
		ext("rlp|ifClassOfIs");
		break;
	case 0xD1:
		ext("rp|getState");
		break;
	case 0xD2:
		ext("rp|getOwner");
		break;
	case 0xD3:
		ext("rp|isScriptRunning");
		break;

	case 0xD5:
		ext("rp|isSoundRunning");
		break;
	case 0xD6:
		ext("rp|abs");
		break;

	case 0xD8:
		ext("ry" "kernelGetFunctions\0"
				"\x73|getWalkBoxAt,"
				"\x74|isPointInBox,"
				"\xCE|getRGBSlot,"
				"\xD3|getKeyState,"
				"\xD7|getBox,"
				"\xD8|findBlastObject,"
				"\xD9|actorHit,"
				"\xDA|lipSyncWidth,"
				"\xDB|lipSyncHeight,"
				"\xDC|actorTalkAnimation,"
				"\xDD|getMasterSFXVol,"
				"\xDE|getMasterVoiceVol,"
				"\xDF|getMasterMusicVol,"
				"\xE0|readRegistryValue"
				);
		break;
	case 0xD9:
		ext("rpp|isActorInBox");
		break;
	case 0xDA:
		ext("rpp|getVerbEntrypoint");
		break;
	case 0xDB:
		ext("rpp|getActorFromXY");
		break;
	case 0xDC:
		ext("rpp|findObject");
		break;
	case 0xDD:
		ext("rpp|getVerbFromXY");
		break;

	case 0xDF:
		ext("rpp|findInventory");
		break;
	case 0xE0:
		ext("rp|getInventoryCount");
		break;
	case 0xE1:
		ext("rpp|getAnimateVariable");
		break;
	case 0xE2:
		ext("rp|getActorRoom");
		break;
	case 0xE3:
		ext("rp|getActorWalkBox");
		break;
	case 0xE4:
		ext("rp|getActorMoving");
		break;
	case 0xE5:
		ext("rp|getActorCostume");
		break;
	case 0xE6:
		ext("rp|getActorScaleX");
		break;
	case 0xE7:
		ext("rp|getActorLayer");
		break;
	case 0xE8:
		ext("rp|getActorElevation");
		break;
	case 0xE9:
		ext("rp|getActorWidth");
		break;
	case 0xEA:
		ext("rp|getObjectDir");
		break;
	case 0xEB:
		ext("rp|getObjectX");
		break;
	case 0xEC:
		ext("rp|getObjectY");
		break;
	case 0xED:
		ext("rp|getActorChore");
		break;
	case 0xEE:
		ext("rpp|getDistObjObj");
		break;
	case 0xEF:
		ext("rpppp|getDistPtPt");
		break;
	case 0xF0:
		ext("rp|getObjectImageX");
		break;
	case 0xF1:
		ext("rp|getObjectImageY");
		break;
	case 0xF2:
		ext("rp|getObjectImageWidth");
		break;
	case 0xF3:
		ext("rp|getObjectImageHeight");
		break;
	case 0xF4:
		ext("rp|getVerbX");
		break;
	case 0xF5:
		ext("rp|getVerbY");
		break;
	case 0xF6:
		ext("rps|stringWidth");
		break;
	case 0xF7:
		ext("rp|getActorZPlane");
		break;

	default:
		invalidop(NULL, code);
		break;
	}
}

#define PRINT_V67(name)           \
	do {                          \
		ext("x" name "\0"         \
				"\x41pp|XY,"      \
				"\x42p|color,"    \
				"\x43p|right,"    \
				"\x45|center,"    \
				"\x47|left,"      \
				"\x48|overhead,"  \
				"\x4A|mumble,"    \
				"\x4Bs|msg,"      \
				"\xFE|begin,"     \
				"\xFF|end"        \
				);                \
	} while(0)

void DeScumm::next_line_V67()
{
	byte code = get_byte();
	StackEnt *se_a, *se_b;

	switch (code) {
	case 0x0:
		push(se_int(get_byte()));
		break;
	case 0x1:
		push(se_int(get_word()));
		break;
	case 0x2:
		push(se_var(get_byte()));
		break;
	case 0x3:
		push(se_var(get_word()));
		break;
	case 0x6:
		push(se_array(get_byte(), NULL, pop()));
		break;
	case 0x7:
		push(se_array(get_word(), NULL, pop()));
		break;
	case 0xA:
		se_a = pop();
		push(se_array(get_byte(), pop(), se_a));
		break;
	case 0xB:
		se_a = pop();
		push(se_array(get_word(), pop(), se_a));
		break;
	case 0xC:
		se_a = dup(pop());
		push(se_a);
		push(se_a);
		break;
	case 0xD:
		push(se_oper(pop(), isZero));
		break;
	case 0xE:
	case 0xF:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
		se_a = pop();
		se_b = pop();
		push(se_oper(se_b, (code - 0xE) + isEqual, se_a));
		break;
	case 0x1A:
	case 0xA7:
		kill(pop());
		break;
	case 0x42:
		writeVar(get_byte(), pop());
		break;
	case 0x43:
		writeVar(get_word(), pop());
		break;
	case 0x46:
		writeArray(get_byte(), NULL, pop(), pop());
		break;
	case 0x47:
		writeArray(get_word(), NULL, pop(), pop());
		break;
	case 0x4A:
		writeArray(get_byte(), pop(), pop(), pop());
		break;
	case 0x4B:
		writeArray(get_word(), pop(), pop(), pop());
		break;
	case 0x4E:
		addVar(get_byte(), 1);
		break;
	case 0x4F:
		addVar(get_word(), 1);
		break;
	case 0x52:
		addArray(get_byte(), pop(), 1);
		break;
	case 0x53:
		addArray(get_word(), pop(), 1);
		break;
	case 0x56:
		addVar(get_byte(), -1);
		break;
	case 0x57:
		addVar(get_word(), -1);
		break;
	case 0x5A:
		addArray(get_byte(), pop(), -1);
		break;
	case 0x5B:
		addArray(get_word(), pop(), -1);
		break;
	case 0x5C:
		jumpif(pop(), true);
		break;
	case 0x5D:
		jumpif(pop(), false);
		break;
	case 0x5E:
		ext("lpp|startScriptEx");
		break;
	case 0x5F:
		ext("lp|startScript");
		break;
	case 0x60:
		ext("lppp|startObject");
		break;
	case 0x61:
		ext("pp|setObjectState");
		break;
	case 0x62:
		ext("ppp|setObjectXY");
		break;
		/* *** */
	case 0x65:
		ext("|stopObjectCodeA");
		break;
	case 0x66:
		ext("|stopObjectCodeB");
		break;
	case 0x67:
		ext("|endCutscene");
		break;
	case 0x68:
		ext("l|beginCutscene");
		break;
	case 0x69:
		ext("|stopMusic");
		break;
	case 0x6A:
		ext("p|freezeUnfreeze");
		break;
	case 0x6B:
		ext("x" "cursorCommand\0"
				"\x90|cursorOn,"
				"\x91|cursorOff,"
				"\x92|userPutOn,"
				"\x93|userPutOff,"
				"\x94|softCursorOn,"
				"\x95|softCursorOff,"
				"\x96|softUserputOn,"
				"\x97|softUserputOff,"
				"\x99z|setCursorImg,"
				"\x9App|setCursorHotspot,"
				"\x9Cp|initCharset,"
				"\x9Dl|charsetColors,"
				"\xD6p|makeCursorColorTransparent");
		break;
	case 0x6C:
		ext("|break");
		break;
	case 0x6D:
		ext("rlp|ifClassOfIs");
		break;
	case 0x6E:
		ext("lp|setClass");
		break;
	case 0x6F:
		ext("rp|getState");
		break;
	case 0x70:
		ext("pp|setState");
		break;
	case 0x71:
		ext("pp|setOwner");
		break;
	case 0x72:
		ext("rp|getOwner");
		break;
	case 0x73:
		jump();
		break;
	case 0x74:
		if (HumongousFlag) {
			ext("pp|startSound");
			break;
		}
		ext("p|startSound");
		break;
	case 0x75:
		ext("p|stopSound");
		break;
	case 0x76:
		ext("p|startMusic");
		break;
	case 0x77:
		ext("p|stopObjectScript");
		break;
	case 0x78:
		if (scriptVersion < 7)
			ext("p|panCameraTo");
		else
			ext("pp|panCameraTo");
		break;
	case 0x79:
		ext("p|actorFollowCamera");
		break;
	case 0x7A:
		if (scriptVersion < 7)
			ext("p|setCameraAt");
		else
			ext("pp|setCameraAt");
		break;
	case 0x7B:
		ext("p|loadRoom");
		break;
	case 0x7C:
		ext("p|stopScript");
		break;
	case 0x7D:
		ext("ppp|walkActorToObj");
		break;
	case 0x7E:
		ext("ppp|walkActorTo");
		break;
	case 0x7F:
		ext("pppp|putActorInRoom");
		break;
	case 0x80:
		ext("zp|putActorAtObject");
		break;
	case 0x81:
		ext("pp|faceActor");
		break;
	case 0x82:
		ext("pp|animateActor");
		break;
	case 0x83:
		ext("pppp|doSentence");
		break;
	case 0x84:
		ext("z|pickupObject");
		break;
	case 0x85:
		ext("ppz|loadRoomWithEgo");
		break;
	case 0x87:
		ext("rp|getRandomNumber");
		break;
	case 0x88:
		ext("rpp|getRandomNumberRange");
		break;
	case 0x8A:
		ext("rp|getActorMoving");
		break;
	case 0x8B:
		ext("rp|isScriptRunning");
		break;
	case 0x8C:
		ext("rp|getActorRoom");
		break;
	case 0x8D:
		ext("rp|getObjectX");
		break;
	case 0x8E:
		ext("rp|getObjectY");
		break;
	case 0x8F:
		ext("rp|getObjectDir");
		break;
	case 0x90:
		ext("rp|getActorWalkBox");
		break;
	case 0x91:
		ext("rp|getActorCostume");
		break;
	case 0x92:
		ext("rpp|findInventory");
		break;
	case 0x93:
		ext("rp|getInventoryCount");
		break;
	case 0x94:
		ext("rpp|getVerbFromXY");
		break;
	case 0x95:
		ext("|beginOverride");
		break;
	case 0x96:
		ext("|endOverride");
		break;
	case 0x97:
		ext("ps|setObjectName");
		break;
	case 0x98:
		ext("rp|isSoundRunning");
		break;
	case 0x99:
		ext("pl|setBoxFlags");
		break;
	case 0x9A:
		ext("|createBoxMatrix");
		break;
	case 0x9B:
		ext("x" "resourceRoutines\0"
				"\x64p|loadScript,"
				"\x65p|loadSound,"
				"\x66p|loadCostume," 
				"\x67p|loadRoom,"
				"\x68p|nukeScript," 
				"\x69p|nukeSound," 
				"\x6Ap|nukeCostume,"
				"\x6Bp|nukeRoom," 
				"\x6Cp|lockScript," 
				"\x6Dp|lockSound," 
				"\x6Ep|lockCostume,"
				"\x6Fp|lockRoom," 
				"\x70p|unlockScript," 
				"\x71p|unlockSound," 
				"\x72p|unlockCostume,"
				"\x73p|unlockRoom," 
				"\x75p|loadCharset," 
				"\x76p|nukeCharset," 
				"\x77z|loadFlObject");
		break;
	case 0x9C:
		ext("x" "roomOps\0"
				"\xACpp|roomScroll,"
				"\xAEpp|setScreen,"
				"\xAFpppp|setPalColor,"
				"\xB0|shakeOn,"
				"\xB1|shakeOff,"
				"\xB3ppp|darkenPalette,"
				"\xB4pp|saveLoadThing,"
				"\xB5p|screenEffect,"
				"\xB6ppppp|darkenPalette,"
				"\xB7ppppp|setupShadowPalette,"
				"\xBApppp|palManipulate,"
				"\xBBpp|colorCycleDelay,"
				"\xD5p|setPalette");
		break;
	case 0x9D:
		ext("x" "actorSet\0"
				"\xC5p|setCurActor,"
				"\x4Cp|setActorCostume,"
				"\x4Dpp|setActorWalkSpeed,"
				"\x4El|setActorSound,"
				"\x4Fp|setActorWalkFrame,"
				"\x50pp|setActorTalkFrame,"
				"\x51p|setActorStandFrame,"
				"\x52ppp|actorSet:82:??,"
				"\x53|initActor,"
				"\x54p|setActorElevation,"
				"\x55|setActorDefAnim,"
				"\x56pp|setActorPalette,"
				"\x57p|setActorTalkColor,"
				"\x58s|setActorName,"
				"\x59p|setActorInitFrame,"
				"\x5Bp|setActorWidth,"
				"\x5Cp|setActorScale,"
				"\x5D|setActorNeverZClip,"
				"\x5Ep|setActorAlwayZClip?,"
				"\xE1p|setActorAlwayZClip?,"
				"\x5F|setActorIgnoreBoxes,"
				"\x60|setActorFollowBoxes,"
				"\x61|setActorAnimSpeed,"
				"\x62|setActorShadowMode,"
				"\x63pp|setActorTalkPos,"
				"\xC6p|setActorAnimVar,"
				"\xD7|setActorIgnoreTurnsOn,"
				"\xD8|setActorIgnoreTurnsOff,"
				"\xD9|initActorLittle,"
				"\xE3p|setActorLayer,"
				"\xE4p|setActorWalkScript,"
				"\xE5|setActorStanding,"
				"\xE6p|setActorDirection,"
				"\xE7p|actorTurnToDirection,"
				"\xE9|freezeActor,"
				"\xEA|unfreezeActor,"
				"\xEBp|setTalkScript");
		break;
	case 0x9E:
		ext("x" "verbOps\0"
				"\xC4p|setCurVerb,"
				"\x7Cp|verbLoadImg,"
				"\x7Ds|verbLoadString,"
				"\x7Ep|verbSetColor,"
				"\x7Fp|verbSetHiColor,"
				"\x80pp|verbSetXY,"
				"\x81|verbSetCurmode1,"	// = verbOn ?
				"\x82|verbSetCurmode0,"	// = verbOff ?
				"\x83|verbKill,"
				"\x84|verbInit,"
				"\x85p|verbSetDimColor,"
				"\x86|verbSetCurmode2,"	// = verbDim ?
				"\x87p|verbSetKey,"
				"\x88|verbSetCenter,"
				"\x89p|verbSetToString,"
				"\x8Bpp|verbSetToObject,"
				"\x8Cp|verbSetBkColor,"
				"\xFF|verbRedraw");
		break;
	case 0x9F:
		ext("rpp|getActorFromXY");
		break;
	case 0xA0:
		ext("rpp|findObject");
		break;
	case 0xA1:
		ext("lp|pseudoRoom");
		break;
	case 0xA2:
		ext("rp|getActorElevation");
		break;
	case 0xA3:
		ext("rpp|getVerbEntrypoint");
		break;
	case 0xA4:
		ext("x" "arrayOps\0" "\xCDwps|arrayOps205," "\xD0wpl|arrayOps208," "\xD4wplp|arrayOps212");
		break;
	case 0xA5:
		ext("x" "saveRestoreVerbs\0"
				"\x8Dppp|saveVerbs,"
				"\x8Eppp|restoreVerbs,"
				"\x8Fppp|deleteVerbs");
		break;
	case 0xA6:
		ext("ppppp|drawBox");
		break;
	case 0xA8:
		ext("rp|getActorWidth");
		break;
	case 0xA9:
		ext("x" "wait\0"
				"\xA8pj|waitForActor,"
				"\xA9|waitForMessage,"
				"\xAA|waitForCamera,"
				"\xAB|waitForSentence,"
				"\xE2pj|waitUntilActorDrawn,"
				"\xE8pj|waitUntilActorTurned,"
				);
		break;
	case 0xAA:
		ext("rp|getActorScaleX");
		break;
	case 0xAB:
		ext("rp|getActorAnimCounter1");
		break;
	case 0xAC:
		ext("l|soundKludge");
		break;
	case 0xAD:
		ext("rlp|isAnyOf");
		break;
	case 0xAE:
		ext("x" "quitPauseRestart\0" "\x9E|pauseGame," "\xA0|shutDown");
		break;
	case 0xAF:
		ext("rp|isActorInBox");
		break;
	case 0xB0:
		ext("p|delay");
		break;
	case 0xB1:
		ext("p|delaySeconds");
		break;
	case 0xB2:
		ext("p|delayMinutes");
		break;
	case 0xB3:
		ext("|stopSentence");
		break;
	case 0xB4:
		PRINT_V67("printLine");
		break;
	case 0xB5:
		PRINT_V67("printCursor");
		break;
	case 0xB6:
		PRINT_V67("printDebug");
		break;
	case 0xB7:
		PRINT_V67("printSystem");
		break;
	case 0xB8:
		// This is *almost* identical to the other print opcodes, only the 'begine' subop differs
		ext("x" "printActor\0"
				"\x41pp|XY,"
				"\x42p|color,"
				"\x43p|right,"
				"\x45|center,"
				"\x47|left," "\x48|overhead," "\x4A|new3," "\x4Bs|msg," "\xFEp|begin," "\xFF|end");
		break;
	case 0xB9:
		PRINT_V67("printEgo");
		break;
	case 0xBA:
		ext("ps|talkActor");
		break;
	case 0xBB:
		ext("s|talkEgo");
		break;
	case 0xBC:
		ext("x" "dim\0"
				"\xC7pw|dimType5,"
				"\xC8pw|dimType1,"
				"\xC9pw|dimType2,"
				"\xCApw|dimType3,"
				"\xCBpw|dimType4,"
				"\xCCw|nukeArray");
		break;
	case 0xBE:
		ext("lpp|startObjectQuick");
		break;
	case 0xBF:
		ext("lp|startScriptQuick");
		break;
	case 0xC0:
		ext("x" "dim2\0"
				"\xC7ppw|dim2Type5,"
				"\xC8ppw|dim2Type1,"
				"\xC9ppw|dim2Type2,"
				"\xCAppw|dim2Type3,"
				"\xCBppw|dim2Type4");
		break;
	case 0xC4:
		ext("rp|abs");
		break;
	case 0xC5:
		ext("rpp|getDistObjObj");
		break;
	case 0xC6:
		ext("rppp|getDistObjPt");
		break;
	case 0xC7:
		ext("rpppp|getDistPtPt");
		break;
	case 0xC8:
		// TODO - add more subopcodes
		ext("ry" "kernelGetFunctions\0"
				"\x73|getWalkBoxAt,"
				"\x74|isPointInBox,"
				"\xCE|getRGBSlot"
				);
		break;
	case 0xC9:
		// TODO - add more subopcodes
		ext("y" "kernelSetFunctions\0"
				"\x4|grabCursor,"
				"\x5|fadeOut,"
				"\x6|startVideo,"
				"\xC|setCursorImg,"
				"\xD|remapCostume,"
				"\xE|remapCostumeInsert,"
				"\xF|setVideoFrameRate,"

				"\x6C|buildPaletteShadow,"
				"\x6D|setPaletteShadow,"

				"\x76|blastShadowObject,"
				"\x77|superBlastObject"
				);
		break;
	case 0xCA:
		ext("p|breakXTimes");
		break;
	case 0xCB:
		ext("rlp|pickOneOf");
		break;
	case 0xCC:
		ext("rplp|pickOneOfDefault");
		break;
	case 0xCD:
		ext("pppp|stampObject");
		break;
	case 0xD0:
		ext("|getDateTime");
		break;
	case 0xD1:
		ext("|stopTalking");
		break;
	case 0xD2:
		ext("rpp|getAnimateVariable");
		break;
	case 0xD4:
		ext("wpp|shuffle");
		break;
	case 0xD5:
		ext("lpp|jumpToScript");
		break;
	case 0xD6:
		se_a = pop();
		se_b = pop();
		push(se_oper(se_b, operBand, se_a));
		break;
	case 0xD7:
		se_a = pop();
		se_b = pop();
		push(se_oper(se_b, operBor, se_a));
		break;
	case 0xD8:
		ext("rp|isRoomScriptRunning");
		break;
	case 0xD9:
		ext("p|closeFile");
		break;
	case 0xDA:
		ext("rsp|openFile");
		break;
	case 0xDB:
		ext("rpp|readFile");
		break;
	case 0xDD:
		ext("rp|findAllObjects");
		break;
	case 0xE0:
		ext("x" "p|unknownEO\0" 
			"\xDEp|unknownE0-DE,"
			"\xDC|unknownE0-DC");
		break;
	case 0xE1:
		ext("rp|unknownE1");
		break;
	case 0xE2:
		ext("p|localizeArray");
		break;
	case 0xDE:
		ext("s|deleteFile");
		break;
	case 0xEC:
		ext("rp|getActorLayer");
		break;
	case 0xED:
		ext("rp|getObjectNewDir");
		break;
	case 0xF3:
		ext("rsp|readINI");
		break;
	case 0xFA:
		get_byte();
		ext("s|unknownFA");
		break;
	default:
		invalidop(NULL, code);
		break;
	}
}


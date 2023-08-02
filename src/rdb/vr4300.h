/*
 * Copyright (c) 2022, 2023 glank
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef VR4300_H
#define VR4300_H

#include <ultra64.h>

#define VR4300_REG_CPU_FIRST	VR4300_REG_R0
#define VR4300_REG_CPU_LAST	VR4300_REG_RA
#define VR4300_REG_CP0_FIRST	VR4300_REG_INDEX
#define VR4300_REG_CP0_LAST	VR4300_REG_INVD6
#define VR4300_REG_CP1_FIRST	VR4300_REG_F0
#define VR4300_REG_CP1_LAST	VR4300_REG_F31
#define VR4300_REG_FCR_FIRST	VR4300_REG_FCR0
#define VR4300_REG_FCR_LAST	VR4300_REG_FCR31

enum vr4300_reg
{
	/* cpu regs */
	VR4300_REG_R0,
	VR4300_REG_AT,
	VR4300_REG_V0,
	VR4300_REG_V1,
	VR4300_REG_A0,
	VR4300_REG_A1,
	VR4300_REG_A2,
	VR4300_REG_A3,
	VR4300_REG_T0,
	VR4300_REG_T1,
	VR4300_REG_T2,
	VR4300_REG_T3,
	VR4300_REG_T4,
	VR4300_REG_T5,
	VR4300_REG_T6,
	VR4300_REG_T7,
	VR4300_REG_S0,
	VR4300_REG_S1,
	VR4300_REG_S2,
	VR4300_REG_S3,
	VR4300_REG_S4,
	VR4300_REG_S5,
	VR4300_REG_S6,
	VR4300_REG_S7,
	VR4300_REG_T8,
	VR4300_REG_T9,
	VR4300_REG_K0,
	VR4300_REG_K1,
	VR4300_REG_GP,
	VR4300_REG_SP,
	VR4300_REG_S8,
	VR4300_REG_RA,
	/* cop0 regs */
	VR4300_REG_INDEX,
	VR4300_REG_RANDOM,
	VR4300_REG_ENTRYLO0,
	VR4300_REG_ENTRYLO1,
	VR4300_REG_CONTEXT,
	VR4300_REG_PAGEMASK,
	VR4300_REG_WIRED,
	VR4300_REG_INVD0,
	VR4300_REG_BADVADDR,
	VR4300_REG_COUNT,
	VR4300_REG_ENTRYHI,
	VR4300_REG_COMPARE,
	VR4300_REG_STATUS,
	VR4300_REG_CAUSE,
	VR4300_REG_EPC,
	VR4300_REG_PRID,
	VR4300_REG_CONFIG,
	VR4300_REG_LLADDR,
	VR4300_REG_WATCHLO,
	VR4300_REG_WATCHHI,
	VR4300_REG_XCONTEXT,
	VR4300_REG_INVD1,
	VR4300_REG_INVD2,
	VR4300_REG_INVD3,
	VR4300_REG_INVD4,
	VR4300_REG_INVD5,
	VR4300_REG_PERR,
	VR4300_REG_CACHEERR,
	VR4300_REG_TAGLO,
	VR4300_REG_TAGHI,
	VR4300_REG_ERROREPC,
	VR4300_REG_INVD6,
	/* cop1 regs */
	VR4300_REG_F0,
	VR4300_REG_F1,
	VR4300_REG_F2,
	VR4300_REG_F3,
	VR4300_REG_F4,
	VR4300_REG_F5,
	VR4300_REG_F6,
	VR4300_REG_F7,
	VR4300_REG_F8,
	VR4300_REG_F9,
	VR4300_REG_F10,
	VR4300_REG_F11,
	VR4300_REG_F12,
	VR4300_REG_F13,
	VR4300_REG_F14,
	VR4300_REG_F15,
	VR4300_REG_F16,
	VR4300_REG_F17,
	VR4300_REG_F18,
	VR4300_REG_F19,
	VR4300_REG_F20,
	VR4300_REG_F21,
	VR4300_REG_F22,
	VR4300_REG_F23,
	VR4300_REG_F24,
	VR4300_REG_F25,
	VR4300_REG_F26,
	VR4300_REG_F27,
	VR4300_REG_F28,
	VR4300_REG_F29,
	VR4300_REG_F30,
	VR4300_REG_F31,
	/* cop1 control regs */
	VR4300_REG_FCR0,
	VR4300_REG_FCR1,
	VR4300_REG_FCR2,
	VR4300_REG_FCR3,
	VR4300_REG_FCR4,
	VR4300_REG_FCR5,
	VR4300_REG_FCR6,
	VR4300_REG_FCR7,
	VR4300_REG_FCR8,
	VR4300_REG_FCR9,
	VR4300_REG_FCR10,
	VR4300_REG_FCR11,
	VR4300_REG_FCR12,
	VR4300_REG_FCR13,
	VR4300_REG_FCR14,
	VR4300_REG_FCR15,
	VR4300_REG_FCR16,
	VR4300_REG_FCR17,
	VR4300_REG_FCR18,
	VR4300_REG_FCR19,
	VR4300_REG_FCR20,
	VR4300_REG_FCR21,
	VR4300_REG_FCR22,
	VR4300_REG_FCR23,
	VR4300_REG_FCR24,
	VR4300_REG_FCR25,
	VR4300_REG_FCR26,
	VR4300_REG_FCR27,
	VR4300_REG_FCR28,
	VR4300_REG_FCR29,
	VR4300_REG_FCR30,
	VR4300_REG_FCR31,
	/* pseudo-registers */
	VR4300_REG_LO,
	VR4300_REG_HI,
	VR4300_REG_CC,
	VR4300_REG_MAX
};

enum vr4300_op
{
	VR4300_OP_ABSD,
	VR4300_OP_ABSS,
	VR4300_OP_ADD,
	VR4300_OP_ADDD,
	VR4300_OP_ADDI,
	VR4300_OP_ADDIU,
	VR4300_OP_ADDS,
	VR4300_OP_ADDU,
	VR4300_OP_AND,
	VR4300_OP_ANDI,
	VR4300_OP_BC1F,
	VR4300_OP_BC1FL,
	VR4300_OP_BC1T,
	VR4300_OP_BC1TL,
	VR4300_OP_BEQ,
	VR4300_OP_BEQL,
	VR4300_OP_BGEZ,
	VR4300_OP_BGEZAL,
	VR4300_OP_BGEZALL,
	VR4300_OP_BGEZL,
	VR4300_OP_BGTZ,
	VR4300_OP_BGTZL,
	VR4300_OP_BLEZ,
	VR4300_OP_BLEZL,
	VR4300_OP_BLTZ,
	VR4300_OP_BLTZAL,
	VR4300_OP_BLTZALL,
	VR4300_OP_BLTZL,
	VR4300_OP_BNE,
	VR4300_OP_BNEL,
	VR4300_OP_BREAK,
	VR4300_OP_CACHE,
	VR4300_OP_CEILLD,
	VR4300_OP_CEILLS,
	VR4300_OP_CEILWD,
	VR4300_OP_CEILWS,
	VR4300_OP_CEQD,
	VR4300_OP_CEQS,
	VR4300_OP_CFC1,
	VR4300_OP_CFD,
	VR4300_OP_CFS,
	VR4300_OP_CLED,
	VR4300_OP_CLES,
	VR4300_OP_CLTD,
	VR4300_OP_CLTS,
	VR4300_OP_CNGED,
	VR4300_OP_CNGES,
	VR4300_OP_CNGLD,
	VR4300_OP_CNGLED,
	VR4300_OP_CNGLES,
	VR4300_OP_CNGLS,
	VR4300_OP_CNGTD,
	VR4300_OP_CNGTS,
	VR4300_OP_COLED,
	VR4300_OP_COLES,
	VR4300_OP_COLTD,
	VR4300_OP_COLTS,
	VR4300_OP_CSEQD,
	VR4300_OP_CSEQS,
	VR4300_OP_CSFD,
	VR4300_OP_CSFS,
	VR4300_OP_CTC1,
	VR4300_OP_CUEQD,
	VR4300_OP_CUEQS,
	VR4300_OP_CULED,
	VR4300_OP_CULES,
	VR4300_OP_CULTD,
	VR4300_OP_CULTS,
	VR4300_OP_CUND,
	VR4300_OP_CUNS,
	VR4300_OP_CVTDL,
	VR4300_OP_CVTDS,
	VR4300_OP_CVTDW,
	VR4300_OP_CVTLD,
	VR4300_OP_CVTLS,
	VR4300_OP_CVTSD,
	VR4300_OP_CVTSL,
	VR4300_OP_CVTSW,
	VR4300_OP_CVTWD,
	VR4300_OP_CVTWS,
	VR4300_OP_DADD,
	VR4300_OP_DADDI,
	VR4300_OP_DADDIU,
	VR4300_OP_DADDU,
	VR4300_OP_DDIV,
	VR4300_OP_DDIVU,
	VR4300_OP_DIV,
	VR4300_OP_DIVD,
	VR4300_OP_DIVS,
	VR4300_OP_DIVU,
	VR4300_OP_DMFC1,
	VR4300_OP_DMTC1,
	VR4300_OP_DMULT,
	VR4300_OP_DMULTU,
	VR4300_OP_DSLL,
	VR4300_OP_DSLL32,
	VR4300_OP_DSLLV,
	VR4300_OP_DSRA,
	VR4300_OP_DSRA32,
	VR4300_OP_DSRAV,
	VR4300_OP_DSRL,
	VR4300_OP_DSRL32,
	VR4300_OP_DSRLV,
	VR4300_OP_DSUB,
	VR4300_OP_DSUBU,
	VR4300_OP_ERET,
	VR4300_OP_FLOORLD,
	VR4300_OP_FLOORLS,
	VR4300_OP_FLOORWD,
	VR4300_OP_FLOORWS,
	VR4300_OP_J,
	VR4300_OP_JAL,
	VR4300_OP_JALR,
	VR4300_OP_JR,
	VR4300_OP_LB,
	VR4300_OP_LBU,
	VR4300_OP_LD,
	VR4300_OP_LDC1,
	VR4300_OP_LDL,
	VR4300_OP_LDR,
	VR4300_OP_LH,
	VR4300_OP_LHU,
	VR4300_OP_LL,
	VR4300_OP_LLD,
	VR4300_OP_LUI,
	VR4300_OP_LW,
	VR4300_OP_LWC1,
	VR4300_OP_LWL,
	VR4300_OP_LWR,
	VR4300_OP_LWU,
	VR4300_OP_MFC0,
	VR4300_OP_MFC1,
	VR4300_OP_MFHI,
	VR4300_OP_MFLO,
	VR4300_OP_MOVD,
	VR4300_OP_MOVS,
	VR4300_OP_MTC0,
	VR4300_OP_MTC1,
	VR4300_OP_MTHI,
	VR4300_OP_MTLO,
	VR4300_OP_MULD,
	VR4300_OP_MULS,
	VR4300_OP_MULT,
	VR4300_OP_MULTU,
	VR4300_OP_NEGD,
	VR4300_OP_NEGS,
	VR4300_OP_NOR,
	VR4300_OP_OR,
	VR4300_OP_ORI,
	VR4300_OP_ROUNDLD,
	VR4300_OP_ROUNDLS,
	VR4300_OP_ROUNDWD,
	VR4300_OP_ROUNDWS,
	VR4300_OP_SB,
	VR4300_OP_SC,
	VR4300_OP_SCD,
	VR4300_OP_SD,
	VR4300_OP_SDC1,
	VR4300_OP_SDL,
	VR4300_OP_SDR,
	VR4300_OP_SH,
	VR4300_OP_SLL,
	VR4300_OP_SLLV,
	VR4300_OP_SLT,
	VR4300_OP_SLTI,
	VR4300_OP_SLTIU,
	VR4300_OP_SLTU,
	VR4300_OP_SQRTD,
	VR4300_OP_SQRTS,
	VR4300_OP_SRA,
	VR4300_OP_SRAV,
	VR4300_OP_SRL,
	VR4300_OP_SRLV,
	VR4300_OP_SUB,
	VR4300_OP_SUBD,
	VR4300_OP_SUBS,
	VR4300_OP_SUBU,
	VR4300_OP_SW,
	VR4300_OP_SWC1,
	VR4300_OP_SWL,
	VR4300_OP_SWR,
	VR4300_OP_SYNC,
	VR4300_OP_SYSCALL,
	VR4300_OP_TEQ,
	VR4300_OP_TEQI,
	VR4300_OP_TGE,
	VR4300_OP_TGEI,
	VR4300_OP_TGEIU,
	VR4300_OP_TGEU,
	VR4300_OP_TLBP,
	VR4300_OP_TLBR,
	VR4300_OP_TLBWI,
	VR4300_OP_TLBWR,
	VR4300_OP_TLT,
	VR4300_OP_TLTI,
	VR4300_OP_TLTIU,
	VR4300_OP_TLTU,
	VR4300_OP_TNE,
	VR4300_OP_TNEI,
	VR4300_OP_TRUNCLD,
	VR4300_OP_TRUNCLS,
	VR4300_OP_TRUNCWD,
	VR4300_OP_TRUNCWS,
	VR4300_OP_XOR,
	VR4300_OP_XORI
};

enum vr4300_opnd_type
{
	VR4300_OPND_NULL,
	VR4300_OPND_CPU,
	VR4300_OPND_CP0,
	VR4300_OPND_CP1,
	VR4300_OPND_FCR,
	VR4300_OPND_JUMP,
	VR4300_OPND_BRANCH,
	VR4300_OPND_OFFSET,
	VR4300_OPND_IMMEDIATE,
	VR4300_OPND_CACHE
};

struct vr4300_insn
{
	enum vr4300_op		opcode;
	enum vr4300_opnd_type	opnd_type[3];
	s32			opnd_value[3];
};

int vr4300_decode_insn(u32 code, struct vr4300_insn *insn);

#endif

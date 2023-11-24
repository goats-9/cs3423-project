#pragma once

class any;
class pos;

/* Arithmetic starts */

// ADD
any ADD(const any &a, const any &b, const pos &p);

// SUB
any SUB(const any &a, const any &b, const pos &p);

// MUL
any MUL(const any &a, const any &b, const pos &p);

// DIV
any DIV(const any &a, const any &b, const pos &p);

// MOD
any MOD(const any &a, const any &b, const pos &p);

// POW
any POW(const any &a, const any &b, const pos &p);

/* Arithmetic ends */

/* Comparison starts */

// Equal
any EQ(const any &a, const any &b, const pos &p);

// Not Equal
any NE(const any &a, const any &b, const pos &p);

// Greater Than
any GT(const any &a, const any &b, const pos &p);

// Less Than
any LT(const any &a, const any &b, const pos &p);

// Greater Than or Equal
any GTE(const any &a, const any &b, const pos &p);

// Less Than or Equal
any LTE(const any &a, const any &b, const pos &p);

/* Comparison ends */

/* Bitwise starts */

// Bitwise OR
any BOR(const any &a, const any &b, const pos &p);

// Bitwise AND
any BAND(const any &a, const any &b, const pos &p);

// Bitwise XOR
any BXOR(const any &a, const any &b, const pos &p);

// Bitwise NOT
any BNOT(const any &a, const pos &p);

// Bitwise Left-Shift
any BLS(const any &a, const any &b, const pos &p);

// Bitwise Right-Shift
any BRS(const any &a, const any &b, const pos &p);

/* Bitwise ends */

// TYPEOF
any TYPEOF(const any &a,const pos &p);

// DISP
any DISP(const any &a,const pos &p);

any sum(any &tab1);
any minimum(any &tab1);
any maximum(any &tab1);
any average(any &tab1);
any product(any &tab1);
any count(any &tab1);
any ceiling(any &tab1);
any floor(any &tab1);
any modulus(any &tab1, any &N);
any power(any &tab1, any &N);

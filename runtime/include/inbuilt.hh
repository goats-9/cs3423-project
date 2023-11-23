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
any BNOT(const any &a, const any &b, const pos &p);

// Bitwise Left-Shift
any BLS(const any &a, const any &b, const pos &p);

// Bitwise Right-Shift
any BRS(const any &a, const any &b, const pos &p);

/* Bitwise ends */

// TYPEOF
any TYPEOF(const any &a,const pos &p);

// DISP
any DISP(const any &a,const pos &p);

any sum(any &r1, any &r2);
any minimum(any &r1, any &r2);
any maximum(any &r1, any &r2);
any product(any &r1, any &r2);
any average(any &r1, any &r2);
any count(any &r1, any &r2);
any ceiling(any &r1, any &r2);
any floor(any &r1, any &r2);
any modulus(any &r1, any &r2, any &r3);
any power(any &r1, any &r2, any &r3);
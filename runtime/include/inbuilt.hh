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
any TYPEOF(const any &a, const pos &p);

// DISP
any DISP(const any &a, const pos &p);

any SUM(any &tab1, const pos &p);
any MINIMUM(any &tab1, const pos &p);
any MAXIMUM(any &tab1, const pos &p);
any AVERAGE(any &tab1, const pos &p);
any PRODUCT(any &tab1, const pos &p);
any COUNT(any &tab1, const pos &p);
any CEILING(any &tab1, const pos &p);
any FLOOR(any &tab1, const pos &p);
any MODULUS(any &tab1, any &K, const pos &p);
any POWER(any &tab1, any &K, const pos &p);

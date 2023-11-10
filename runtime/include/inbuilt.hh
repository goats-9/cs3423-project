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
any EQ(any &a, const any &b, const pos &p);

// Not Equal
any NE(any &a, const any &b, const pos &p);

// Greater Than
any GT(any &a, const any &b, const pos &p);

// Less Than
any LT(any &a, const any &b, const pos &p);

// Greater Than or Equal
any GTE(any &a, const any &b, const pos &p);

// Less Than or Equal
any LTE(any &a, const any &b, const pos &p);

/* Comparison ends */

// DISP
any DISP(const any &a);
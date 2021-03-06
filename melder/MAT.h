#pragma once
/* MAT.h
 *
 * Copyright (C) 2017,2018 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, double number) noexcept { \
		integer mindim = ( target.rowStride < target.colStride ? 1 : 2 ); \
		if (mindim == 1) \
			for (integer icol = 1; icol <= target.ncol; icol ++) \
				for (integer irow = 1; irow <= target.nrow; irow ++) \
					target [irow] [icol] op number; \
		else \
			for (integer irow = 1; irow <= target.nrow; irow ++) \
				for (integer icol = 1; icol <= target.ncol; icol ++) \
					target [irow] [icol] op number; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION

#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, constMATVU const& x) { \
		Melder_assert (target.nrow == x.nrow); \
		Melder_assert (target.ncol == x.ncol); \
		integer mindim = ( target.rowStride < target.colStride ? 1 : 2 ); \
		if (mindim == 1) \
			for (integer icol = 1; icol <= target.ncol; icol ++) \
				for (integer irow = 1; irow <= target.nrow; irow ++) \
					target [irow] [icol] op x [irow] [icol]; \
		else \
			for (integer irow = 1; irow <= target.nrow; irow ++) \
				for (integer icol = 1; icol <= target.ncol; icol ++) \
					target [irow] [icol] op x [irow] [icol]; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION

#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, constVECVU const& x) { \
		Melder_assert (target.ncol == x.size); \
		integer mindim = ( target.rowStride < target.colStride ? 1 : 2 ); \
		if (mindim == 1) \
			for (integer icol = 1; icol <= target.ncol; icol ++) \
				for (integer irow = 1; irow <= target.nrow; irow ++) \
					target [irow] [icol] op x [icol]; \
		else \
			for (integer irow = 1; irow <= target.nrow; irow ++) \
				for (integer icol = 1; icol <= target.ncol; icol ++) \
					target [irow] [icol] op x [icol]; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION

struct TypeMATadd_MAT_NUM          { constMATVU const& x; double number; };
inline TypeMATadd_MAT_NUM operator+ (constMATVU const& x, double number) { return { x, number }; }
inline TypeMATadd_MAT_NUM operator+ (double number, constMATVU const& x) { return { x, number }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATadd_MAT_NUM const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.x [irow] [icol] + expr.number; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATadd (constMATVU const& x, double number) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= x  +  number;
	return result;
}

struct TypeMATsubtract_MAT_NUM          { constMATVU const& x; double number; };
inline TypeMATsubtract_MAT_NUM operator- (constMATVU const& x, double number) { return { x, number }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATsubtract_MAT_NUM const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.x [irow] [icol] - expr.number; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATsubtract (constMATVU const& x, double number) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= x  -  number;
	return result;
}

struct TypeMATsubtract_NUM_MAT          { double number; constMATVU const& x; };
inline TypeMATsubtract_NUM_MAT operator- (double number, constMATVU const& x) { return { number, x }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATsubtract_NUM_MAT const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.number - expr.x [irow] [icol]; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATsubtract (double number, constMATVU const& x) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= number  -  x;
	return result;
}

struct TypeMATmultiply_MAT_NUM          { constMATVU const& x; double number; };
inline TypeMATmultiply_MAT_NUM operator* (constMATVU const& x, double number) { return { x, number }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATmultiply_MAT_NUM const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.x [irow] [icol] * expr.number; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATmultiply (constMATVU const& x, double number) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= x  *  number;
	return result;
}

struct TypeMATadd_MAT_MAT          { constMATVU const& x; constMATVU const& y; };
inline TypeMATadd_MAT_MAT operator+ (constMATVU const& x, constMATVU const& y) { return { x, y }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATadd_MAT_MAT const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		Melder_assert (expr.x.nrow == expr.y.nrow); \
		Melder_assert (expr.x.ncol == expr.y.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.x [irow] [icol] + expr.y [irow] [icol]; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATadd (constMATVU const& x, constMATVU const& y) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= x  +  y;
	return result;
}

struct TypeMATsubtract_MAT_MAT          { constMATVU const& x; constMATVU const& y; };
inline TypeMATsubtract_MAT_MAT operator- (constMATVU const& x, constMATVU const& y) { return { x, y }; }
#define GENERATE_ONE_TENSOR_FUNCTION(operator, op)  \
	inline void operator (MATVU const& target, TypeMATsubtract_MAT_MAT const& expr) noexcept { \
		Melder_assert (expr.x.nrow == target.nrow); \
		Melder_assert (expr.x.ncol == target.ncol); \
		Melder_assert (expr.x.nrow == expr.y.nrow); \
		Melder_assert (expr.x.ncol == expr.y.ncol); \
		for (integer irow = 1; irow <= expr.x.nrow; irow ++) \
			for (integer icol = 1; icol <= expr.x.ncol; icol ++) \
				target [irow] [icol] op expr.x [irow] [icol] - expr.y [irow] [icol]; \
	}
GENERATE_FIVE_TENSOR_FUNCTIONS
#undef GENERATE_ONE_TENSOR_FUNCTION
inline autoMAT newMATsubtract (constMATVU const& x, constMATVU const& y) {
	autoMAT result = newMATraw (x.nrow, x.ncol);
	result.all() <<= x  -  y;
	return result;
}

/*
	Make the average of each column zero.
		a[i][j] -= a[.][j]
*/
extern void MATcentreEachColumn_inplace (const MAT& x) noexcept;

/*
	Make the average of each row zero.
		a[i][j] -= a[i][.]
*/
extern void MATcentreEachRow_inplace (const MAT& x) noexcept;

/*
	Make the average of every column and every row zero.
		a[i][j] += - a[i][.] - a[.][j] + a[.][.]
*/
extern void MATdoubleCentre_inplace (const MAT& x) noexcept;

extern void MATmtm_preallocated (const MAT& target, const constMAT& x) noexcept;
inline autoMAT newMATmtm (const constMAT& x) {
	autoMAT result = newMATraw (x.ncol, x.ncol);
	MATmtm_preallocated (result.get(), x);
	return result;
}

/*
	Precise matrix multiplication, using pairwise summation.
*/
extern void MATVUmul_ (const MATVU& target, const constMATVU& x, const constMATVU& y) noexcept;
inline void MATVUmul  (const MATVU& target, const constMATVU& x, const constMATVU& y) noexcept {
	Melder_assert (target.nrow == x.nrow);
	Melder_assert (target.ncol == y.ncol);
	Melder_assert (x.ncol == y.nrow);
	MATVUmul_ (target, x, y);
}
inline autoMAT newMATmul (const constMATVU& x, const constMATVU& y) {
	autoMAT result = newMATraw (x.nrow, y.ncol);
	MATVUmul (result.all(), x, y);
	return result;
}
/*
	Rough matrix multiplication, using an in-cache inner loop if that is faster.
*/
extern void MATVUmul_fast_ (const MATVU& target, const constMATVU& x, const constMATVU& y) noexcept;
inline void MATVUmul_fast  (const MATVU& target, const constMATVU& x, const constMATVU& y) noexcept {
	Melder_assert (target.nrow == x.nrow);
	Melder_assert (target.ncol == y.ncol);
	Melder_assert (x.ncol == y.nrow);
	MATVUmul_fast_ (target, x, y);
}
inline autoMAT newMATmul_fast (const constMAT& x, const constMAT& y) {
	autoMAT result = newMATraw (x.nrow, y.ncol);
	MATVUmul_fast (result.all(), x, y);
	return result;
}

void MATouter_preallocated (const MAT& target, const constVEC& x, const constVEC& y);
extern autoMAT newMATouter (const constVEC& x, const constVEC& y);

extern autoMAT newMATpeaks (const constVEC& x, bool includeEdges, int interpolate, bool sortByHeight);

inline autoMAT newMATrandomGauss (integer nrow, integer ncol, double mu, double sigma) {
	autoMAT result = newMATraw (nrow, ncol);
	for (integer irow = 1; irow <= nrow; irow ++)
		for (integer icol = 1; icol <= ncol; icol ++)
			result [irow] [icol] = NUMrandomGauss (mu, sigma);
	return result;
}

inline void MATsin_inplace (const MAT& x) noexcept {
	VECsin_inplace (asvector (x));
}

inline void MATsubtractReversed_inplace (const MAT& x, double number) noexcept {
	for (integer irow = 1; irow <= x.nrow; irow ++)
		for (integer icol = 1; icol <= x.ncol; icol ++)
			x [irow] [icol] = number - x [irow] [icol];
}
inline void MATsubtractReversed_inplace (const MAT& x, const constMAT& y) noexcept {
	Melder_assert (y.nrow == x.nrow && y.ncol == x.ncol);
	for (integer irow = 1; irow <= x.nrow; irow ++)
		for (integer icol = 1; icol <= x.ncol; icol ++)
			x [irow] [icol] = y [irow] [icol] - x [irow] [icol];
}

inline void MATtranspose_inplace_mustBeSquare (const MAT& x) noexcept {
	Melder_assert (x.nrow == x.ncol);
	integer n = x.nrow;
	for (integer i = 1; i < n; i ++)
		for (integer j = i + 1; j <= n; j ++)
			std::swap (x [i] [j], x [j] [i]);
}
inline void MATtranspose_preallocated (const MAT& target, const constMAT& x) noexcept {
	Melder_assert (x.nrow == target.ncol && x.ncol == target.nrow);
	for (integer irow = 1; irow <= target.nrow; irow ++)
		for (integer icol = 1; icol <= target.ncol; icol ++)
			target [irow] [icol] = x [icol] [irow];
}
inline autoMAT newMATtranspose (const constMAT& x) {
	autoMAT result = newMATraw (x.ncol, x.nrow);
	MATtranspose_preallocated (result.get(), x);
	return result;
}

/* End of file MAT.h */

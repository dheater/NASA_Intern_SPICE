/*
 * @(#)matrix.h    generated by: makeheader    Fri Apr 25 00:51:35 1997
 *
 *		built from:	copyright.h
 *				../../src/include/mathwork.hh
 *				../../src/include/mxassert.h
 *				../../src/matrix/array.c
 *				../../src/matrix/array2.c
 *				../../src/matrix/ieee_wrap.c
 *				../../src/matrix/mxapiv4.c
 *				../../src/matrix/mxdbg.h
 */

#ifndef matrix_h
#define matrix_h


/* $Revision: 1.2 $ */
/*
 * Copyright (c) 1984-1997 by The MathWorks, Inc.
 * All Rights Reserved.
 */


#ifdef __cplusplus
    extern "C" {
#endif


#if !defined(__cplusplus) || defined(NO_BUILT_IN_SUPPORT_FOR_BOOL)

  typedef int bool;
  #if !defined(false)
    #define false (0)
  #endif
  #if !defined(true)
    #define true (1)
  #endif

#endif /* !defined(__cplusplus) || defined(NO_BUILT_IN_SUPPORT_FOR_BOOL) */


typedef struct mxArray_tag mxArray;


/* $Revision: 1.4 $ */

/*
mxAssert(int expression, char *error_message)
---------------------------------------------

  Similar to ANSI C's assert() macro, the mxAssert macro checks the
  value of an assertion, continuing execution only if the assertion
  holds.  If 'expression' evaluates to be true, then the mxAssert does
  nothing.  If, however, 'expression' is false, then mxAssert prints an
  error message to the MATLAB Command Window, consisting of the failed
  assertion's expression, the file name and line number where the failed
  assertion occurred, and the string 'error_message'.  'error_message'
  allows the user to specify a more understandable description of why
  the assertion failed.  (Use an empty string if no extra description
  should follow the failed assertion message.)  After a failed
  assertion, control returns to the MATLAB command line. 

  mxAssertS, (the S for Simple), takes the same inputs as mxAssert.  It 
  does not print the text of the failed assertion, only the file and 
  line where the assertion failed, and the explanatory error_message.

  Note that script MEX will turn off these assertions when building
  optimized MEX-functions, so they should be used for debugging 
  purposes only.
*/

#ifdef MATLAB_MEX_FILE
#  ifndef NDEBUG
     extern void mexPrintAssertion(
                                   const char *test, 
                                   const char *fname, 
                                   int linenum, 
                                   const char *message);
#    define mxAssert(test, message) ( (test) ? (void) 0 : mexPrintAssertion(": " #test ",", __FILE__, __LINE__, message))
#    define mxAssertS(test, message) ( (test) ? (void) 0 : mexPrintAssertion("", __FILE__, __LINE__, message))
#  else
#    define mxAssert(test, message) ((void) 0)
#    define mxAssertS(test, message) ((void) 0)
#  endif
#else
#  include <assert.h>
#  define mxAssert(test, message) assert(test)
#  define mxAssertS(test, message) assert(test)
#endif




#include "tmwtypes.h"
#define mxMAXNAM  32	/* maximum name length */

#ifdef V4_COMPAT
typedef double Real;    /* mimic MATLAB 4's matrix.h */
#endif

typedef uint16_T mxChar;

typedef enum {
	mxCELL_CLASS = 1,
	mxSTRUCT_CLASS,
	mxOBJECT_CLASS,
	mxCHAR_CLASS,
	mxSPARSE_CLASS,
	mxDOUBLE_CLASS,
	mxSINGLE_CLASS,
	mxINT8_CLASS,
	mxUINT8_CLASS,
	mxINT16_CLASS,
	mxUINT16_CLASS,
	mxINT32_CLASS,
	mxUINT32_CLASS,
	mxINT64_CLASS,		/* place holder - future enhancements */
	mxUINT64_CLASS,		/* place holder - future enhancements */
	mxUNKNOWN_CLASS = -1
} mxClassID;

typedef enum {
    mxREAL,
    mxCOMPLEX
} mxComplexity;


#if defined(ARRAY_ACCESS_INLINING)
/*
 * This modified version of the mxArray structure is needed to support
 * the ARRAY_ACCESS_INLINING macros.  NOTE: The elements in this structure
 * should not be accessed directly.
 */

struct mxArray_tag {
    char     name[mxMAXNAM];
    int      reserved1[2];
    void    *reserved2;
    int      number_of_dims;
    int      nelements_allocated;
    int      reserved3[3];
    union {
        struct {
            void  *pdata;
            void  *pimag_data;
            void  *reserved4;
            int    reserved5[3];
        }   number_array;
    }   data;
};
#endif /* ARRAY_ACCESS_INLINING */

 
/*
 * Return the class (catergory) of data that the array holds.
 */
extern mxClassID mxGetClassID(const mxArray *pa);


/* 
 * Get pointer to array name.
 */
extern const char *mxGetName(
    const mxArray *pa		/* pointer to array */
    );

 
/* 
 * Set array name.  This routine copies the string pointed to by s
 * into the mxMAXNAM length character name field.
 */
extern void mxSetName(
    mxArray    *pa,		/* pointer to array */
    const char *s		/* string to copy into name */
    );


/*
 * Get pointer to data
 */
extern void *mxGetData(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set pointer to data
 */
extern void mxSetData(
    mxArray *pa,		/* pointer to array */
    void  *pd			/* pointer to data */
    );


/*
 * Get real data pointer for numeric array
 */
extern double *mxGetPr(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set real data pointer for numeric array
 */
extern void mxSetPr(
    mxArray *pa,		/* pointer to array */
    double  *pr			/* real data array pointer */
    );


/*
 * Get imaginary data pointer for numeric array
 */
extern void *mxGetImagData(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set imaginary data pointer for numeric array
 */
extern void mxSetImagData(
    mxArray *pa,		/* pointer to array */
    void    *pi			/* imaginary data array pointer */
    );


/*
 * Get imaginary data pointer for numeric array
 */
extern double *mxGetPi(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set imaginary data pointer for numeric array
 */
extern void mxSetPi(
    mxArray *pa,		/* pointer to array */
    double  *pi			/* imaginary data array pointer */
    );


/* 
 * Determine whether the specified array contains numeric (as opposed 
 * to cell or struct) data.
 */
extern bool mxIsNumeric(const mxArray *pa);


/* 
 * Determine whether the given array is a cell array.
 */
extern bool mxIsCell(const mxArray *pa);


/*  
 * Determine whether the given array contains character data. 
 */
extern bool mxIsChar(const mxArray *pa);


/*
 * Determine whether the given array is a sparse (as opposed to full). 
 */
extern bool mxIsSparse(const mxArray *pa);


/*
 * Determine whether the given array is a structure array.
 */
extern bool mxIsStruct(const mxArray *pa);


/*
 * Determine whether the given array contains complex data.
 */
extern bool mxIsComplex(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * double-precision floating-point numbers.
 */
extern bool mxIsDouble(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * single-precision floating-point numbers.
 */
extern bool mxIsSingle(const mxArray *pa);


/*
 * Determine whether the given array's logical flag is on.
 */ 
extern bool mxIsLogical(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * signed 8-bit integers.
 */
extern bool mxIsInt8(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * unsigned 8-bit integers.
 */
extern bool mxIsUint8(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * signed 16-bit integers.
 */
extern bool mxIsInt16(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * unsigned 16-bit integers.
 */
extern bool mxIsUint16(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * signed 32-bit integers.
 */
extern bool mxIsInt32(const mxArray *pa);


/*
 * Determine whether the specified array represents its data as 
 * unsigned 32-bit integers.
 */
extern bool mxIsUint32(const mxArray *pa);


#ifdef __WATCOMC__
#ifndef __cplusplus
#pragma aux mxGetScalar value [8087];
#endif
#endif


/*
 * Get the real component of the specified array's first data element.
 */
extern double mxGetScalar(const mxArray *pa);


/*
 * Specify that the data in an array is to be treated as Boolean data.
 */
extern void mxSetLogical(mxArray *pa);


/*
 * Specify that the data in an array is to be treated as numerical
 * (as opposed to Boolean) data. 
 */
extern void mxClearLogical(mxArray *pa);


/*
 * Is the isFromGlobalWorkspace bit set?
 */
extern bool mxIsFromGlobalWS(const mxArray *pa);


/*
 * Set the isFromGlobalWorkspace bit.
 */
extern void mxSetFromGlobalWS(mxArray *pa, bool global);


/*
 * Get number of dimensions in array
 */
extern int mxGetNumberOfDimensions(
    const mxArray *pa		/* pointer to array */
    );


/* 
 * Get row dimension
 */
extern int mxGetM(
    const mxArray *pa		/* pointer to array */
    );	


/* 
 * Set row dimension
 */
extern void mxSetM(
    mxArray *pa,		/* pointer to array */
    int     m			/* row dimension */
    );


/* 
 * Get column dimension
 */
extern int mxGetN(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Get pointer to dimension array
 */
extern const int *mxGetDimensions(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Is array empty
 */
extern bool mxIsEmpty(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Get row data pointer for sparse numeric array
 */
extern int *mxGetIr(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set row data pointer for numeric array
 */
extern void mxSetIr(
    mxArray *pa,		/* pointer to array */
    int     *ir			/* row data array pointer */
    );


/*
 * Get column data pointer for sparse numeric array
 */
extern int *mxGetJc(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set column data pointer for numeric array
 */
extern void mxSetJc(
    mxArray *pa,		/* pointer to array */
    int     *jc			/* column data array pointer */
    );


/*
 * Get maximum nonzero elements for sparse numeric array
 */
extern int mxGetNzmax(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Set maximum nonzero elements for numeric array
 */
extern void mxSetNzmax(
    mxArray *pa,		/* pointer to array */
    int     nzmax		/* maximum nonzero elements */
    );


/* 
 * Get number of elements in array
 */
extern int mxGetNumberOfElements(	
    const mxArray *pa		/* pointer to array */
    );


/*
 * Get array data element size
 */
extern int mxGetElementSize(const mxArray *pa);


/* 
 * Return the offset (in number of elements) from the beginning of 
 * the array to a given subscript.
 */
extern int mxCalcSingleSubscript(const mxArray *pa, int nsubs, const int *subs);


/*
 * Get a pointer to the specified cell element. 
 */ 
extern mxArray *mxGetCell(const mxArray *pa, int i);


/*
 * Set an element in a cell array to the specified value.
 */
extern void mxSetCell(mxArray *pa, int i, mxArray *value);


/*
 * Get number of structure fields in array
 */
extern int mxGetNumberOfFields(
    const mxArray *pa		/* pointer to array */
    );


/*
 * Return a pointer to the contents of the named field for the ith 
 * element (zero based).
 */
extern mxArray *mxGetField(const mxArray *pa, int i, const char *fieldname);


/*
 * Set pa[i]->fieldname = value  
 */
extern void mxSetField(mxArray *pa, int i, const char *fieldname, mxArray *value);


/*
 * Get the index to the named field.
 */ 
extern int mxGetFieldNumber(const mxArray *pa, const char *name);


/*
 * Return a pointer to the contents of the named field for 
 * the ith element (zero based).
 */ 
extern mxArray *mxGetFieldByNumber(const mxArray *pa, int i, int fieldnum);


/*
 * Set pa[i][fieldnum] = value 
 */
extern void mxSetFieldByNumber(mxArray *pa, int i, int fieldnum, mxArray *value);


/*
 * Return pointer to the nth field name
 */
extern const char *mxGetFieldNameByNumber(const mxArray *pa, int n);

 
/* 
 * Return the name of an array's class.  
 */
extern const char *mxGetClassName(const mxArray *pa);


/*
 * Determine whether an array is a member of the specified class. 
 */
extern bool mxIsClass(const mxArray *pa, const char *name);


#if defined(ARRAY_ACCESS_INLINING)
/*
 * Macros for API functions.
 * NOTE: Inlined mex files are not NOT guaranteed to be portable from one
 * release of MATLAB to another.
 */
#define mxGetName(pa)           ((const char *)((pa)->name))
#define mxGetPr(pa)             ((double *)((pa)->data.number_array.pdata))
#define mxSetPr(pa,pv)          ((pa)->data.number_array.pdata = (pv))
#define mxGetPi(pa)             ((double *)((pa)->data.number_array.pimag_data))
#define mxSetPi(pa,pv)          ((pa)->data.number_array.pimag_data = (pv))
#define mxGetData(pa)           ((pa)->data.number_array.pdata)
#define mxSetData(pa,pv)        ((pa)->data.number_array.pdata = (pv))
#define mxGetImagData(pa)       ((pa)->data.number_array.pimag_data)
#define mxSetImagData(pa,pv)    ((pa)->data.number_array.pimag_data = (pv))
#define mxGetNumberOfDimensions(pa)          ((pa)->number_of_dims)
#define mxGetNzmax(pa)          ((pa)->nelements_allocated)
#define mxSetNzmax(pa, nzmax)   ((pa)->nelements_allocated = (nzmax))
#endif  /* defined(ARRAY_ACCESS_INLINING) */


#include <stdlib.h>


typedef void * (*calloc_proc)(size_t nmemb, size_t size);


typedef void (*free_proc)(void *ptr);


typedef void * (*malloc_proc)(size_t size);


typedef void * (*realloc_proc)(void *ptr, size_t size);


typedef int (*multibyte_charset_proc)(void);


typedef int (*multibyte_lead_proc)(unsigned char);


/*
 * Set the multi-byte functions used by the matrix library for handling
 * international strings. You must supply multibyte_charsetfcn and
 * multibyte_leadfcn functions when using mxSetMultiByteFcns.
 * The multibyte_charsetfcn takes no arguments and returns true if the
 * current character set is a multi-byte character set and false if it is not.
 * The multibyte_leadfcn takes an unsigned char and returns true if the
 * character is the lead byte of a multi-byte character and false if it is not.
 */
extern void mxSetMultiByteFcns(
	multibyte_charset_proc	multibyte_charsetfcn,
	multibyte_lead_proc		multibyte_leadfcn
	);


#if !defined(MATLAB_MEX_FILE)


/*
 * Set the memory allocation functions used by the matrix library. You must
 * supply calloc, realloc, free and malloc functions when using mxSetAllocFcns.
 * NOTE: the free function MUST handle the case when the pointer to be freed is NULL.
 * The default AllocFcns for the matrix library are based on the standard C
 * library functions calloc, realloc, free and malloc.
 */
extern void mxSetAllocFcns(
	calloc_proc		callocfcn,
	free_proc		freefcn,
	realloc_proc	reallocfcn,
	malloc_proc		mallocfcn
	);


#endif /* !defined(MATLAB_MEX_FILE) */


/*
 * allocate managed memory
 */
extern void *mxMalloc(
    size_t	n		/* number of bytes */
    );


/*
 * allocate managed memory
 */
extern void *mxCalloc(
    size_t	n,		/* number of objects */
    size_t	size	/* size of objects */
    );


/*
 * free memory routine for managed list
 */
extern void mxFree(void *ptr);	/* pointer to memory to be freed */


/*
 * reallocate the memory routine for the managed list
 *
 * Note: If realloc() fails to allocate a block, it is the end-user's responsibility
 * to free the block because the ANSI definition of realloc states that the block
 * will remain allocated.  realloc() returns a NULL in this case.  This means that
 * calls to realloc of the form:
 *
 * x = mxRealloc(x, size)
 *
 * will cause memory leaks if realloc fails (and returns a NULL).
 *
 */
extern void *mxRealloc(void *ptr, size_t size);

 
/* 
 * Set column dimension
 */
extern void mxSetN(mxArray *pa, int n);


/*
 * Set dimension array and number of dimensions.  Returns 0 on success and 1
 * if there was not enough memory available to reallocate the dimensions array.
 */
extern int mxSetDimensions(mxArray *pa, const int *size, int ndims);


/*
 * Deallocate (free) the heap memory held by the specified array.
 */
extern void mxDestroyArray(mxArray *pa);


/*
 * Initialize a static empty array
 */
extern void mxInitStaticArray(mxArray *pa, mxClassID classid);


/*
 * Create a numeric array and initialize all its data elements to 0.
 */ 
extern mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID classid, mxComplexity flag);


/*
 * Create a two-dimensional array to hold double-precision 
 * floating-point data; initialize each data element to 0.
 */
extern mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity flag);


/*
 * Create a 2-Dimensional sparse array.
 */
extern mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity flag);


/* 
 * Converts a string array to a C-style string.
 */
extern int mxGetString(const mxArray *pa, char *buf, int buflen);


/*
 * Create a 1-by-n string array initialized to null terminated string
 * where n is the length of the string.
 */
extern mxArray *mxCreateString(const char *str);


/*
 * Create an N-Dimensional array to hold string data;
 * initialize all elements to 0.
 */
extern mxArray *mxCreateCharArray(int ndim, const int *dims);


/*
 * Create a string array initialized to the strings in str. 
 */
extern mxArray *mxCreateCharMatrixFromStrings(int m, const char **str);


/*
 * Create a 2-Dimensional cell array, with each cell initialized
 * to NULL.
 */
extern mxArray *mxCreateCellMatrix(int m, int n);


/*
 * Create an N-Dimensional cell array, with each cell initialized
 * to NULL. 
 */
extern mxArray *mxCreateCellArray(int ndim, const int *dims);


/*
 * Create a 2-Dimensional structure array having the specified fields;
 * initialize all values to NULL.
 */ 
extern mxArray *mxCreateStructMatrix(int m, int n, int nfields, const char **fieldnames);


/*
 * Create an N-Dimensional structure array having the specified fields;
 * initialize all values to NULL.
 */ 
extern mxArray *mxCreateStructArray(int ndim, const int *dims, int nfields,
								const char **fieldnames);


/*
 * Make a deep copy of an array, return a pointer to the copy. 
 */
extern mxArray *mxDuplicateArray(const mxArray *in);


/*
 * Set classname of an unvalidated object array.  It is illegal to 
 * call this function on a previously validated object array. 
 * Return 0 for success, 1 for failure.
 */
extern int mxSetClassName(mxArray *pa, const char *classname);


#ifdef __WATCOMC__
#ifndef __cplusplus
#pragma aux mxGetEps value [8087];
#pragma aux mxGetInf value [8087];
#pragma aux mxGetNaN value [8087];
#endif
#endif


/*
 * Function for obtaining MATLAB's concept of EPS
 */
extern double mxGetEps(void);


/*
 * Function for obtaining MATLAB's concept of INF (Used in MEX-File callback).
 */
extern double mxGetInf(void);


/*
 * Function for obtaining MATLAB's concept of NaN (Used in MEX-File callback).
 */
extern double mxGetNaN(void);


/*
 * test for finiteness in a machine-independent manner
 */
extern bool mxIsFinite(
    double x                  /* value to test */
    );


/*
 * test for infinity in a machine-independent manner
 */
extern bool mxIsInf(
    double x                  /* value to test */
    );


/*
 * test for NaN in a machine-independent manner
 */
extern bool mxIsNaN(
    double x                  /* value to test */
    );


#if defined(V4_COMPAT)
#define Matrix  mxArray
#define COMPLEX mxCOMPLEX
#define REAL    mxREAL
#endif /* V4_COMPAT */


#if defined(V4_COMPAT)


/*
 * Is matrix struc set to FULL
 * Obsolete: use !mxIsSparse() instead
 */
extern int mxIsFull(
    const Matrix *pm		/* pointer to matrix */
    );


#define mxCreateFull mxCreateDoubleMatrix
#define mxIsString   mxIsChar
#define mxFreeMatrix mxDestroyArray
#else
#define mxIsFull()     mxIsFull_is_obsolete
#define mxCreateFull() mxCreateFull_is_obsolete
#define mxIsString()   mxIsString_is_obsolete
#define mxFreeMatrix() mxFreeMatrix_is_obsolete
#endif /* V4_COMPAT */


/* $Revision: 1.10 $ */
#ifdef ARGCHECK

#include "mwdebug.h" /* Prototype _d versions of API functions */

#define mxCalcSingleSubscript(pa, nsubs, subs) mxCalcSingleSubscript_d(pa, nsubs, subs, __FILE__, __LINE__) 
#define mxCalloc(nelems, size) mxCalloc_d(nelems, size, __FILE__, __LINE__) 
#define mxClearLogical(pa)				mxClearLogical_d(pa, __FILE__, __LINE__)
#define mxCreateCellArray(ndim, dims)	mxCreateCellArray_d(ndim, dims, __FILE__, __LINE__)
#define mxCreateCellMatrix(m, n)		mxCreateCellMatrix_d(m, n, __FILE__, __LINE__)
#define mxCreateCharArray(ndim, dims) mxCreateCharArray_d(ndim, dims, __FILE__, __LINE__)
#define mxCreateCharMatrixFromStrings(m, strings) mxCreateCharMatrixFromStrings_d(m, strings, __FILE__, __LINE__)
#define mxCreateDoubleMatrix(m, n, cplxflag)				mxCreateDoubleMatrix_d(m, n, cplxflag, __FILE__, __LINE__)
#define mxCreateNumericArray(ndim, dims, classname, cplxflag) mxCreateNumericArray_d(ndim, dims, classname, cplxflag, __FILE__, __LINE__) 
#define mxCreateSparse(m, n, nzmax, cplxflag) mxCreateSparse_d(m, n, nzmax, cplxflag, __FILE__, __LINE__) 
#define mxCreateString(string) mxCreateString_d(string, __FILE__, __LINE__)
#define mxCreateStructArray(ndim, dims, nfields, fieldnames) mxCreateStructArray_d(ndim, dims, nfields, fieldnames, __FILE__, __LINE__)
#define mxCreateStructMatrix(m, n, nfields, fieldnames) mxCreateStructMatrix_d(m, n, nfields, fieldnames, __FILE__, __LINE__)
#define mxDestroyArray(pa)                    mxDestroyArray_d(pa, __FILE__, __LINE__)
#define mxDuplicateArray(pa)                    mxDuplicateArray_d(pa, __FILE__, __LINE__)
#define mxFree(pm)			mxFree_d(pm, __FILE__, __LINE__)
#define mxGetCell(pa, index)			mxGetCell_d(pa, index, __FILE__, __LINE__)
#define mxGetClassID(pa) 			mxGetClassID_d(pa, __FILE__, __LINE__)
#define mxGetClassName(pa) 			mxGetClassName_d(pa, __FILE__, __LINE__)
#define mxGetData(pa) mxGetData_d(pa, __FILE__, __LINE__)
#define mxGetDimensions(pa)  				mxGetDimensions_d(pa, __FILE__, __LINE__)
#define mxGetElementSize(pa)			mxGetElementSize_d(pa, __FILE__, __LINE__)
#define mxGetField(pa, index, fieldname) mxGetField_d(pa, index, fieldname, __FILE__, __LINE__)
#define mxGetFieldByNumber(pa, index, fieldnum) mxGetFieldByNumber_d(pa, index, fieldnum, __FILE__, __LINE__)
#define mxGetFieldNameByNumber(pa, fieldnum) mxGetFieldNameByNumber_d(pa, fieldnum, __FILE__, __LINE__)
#define mxGetFieldNumber(pa, fieldname) mxGetFieldNumber_d(pa, fieldname, __FILE__, __LINE__)
#define mxGetImagData(pa) mxGetImagData_d(pa, __FILE__, __LINE__)
#define mxGetIr(pa) mxGetIr_d(pa, __FILE__, __LINE__)
#define mxGetJc(pa) mxGetJc_d(pa, __FILE__, __LINE__)
#define mxGetName(pa)  				mxGetName_d(pa, __FILE__, __LINE__)
#define mxGetNumberOfDimensions(pa)	mxGetNumberOfDimensions_d(pa, __FILE__, __LINE__)
#define mxGetNumberOfElements(pa)	mxGetNumberOfElements_d(pa, __FILE__, __LINE__)
#define mxGetNumberOfFields(pa)			mxGetNumberOfFields_d(pa, __FILE__, __LINE__)
#define mxGetNzmax(pa)					mxGetNzmax_d(pa, __FILE__, __LINE__)
#define mxGetM(pa)					mxGetM_d(pa, __FILE__, __LINE__)
#define mxGetN(pa)					mxGetN_d(pa, __FILE__, __LINE__)
#define mxGetPi(pa) mxGetPi_d(pa, __FILE__, __LINE__)
#define mxGetPr(pa) mxGetPr_d(pa, __FILE__, __LINE__)
#define mxGetScalar(pa)					mxGetScalar_d(pa, __FILE__, __LINE__)
#define mxGetString(pa, buffer, buflen) mxGetString_d(pa, buffer, buflen, __FILE__, __LINE__)
#define mxIsCell(pa)					mxIsCell_d(pa, __FILE__, __LINE__)
#define mxIsChar(pa)					mxIsChar_d(pa, __FILE__, __LINE__)
#define mxIsClass(pa, classname) mxIsClass_d(pa, classname, __FILE__, __LINE__)
#define mxIsComplex(pa)					mxIsComplex_d(pa, __FILE__, __LINE__)
#define mxIsDouble(pa)					mxIsDouble_d(pa, __FILE__, __LINE__)
#define mxIsEmpty(pa)					mxIsEmpty_d(pa, __FILE__, __LINE__)
#define mxIsFromGlobalWS(pa)					mxIsFromGlobalWS_d(pa, __FILE__, __LINE__)
#define mxIsInt8(pa)					mxIsInt8_d(pa, __FILE__, __LINE__)
#define mxIsInt16(pa)					mxIsInt16_d(pa, __FILE__, __LINE__)
#define mxIsInt32(pa)					mxIsInt32_d(pa, __FILE__, __LINE__)
#define mxIsLogical(pa)					mxIsLogical_d(pa, __FILE__, __LINE__)
#define mxIsNumeric(pa)					mxIsNumeric_d(pa, __FILE__, __LINE__)
#define mxIsSingle(pa)					mxIsSingle_d(pa, __FILE__, __LINE__)
#define mxIsSparse(pa)					mxIsSparse_d(pa, __FILE__, __LINE__)
#define mxIsStruct(pa)					mxIsStruct_d(pa, __FILE__, __LINE__)
#define mxIsUint8(pa)					mxIsUint8_d(pa, __FILE__, __LINE__)
#define mxIsUint16(pa)					mxIsUint16_d(pa, __FILE__, __LINE__)
#define mxIsUint32(pa)					mxIsUint32_d(pa, __FILE__, __LINE__)
#define mxRealloc(pm, nelems)				mxRealloc_d(pm, nelems, __FILE__, __LINE__)
#if !defined(MATLAB_MEX_FILE)
#define mxSetAllocFcns(callocptr, freeptr, reallocptr, mallocptr) mxSetAllocFcns_d(callocptr, freeptr, reallocptr, freeptr, __FILE__, __LINE__)
#endif /* MATLAB_MEX_FILE */
#define mxSetCell(pa, index, value)		mxSetCell_d(pa, index, value, __FILE__, __LINE__)
#define mxSetClassName(pa, name)		mxSetClassName_d(pa, name, __FILE__, __LINE__)
#define mxSetData(pa, pd)				mxSetData_d(pa, pd, __FILE__, __LINE__)
#define mxSetDimensions(pa, size, ndims) mxSetDimensions_d(pa, size, ndims, __FILE__, __LINE__)
#define mxSetField(pa, index, fieldname, value) mxSetField_d(pa, index, fieldname, value, __FILE__, __LINE__)
#define mxSetFieldByNumber(pa, index, fieldnum, value) mxSetFieldByNumber_d(pa, index, fieldnum, value, __FILE__, __LINE__)
#define mxSetImagData(pa, pid)			   mxSetImagData_d(pa, pid, __FILE__, __LINE__)
#define mxSetIr(pa, ir)					mxSetIr_d(pa, ir, __FILE__, __LINE__)
#define mxSetJc(pa, jc)					mxSetJc_d(pa, jc, __FILE__, __LINE__)
#define mxSetLogical(pa)				mxSetLogical_d(pa, __FILE__, __LINE__)
#define mxSetM(pa, m)				mxSetM_d(pa, m, __FILE__, __LINE__)
#define mxSetN(pa, n)				mxSetN_d(pa, n, __FILE__, __LINE__)
#define mxSetName(pa, name) 		mxSetName_d(pa, name, __FILE__, __LINE__)
#define mxSetNzmax(pa, nzmax)			mxSetNzmax_d(pa, nzmax, __FILE__, __LINE__)
#define mxSetPi(pa, pi)					mxSetPi_d(pa, pi, __FILE__, __LINE__)
#define mxSetPr(pa, pr)					mxSetPr_d(pa, pr, __FILE__, __LINE__)
#endif

#ifdef __cplusplus
    }	/* extern "C" */
#endif

#endif /* matrix_h */
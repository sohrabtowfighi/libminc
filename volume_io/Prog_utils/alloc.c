/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#include  <internal_volume_io.h>

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/minc/volume_io/Prog_utils/alloc.c,v 1.15 1995-08-02 19:21:50 david Exp $";
#endif

private    void       record_alloc( size_t );
private    void       record_realloc( size_t );
private    void       record_free( void );

/* ----------------------------- MNI Header -----------------------------------
@NAME       : alloc_memory
@INPUT      : n_bytes
@OUTPUT     : ptr
@RETURNS    : 
@DESCRIPTION: Allocates the specified number of bytes.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    :                      David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  alloc_memory(
    void         **ptr,
    size_t       n_bytes )
{
    if( n_bytes != 0 )
    {
        *ptr = (void *) malloc( n_bytes );

        if( *ptr == (void *) 0 )
        {
            print_error( "Error alloc_memory: out of memory, %d bytes.\n",
                         n_bytes );
            abort();
        }
    }
    else
        *ptr = NULL;

    record_alloc( n_bytes );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : realloc_memory
@INPUT      : ptr
            : n_bytes
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Reallocates the ptr.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    :                      David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  realloc_memory(
    void      **ptr,
    size_t    n_bytes )
{
    if( n_bytes != 0 )
    {
        *ptr = (void *) realloc( *ptr, n_bytes );

        if( *ptr == (void *) 0 )
        {
            print_error( "Error realloc_memory: out of memory, %d bytes.\n",
                         n_bytes );
            abort_if_allowed();
        }
    }

    record_realloc( n_bytes );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : free_memory
@INPUT      : ptr
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Frees the pointer, and sets it to NIL.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    :                      David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  free_memory( void   **ptr )
{
    record_free();

    if( *ptr != (void *) 0 )
    {
        free( *ptr );

        *ptr = (void *) 0;
    }
}

#include  <stdio.h>

private  FILE  *file;

/* ----------------------------- MNI Header -----------------------------------
@NAME       : writing_alloc_debug
@INPUT      : 
@OUTPUT     : 
@RETURNS    : TRUE if saving debug info
@DESCRIPTION: 
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  BOOLEAN  writing_alloc_debug( void )
{
    static   BOOLEAN   first = TRUE;
    static   BOOLEAN   writing = FALSE;
    char               *filename;

    if( first )
    {
        first = FALSE;
        filename = getenv( "ALLOC_OUTPUT_FILE" );

        if( filename != (char *) 0 )
        {
            file = fopen( filename, "w" );

            if( file != (FILE *) 0 )
                writing = TRUE;
        }
    }

    return( writing );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : record_alloc
@INPUT      : n_bytes
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: If enabled, prints out the amount of memory allocated.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  record_alloc(
    size_t  n_bytes )
{
    if( writing_alloc_debug() )
    {
        (void) fprintf( file, "ALLOC   %20d\n", n_bytes );
        (void) fflush( file );
    }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : record_realloc
@INPUT      : n_bytes
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: If enabled, prints out the amount of memory reallocated.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  record_realloc(
    size_t   n_bytes )
{
    if( writing_alloc_debug() )
    {
        (void) fprintf( file, "REALLOC %20d\n", n_bytes );
        (void) fflush( file );
    }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : record_free
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: If enabled, prints out that memory was deallocated.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  record_free( void )
{
    if( writing_alloc_debug() )
    {
        (void) fprintf( file, "FREE\n" );
        (void) fflush( file );
    }
}

public  void  set_up_array_pointers_2D(
    void      **ptr,
    long      n1,
    long      n2,
    size_t    type_size )
{
    int   i;

    for_less( i, 1, n1 )
        ptr[i] = (void *) ((long) ptr[i-1] + n2 * type_size);
}

#ifdef DELETE_THIS
public  void  set_up_array_pointers_3D(
    void      ***ptr,
    long      n1,
    long      n2,
    long      n3,
    size_t    type_size )
{
    int   i, j;

    for_less( i, 0, n1 )
    {
        if( i > 0 )
            ptr[i][0] = (void *) ((long) (ptr[i-1][0]) + n2 * n3 * type_size);

        for_less( j, 1, n2 )
            ptr[i][j] = (void *) ((long) (ptr[i][j-1]) + n3 * type_size);
    }
}

public  void  set_up_array_pointers_4D(
    void      ****ptr,
    long      n1,
    long      n2,
    long      n3,
    long      n4,
    size_t    type_size )
{
    int   i, j, k;

    for_less( i, 0, n1 )
    {
        if( i > 0 )
            ptr[i][0][0] = (void *) ((long) (ptr[i-1][0][0]) +
                                     n2 * n3 * n4 *type_size);
        for_less( j, 0, n2 )
        {
            if( j > 0 )
                ptr[i][j][0] = (void *) ((long) (ptr[i][j-1][0]) +
                                         n3 * n4 * type_size);
            for_less( k, 1, n3 )
            {
                ptr[i][j][k] = (void *) ((long) (ptr[i][j][k-1]) +
                                         n4 * type_size);
            }
        }
    }                                  
}

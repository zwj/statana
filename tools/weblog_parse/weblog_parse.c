/* weblog_parse - extract specified fields from a web log file
**
** Copyright © 1998 by Jef Poskanzer <jef@mail.acme.com>.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#ifndef REG_BASIC
#define REG_BASIC 0
#endif


/* Defines. */

/* The item names. */
#define I_HOST 0
#define I_RFC931USER 1
#define I_AUTHUSER 2
#define I_DATE 3
#define I_METHOD 4
#define I_FILE 5
#define I_PROTOCOL 6
#define I_STATUS 7
#define I_BYTES 8
#define I_REFERER 9
#define I_USERAGENT 10
#define N_ITEM_NAMES 11

/* Actions. */
#define A_ALWAYS 0
#define A_EQUALS 1
#define A_NOTEQUALS 2
#define A_WILDCARD 3
#define A_NOTWILDCARD 4


/* Globals. */

static char* argv0;
static int quiet;


/* Forwards. */
static void usage( void );
static void bogus( char* where, char* rest );


int
main( int argc, char** argv )
    {
    int argn;
    char line[10000];
    char* cp;
    char* item_name[N_ITEM_NAMES] = {
      "host", "rfc931user", "authuser", "date", "method", "file", "protocol",
      "status", "bytes", "referer", "useragent",
      };
    int n_items, name_len;
    int item[100];
    int action[100];
    char* match_str[100];
    regex_t match_re[100];
    char* item_str[N_ITEM_NAMES];
    int i, r;
    char* wildcards = ".*[";	/* only a few of the valid r.e. wildcards */

    argv0 = argv[0];

    /* Parse args. */
    argn = 1;
    quiet = 0;
    while ( argn < argc && argv[argn][0] == '-' && argv[argn][1] != '\0' )
	{
	if ( strcmp( argv[argn], "-quiet" ) == 0 )
	    quiet = 1;
	else
	    usage();
	++argn;
	}
    if ( argn >= argc )
	usage();
    n_items = 0;
    for ( ; argn < argc; ++argn )
	{
	for ( i = 0; i < N_ITEM_NAMES; ++i )
	    {
	    name_len = strlen( item_name[i] );
	    if ( strncasecmp( argv[argn], item_name[i], name_len ) == 0 )
		{
		item[n_items] = i;
		if ( argv[argn][name_len] == '\0' )
		    action[n_items] = A_ALWAYS;
		else
		    {
		    if ( argv[argn][name_len] == '=' )
			action[n_items] = A_EQUALS;
		    else if ( argv[argn][name_len] == '^' )
			action[n_items] = A_NOTEQUALS;
		    else
			usage();
		    match_str[n_items] = &(argv[argn][name_len+1]);
		    if ( strpbrk( match_str[n_items], wildcards ) != (char*) 0 )
			{
			switch ( action[n_items] )
			    {
			    case A_EQUALS: action[n_items] = A_WILDCARD; break;
			    case A_NOTEQUALS: action[n_items] = A_NOTWILDCARD; break;
			    }
			r = regcomp( &(match_re[n_items]), match_str[n_items], REG_BASIC|REG_NOSUB );
			if ( r != 0 )
			    {
			    char buf[1000];
			    (void) regerror( r, &(match_re[n_items]), buf, sizeof(buf) );
			    (void) fprintf( stderr, "%s: regexp problem - %s\n", argv0, buf );
			    exit( 1 );
			    }
			}
		    }
		n_items++;
		goto next_arg;
		}
	    }
	usage();
	next_arg: ;
	}

    /* Read the input file. */
    while ( fgets( line, sizeof(line), stdin ) != (char*) 0 )
	{
	/* Parse the line. */
	item_str[I_HOST] = line;
	cp = strchr( item_str[I_HOST], ' ' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_HOST], item_str[I_HOST] ); continue; }
	*cp = '\0';
	item_str[I_RFC931USER] = cp + 1;
	cp = strchr( item_str[I_RFC931USER], ' ' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_RFC931USER], item_str[I_RFC931USER] ); continue; }
	*cp = '\0';
	item_str[I_AUTHUSER] = cp + 1;
	cp = strchr( item_str[I_AUTHUSER], ' ' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_AUTHUSER], item_str[I_AUTHUSER] ); continue; }
	*cp = '\0';
	item_str[I_DATE] = cp + 1;
	if ( *item_str[I_DATE] != '[' )
	    { bogus( item_name[I_DATE], item_str[I_DATE] ); continue; }
	++item_str[I_DATE];
	cp = strchr( item_str[I_DATE], ']' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_DATE], item_str[I_DATE] ); continue; }
	*cp = '\0';
	item_str[I_METHOD] = cp + 1;
	if ( *item_str[I_METHOD] != ' ' )
	    { bogus( item_name[I_METHOD], item_str[I_METHOD] ); continue; }
	++item_str[I_METHOD];
	if ( *item_str[I_METHOD] != '"' )
	    { bogus( item_name[I_METHOD], item_str[I_METHOD] ); continue; }
	++item_str[I_METHOD];
	cp = strchr( item_str[I_METHOD], ' ' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_METHOD], item_str[I_METHOD] ); continue; }
	*cp = '\0';
	item_str[I_FILE] = cp + 1;
	i = strcspn( item_str[I_FILE], " \"" );
	if ( item_str[I_FILE][i] != ' ' && item_str[I_FILE][i] != '"' )
	    { bogus( item_name[I_FILE], item_str[I_FILE] ); continue; }
	if ( item_str[I_FILE][i] == ' ' )
	    {
	    item_str[I_FILE][i] = '\0';
	    item_str[I_PROTOCOL] = &(item_str[I_FILE][i + 1]);
	    cp = strchr( item_str[I_PROTOCOL], '"' );
	    if ( cp == (char*) 0 )
		{ bogus( item_name[I_PROTOCOL], item_str[I_PROTOCOL] ); continue; }
	    *cp = '\0';
	    item_str[I_STATUS] = cp + 1;
	    }
	else if ( item_str[I_FILE][i] == '"' )
	    {
	    item_str[I_FILE][i] = '\0';
	    item_str[I_PROTOCOL] = "HTTP/0.9";
	    item_str[I_STATUS] = &(item_str[I_FILE][i + 1]);
	    }
	if ( *item_str[I_STATUS] != ' ' )
	    { bogus( item_name[I_STATUS], item_str[I_STATUS] ); continue; }
	++item_str[I_STATUS];
	cp = strchr( item_str[I_STATUS], ' ' );
	if ( cp == (char*) 0 )
	    { bogus( item_name[I_STATUS], item_str[I_STATUS] ); continue; }
	*cp = '\0';
	item_str[I_BYTES] = cp + 1;
	cp = strchr( item_str[I_BYTES], ' ' );
	if ( cp == (char*) 0 )
	    {
	    cp = strchr( item_str[I_BYTES], '\n' );
	    if ( cp != (char*) 0 )
		*cp = '\0';
	    item_str[I_REFERER] = "";
	    item_str[I_USERAGENT] = "";
	    }
	else
	    {
	    *cp = '\0';
	    item_str[I_REFERER] = cp + 1;
	    if ( *item_str[I_REFERER] != '"' )
		{ bogus( item_name[I_REFERER], item_str[I_REFERER] ); continue; }
	    ++item_str[I_REFERER];
	    cp = strchr( item_str[I_REFERER], '"' );
	    if ( cp == (char*) 0 )
		{ bogus( item_name[I_REFERER], item_str[I_REFERER] ); continue; }
	    *cp = '\0';
	    item_str[I_USERAGENT] = cp + 1;
	    if ( *item_str[I_USERAGENT] != ' ' )
		{ bogus( item_name[I_USERAGENT], item_str[I_USERAGENT] ); continue; }
	    ++item_str[I_USERAGENT];
	    if ( *item_str[I_USERAGENT] != '"' )
		{ bogus( item_name[I_USERAGENT], item_str[I_USERAGENT] ); continue; }
	    ++item_str[I_USERAGENT];
	    cp = strchr( item_str[I_USERAGENT], '"' );
	    if ( cp == (char*) 0 )
		{ bogus( item_name[I_USERAGENT], item_str[I_USERAGENT] ); continue; }
	    *cp = '\0';
	    ++cp;
	    }

	/* Check if we should show this record. */
	for ( i = 0; i < n_items; ++i )
	    {
	    switch ( action[i] )
		{
		case A_ALWAYS:
		break;
		case A_EQUALS:
		if ( strcmp( item_str[item[i]], match_str[i] ) != 0 )
		    goto next_line;
		break;
		case A_NOTEQUALS:
		if ( strcmp( item_str[item[i]], match_str[i] ) == 0 )
		    goto next_line;
		break;
		case A_WILDCARD:
		if ( regexec( &(match_re[i]), item_str[item[i]], 0, (regmatch_t*) 0, 0 ) == REG_NOMATCH )
		    goto next_line;
		break;
		case A_NOTWILDCARD:
		if ( regexec( &(match_re[i]), item_str[item[i]], 0, (regmatch_t*) 0, 0 ) != REG_NOMATCH )
		    goto next_line;
		break;
		}
	    }

	/* Write out the requested items. */
	for ( i = 0; i < n_items; ++i )
	    {
	    if ( i != 0 )
		putchar( '\t' );
	    (void) fputs( item_str[item[i]], stdout );
	    }
	putchar( '\n' );

	next_line: ;
	}

    /* Done. */
    exit( 0 );
    }


static void
usage( void )
    {
    (void) fprintf( stderr, "usage:  %s [-quiet] host|rfc931user|authuser|date|method|file|protocol|status|bytes|referer|useragent ...\n", argv0 );
    exit( 1 );
    }

static void
bogus( char* where, char* rest )
    {
    int len;

    if ( ! quiet )
	{
	len = strlen( rest );
	if ( rest[len - 1] == '\n' )
	    rest[len - 1] = '\0';
	(void) fprintf( stderr, "%s: bogus line at %s - '%s'\n", argv0, where, rest );
	}
    }

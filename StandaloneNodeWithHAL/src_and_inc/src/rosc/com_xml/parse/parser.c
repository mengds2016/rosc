#include <stdio.h>
#include <rosc/com_xml/parse/parser.h>

void xmlrpc_parse_act_init(parse_act_t *pact, xmlrpc_parser_type_t type, void * handler_data_storage)
{
	pact->handler_data_storage=handler_data_storage;
	pact->mode=PARSE_MODE_HEADER;
	pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_BEGIN;
	pact->submode=PARSE_SUBMODE_NONE;


	switch (type) {
		case  XMLRPC_SERVER:
			pact->handler_fkt=&xmlrpc_server_handler;
			break;
		case XMLRPC_CLIENT:
			printf("TODO: IMPLEMENT!");
			break;
		default:
			break;
	}

}

void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{

	bool chunk_processed=false;
	while(!chunk_processed)
	{
		if((pact->submode==PARSE_SUBMODE_NONE) || (pact->submode_state==PARSE_SUBMODE_FINISHED))
		{
			switch(pact->mode)
			{
			case PARSE_MODE_HEADER:
					#ifdef FORCE_INLINE
						#define ENABLE_C
							#include "mode/parse_mode_header.c"
						#undef ENABLE_C
					#else
						parse_mode_header(buf, &len, pact);
					#endif
				break;
			case PARSE_MODE_XML:

				break;
			default:
				//TODO Error
				break;
			}
		}
		else
		{
			switch (pact->submode) {
				case PARSE_SUBMODE_COPY2BUFFER:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/copy2buffer.c"
							#undef ENABLE_C
						#else
							copy2buffer(buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_NUMBERPARSE:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/numberparse.c"
							#undef ENABLE_C
						#else
							numberparse(buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SEEKSTRING:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/seekstring.c"
							#undef ENABLE_C
						#else
							seekstring(buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SKIPWHOLEMESSAGE:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/skipwholemessage.c"
							#undef ENABLE_C
						#else
							skipwholemessage(buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SKIPUNTILCHAR:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/skipuntilchar.c"
							#undef ENABLE_C
						#else
							skipunitlchar(buf, &len, pact);
						#endif
					break;
				default:
					break;
			}
		}
	}
	if((len <= 0)  && pact->submode_state!=PARSE_SUBMODE_FINISHED)
	{
		chunk_processed=true;
	}
}



//	pact->handler_fkt(pact);
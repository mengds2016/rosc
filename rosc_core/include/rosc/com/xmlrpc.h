/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met: 
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer. 
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution. 
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies, 
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  xmlrpc.h created by Christian Holl
 */

#ifndef XMLRPC_H_
#define XMLRPC_H_

#define XMLRPC_MAX_ARRAY_NESTING 3

#define XMLRPC_SECURITY_MAX_MESSAGE_SIZE 1024


#include <rosc/system/eth.h>
#include <rosc/sebs_parse_fw/send_modules/socket_connect.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_http.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_xml.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_parseurl.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>
#include <rosc/sebs_parse_fw/send_modules/msggen.h>



typedef enum
{
	XMLRPC_TYPE_CLIENT,
	XMLRPC_TYPE_SERVER,
}xmlrpc_t;



typedef enum
{
	XMLRPC_STATE_SENDING_REQUEST,
	XMLRPC_STATE_SENDING_RESPONSE,
	XMLRPC_STATE_CLOSE,
	XMLRPC_STATE_CONNECT,
	XMLRPC_STATE_SEND_REQUEST,
	XMLRPC_STATE_HTTP,
	XMLRPC_STATE_XML,
	XMLRPC_STATE_SEND_RESPONSE,
}xmlrpc_state_t;

/**
 * Creating the enum for descriptor usage by hander function
 */
typedef enum
{

	XMLRPC_DESCRIPTOR_UNKNOWN=SEBS_PARSE_SEEKSTRING_NOT_FOUND ,
	XMLRPC_HTTP_DESCRIPTORS(XMLRPC)
}xmlrpc_header_descriptors_t;

/**
 * Creating the enum for action usage by hander function
 */
typedef enum
{
	XMLRPC_ACTION_UNKNOWN=SEBS_PARSE_SEEKSTRING_NOT_FOUND ,
	XMLRPC_HTTP_ACTIONS(XMLRPC)
}xmlrpc_http_actions_t;

/**
 * Creating the enum for method usage by hander function
 */
typedef enum
{
	XMLRPC_METHOD_UNKNOWN=SEBS_PARSE_SEEKSTRING_NOT_FOUND ,
	XMLRPC_HTTP_METHODS(XMLRPC)
}xmlrpc_http_methods_t;

/**
 * XML Tags
 * This enum contains the number for each known tag
 */
typedef enum
{
	XMLRPC_TAG_NONE_UNKNOWN = SEBS_PARSE_SEEKSTRING_NOT_FOUND ,//!< ROSC_XMLRPC_TAG_NONE_UNKNOWN
	XMLRPC_TAG_STRINGS(XMLRPC) //!< __XMLRPC_TAG_STRINGS
	//!< imports all string numbers from autogenerated file
} parse_xmlrpc_tags_t;

/**
 * XML Attributes
 */
typedef enum
{
	XMLRPC_ATTRIBUTE_NONE_UNKNOWN = SEBS_PARSE_SEEKSTRING_NOT_FOUND ,
	XMLRPC_ATTRIBUTE_STRINGS(XMLRPC)
	//!< imports all string numbers from autogenerated file
} parse_xmlrpc_attrib_t;

typedef enum
{
	XMLRPC_RESULT_NONE,
	XMLRPC_RESULT_REQUEST_SENT,
	XMLRPC_RESULT_CONTENT_LENGTH,
	XMLRPC_RESULT_METHOD_NAME,
	XMLRPC_RESULT_CALLERID,
	XMLRPC_RESULT_PUBLISHER_UPDATE_URL,
}xmlrpc_result_handling_t;

typedef enum
{
	XMLRPC_METHODNAME_UNKNOWN=SEBS_PARSE_SEEKSTRING_NOT_FOUND ,
    XMLRPC_SLAVE_METHODNAMES(XMLRPC),
}xmlrpc_ros_methodname_t;

typedef enum
{
	XMLRPC_TAG_STATE_NONE,
	XMLRPC_TAG_STATE_METHODRC, //<! inside methodCall or methodResponse tag
	XMLRPC_TAG_STATE_PARAMS,
	XMLRPC_TAG_STATE_PARAM,
	XMLRPC_TAG_STATE_VALUE,
}
xmlrpc_tag_state_t;

typedef enum
{
	XMLRPC_TYPE_TAG_NONE,
	XMLRPC_TYPE_TAG_BOOLEAN,
	XMLRPC_TYPE_TAG_STRING,
	XMLRPC_TYPE_TAG_INT,
	XMLRPC_TYPE_TAG_DOUBLE,
}xmlrpc_type_tag_t;

typedef enum
{
	XMLRPC_ARRAY_STATE_NONE,
	XMLRPC_ARRAY_STATE_ARRAY,
	XMLRPC_ARRAY_STATE_DATA,
	XMLRPC_ARRAY_STATE_VALUE,
}xmlrpc_array_state_t;



typedef enum
{
	XMLRPC_CLIENT_TYPE_NO_CLIENT,
	XMLRPC_CLIENT_TYPE_REGISTER,
	XMLRPC_CLIENT_TYPE_UNREGISTER,
	XMLRPC_CLIENT_TYPE_REQUEST_TOPIC,
}xmlrpc_client_type_t;


typedef struct
{

	xmlrpc_t xmlrpc_type; //!< type (server or client)
	xmlrpc_client_type_t client_type; //!< stores type for client mode

	xmlrpc_state_t xmlrpc_state;	//!< state of the handler
	xmlrpc_result_handling_t result_handling; //!< if the handler called a function this must be set to specify handling of the result

	//HTTP variables
	uint16_t http_response_code; //!<saves the response code
	xmlrpc_http_methods_t method; //!< the method requested
	xmlrpc_http_actions_t action; //!< the action requested

	//XML variables
	uint32_t xml_length;	//!< storage for the xml length from the header
	xmlrpc_tag_state_t tag_state; //!< defines where we are inside a xml document
	xmlrpc_type_tag_t type_tag; //!< what is the current tag type
	uint8_t param_no; //!< number of the param tag
	uint32_t array_level; //!< depth inside nested xmlrpc arrays
	uint32_t array_value_number[XMLRPC_MAX_ARRAY_NESTING]; //!<
	xmlrpc_array_state_t array_state; //!< stores the current state of the array
	xmlrpc_ros_methodname_t rpc_methodname; //!< stores the current methodname

	char caller_id[__NODENAME_MAX_LEN__]; //!< stores the id of the caller

	/**
	 * This union contains data from the main
	 * xmlrpc submodes. Http and xml parser and message generator.
	 */
	union
	{
		socket_connect_data_t connect; /*!< connection handler state*/
		sebs_parse_http_data_t http; /*!< http parser state*/
		sebs_parse_xml_data_t xml; /*!< xml parser state*/
		sebs_msggen_t gen; /*!< message generation state*/
	};

	const void * genHeaderData[1];
	const void * genPayloadData[5]; /*!< pointers to payload data for message generation*/
	sebs_parse_copy2buffer_data_t copy2buffer; //!< stores state for copying data into buffer


}xmlrpc_data_t;




sebs_parse_return_t xmlrpc(sebs_parser_data_t* pdata);

#endif /* XMLRPC_H_ */

#include "rt_errno.h"

#define CASE_CODE(E)  case E: return #E

/**
 * \brief Maps the error code, to its string equivalent
 *
 * \param The error code
 *
 * \retval The string equivalent for the error code
 */
const char * rt_errno2string(rt_errno err)
{
    switch (err) {
        CASE_CODE (XSUCCESS);
        CASE_CODE (ERRNO_NO_ELEMENT);
        CASE_CODE (ERRNO_INVALID_ARGU);
        CASE_CODE (ERRNO_INVALID_VAL);
        CASE_CODE (ERRNO_ACCESS_DENIED);
        CASE_CODE (ERRNO_RANGE);
        CASE_CODE (ERRNO_TMDOUT);
        CASE_CODE (ERRNO_FATAL);
        CASE_CODE (ERRNO_WARNING);
        CASE_CODE (ERRNO_MEM_ALLOC);
        CASE_CODE (ERRNO_MEM_FREE);
        CASE_CODE (ERRNO_MEM_INVALID);
        CASE_CODE (ERRNO_SEMPHORE_POST);
        CASE_CODE (ERRNO_SEMPHORE_WAIT);
        CASE_CODE (ERRNO_MUTEX_LOCK);
        CASE_CODE (ERRNO_MUTEX_UNLOCK);

        CASE_CODE (ERRNO_THRD_CREATE);
        CASE_CODE (ERRNO_THRD_DETACH);
        
        CASE_CODE (ERRNO_LOGGING_INIT);
        CASE_CODE (ERRNO_LOGGING_FG_FILTER_MATCH);
        CASE_CODE (ERRNO_LOGGING_FD_FILTER_MATCH);

        CASE_CODE (ERRNO_MQ_INVALID);
        CASE_CODE (ERRNO_MQ_NO_MEMORY);
        CASE_CODE (ERRNO_MQ_NO_RESOURCE);

        CASE_CODE (ERRNO_PCAP);
        CASE_CODE (ERRNO_PCAP_ERROR);
        CASE_CODE (ERRNO_PCAP_OPEN);
        CASE_CODE (ERRNO_PCAP_COMPILE);
        
        CASE_CODE (ERRNO_THROUGHPUT_NO_FILE);
        
        CASE_CODE (ERRNO_RPC_INVALID_ID);
        CASE_CODE (ERRNO_RPC_SIZE);


        CASE_CODE (ERRNO_YAML);

        CASE_CODE (ERRNO_UI_YAML_ARGU);
        CASE_CODE (ERRNO_UI_A29);
        CASE_CODE (ERRNO_UI_LOCAL);

        CASE_CODE (ERRNO_MA_YAML_NO_FILE);
        CASE_CODE (ERRNO_MA_REGISTRY);
        CASE_CODE (ERRNO_MA_KEEPALIVE);
        CASE_CODE (ERRNO_MA_INTERACTIVE);

        CASE_CODE (ERRNO_DECODER_YAML_ARGU);
        
        CASE_CODE (ERRNO_SESSION_CB_ALLOC);

        CASE_CODE (ERRNO_TM);
        CASE_CODE (ERRNO_TM_INVALID);
        CASE_CODE (ERRNO_TM_NO_IDLE);
        CASE_CODE (ERRNO_TM_EXSIT);

        CASE_CODE (ERRNO_SOCK_ERR);
        CASE_CODE (ERRNO_SOCK_CLOSE);

        CASE_CODE (ERRNO_DC_PEER_CLOSE);
        CASE_CODE (ERRNO_DC_PEER_ERROR);

        CASE_CODE (ERRNO_TASK_EXSIT);
        CASE_CODE (ERRNO_TASK_CREATE);
        CASE_CODE (ERRNO_TASK_MULTIPLE_REG);
	CASE_CODE (ERRNO_SG);
        default:
            return "UNKNOWN_ERROR";
    }
}

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RA_H_RPCGEN
#define _RA_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


struct rsrc_req {
	int req;
};
typedef struct rsrc_req rsrc_req;

struct reply {
	int rep;
};
typedef struct reply reply;

#define ResourceAllocator 0241
#define RA 2

#if defined(__STDC__) || defined(__cplusplus)
#define allocate 1
extern  enum clnt_stat allocate_2(rsrc_req *, reply *, CLIENT *);
extern  bool_t allocate_2_svc(rsrc_req *, reply *, struct svc_req *);
extern int resourceallocator_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define allocate 1
extern  enum clnt_stat allocate_2();
extern  bool_t allocate_2_svc();
extern int resourceallocator_2_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_rsrc_req (XDR *, rsrc_req*);
extern  bool_t xdr_reply (XDR *, reply*);

#else /* K&R C */
extern bool_t xdr_rsrc_req ();
extern bool_t xdr_reply ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RA_H_RPCGEN */
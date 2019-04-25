
/*
 * RA.x: remote resource allocator (manager).
 */

struct rsrc_req{
	int req;
};

struct reply{
        int rep;
};


/* program definition */
program ResourceAllocator{
	version RA{
		reply allocate(rsrc_req)=1;
		reply release(rsrc_req)=2;
	}=2;
}=0241;

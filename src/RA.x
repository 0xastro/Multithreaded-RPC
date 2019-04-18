struct rsrc_req{
	int req;
};

struct reply{
        int rep;
};

program ResourceAllocator{
	version RA{
		reply allocate(rsrc_req)=1;
	}=2;
}=0241;

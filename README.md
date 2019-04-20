# Multithreaded-RPC
Multithreaded RPC server (Allocator- Resource Manager) using rpcgen.

The multithreaded server is responsible of allocating n-number of Resources with respect to each client request. Each request is handled by a thread on the server to allocate the resources.
```
Server @PrivateResources := 10;

  Client 1 Ask for 5 Resources
  Client 2 Ask for 2 Resources
  Client 3 Ask for 6 Resources

Client 1 and Client 2 will be able to execute in parallel
while Client 3 will be blocked on a conditional variable where
@PrivateResources !=6;
```

```C 
for each client request Create_Thread
    for each Thread(i)
        while (RequestedResources < AvailableResources);
        
        AquireLock();
        /*Update the AvailableResources*/
        AvailableResources = AvailableResources-RequestedResources;
        SEND RequestedResources ACK back to the client
        /*Do Some Work*/
        ...
    
        /*Update the AvailableResources*/
        AvailableResources = AvailableResources+RequestedResources;        
        ReleaseLock();

    end
end
```

You shouldn't use any of these commands; where most of the generated server and client files have been modified.
``` sh
rpcgen -a -M RA.x
make clean -f Makefile.RA

```
only 
``` sh
make -f Makefile.RA
```

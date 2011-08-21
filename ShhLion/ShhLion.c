// Implementation is based on code from cbhense.
// Source: https://discussions.apple.com/thread/461317?start=210&tstart=0

// Description:
// Intel Manual on 945 Chipset, page 81, states that
// the offset for MMR space is at offset 44h of PCI Device 0 (Host Bridge)
// Here we read 0xfed14f10
// It further states, page 186, that in MMR at offset 0xf10
// we can find a 32 bit value of which bits 1 and 0 control the Enhanced Power Management Mode of the Core Duo Processor:
// 00: All functions allowed
// 01: Disable C3 and C4
// 10: Disable C4
// The default value is 00
// This whole code is about setting this to 10
// Because after wake up from sleep it is set back to 00 we 
// have a kernel thread which set to 10 every second

#include <mach/mach_types.h>
#include <IOKit/IOLib.h>


int stop;
UInt8 data_org;
IOPhysicalAddress address = 0xfed14f10;


void shh_lion_run(void *argument, wait_result_t wr);
kern_return_t shh_lion_start(kmod_info_t * ki, void * d);
kern_return_t shh_lion_stop(kmod_info_t * ki, void * d);


void shh_lion_run(void *argument, wait_result_t wr) {
    
    UInt8 data;
    
    // Read original power management value
    data_org = IOMappedRead8(address);
    
    // Set new power management value (Disable C4)
    data = data_org | 0x2;
    stop = 0;
    
    // Write new power management value every 10 seconds
    while (stop == 0)
    {
        IOMappedWrite8(address, data);
        IODelay(1000000);
    }
}


kern_return_t shh_lion_start (kmod_info_t * ki, void * d) {
    
    thread_t shhLionThreadRef = NULL;
    
	if (KERN_SUCCESS != kernel_thread_start(shh_lion_run, NULL, &shhLionThreadRef) ) {
        IOLog("Failed to create ShhLion worker thread");
        return KERN_FAILURE;
	}
	else {
		thread_deallocate(shhLionThreadRef);
		shhLionThreadRef = NULL;
        
        IOLog("ShhLion KEXT has loaded!\n");
        return KERN_SUCCESS;
	}
}


kern_return_t shh_lion_stop (kmod_info_t * ki, void * d) {
    // Before we can unload this Kernel Extension we have to signal shh_lion_run to exit.
    // If we unload while shh_lion_run is still running the wake up timer of the loop
    // will cause a kernel panic
    stop = 1;
    IODelay(2000000);
    
    // Restore power management value
    IOMappedWrite8(address,data_org);
    IOLog("ShhLion KEXT has unloaded!\n");
    return KERN_SUCCESS;
}




#pragma once
#include <pthread.h>
#import "DOCXObjc.h"

@class RDVLocker;
@class RDVEvent;
@class DOCXCache;
@class DOCXVFinder;

struct DOCXThreadBack
{
    SEL OnCacheRendered;
    SEL OnCacheDestroy;
    SEL OnFound;
};

@interface DOCXThread : NSObject
{
    dispatch_queue_t m_queue;
    struct DOCXThreadBack m_back;
    id m_notifier;
}
-(bool)create:(id)notifier :(const struct DOCXThreadBack*)disp;
-(void)destroy;
-(void)start_render:(DOCXCache *)cache;
-(void)end_render:(DOCXCache *)cache;
-(void)start_find:(DOCXVFinder *)finder;
-(void)end_page:(DOCXPage *)page;

-(void)notify_render:(DOCXCache *)cache;
-(void)notify_dealloc:(DOCXCache *)cache;
-(void)notify_find:(DOCXVFinder *)finder;
@end

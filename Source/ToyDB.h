/*
 *  ToyDB.h
 *  ToyCouch
 *
 *  Created by Jens Alfke on 6/19/10.
 *  Copyright 2010 Jens Alfke. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>
@class FMDatabase, ToyDocument;


extern NSString* const ToyDBChangeNotification;


typedef struct {
    NSString* startKey;
    NSString* endKey;
    int skip;
    int limit;
    BOOL descending;
    BOOL includeDocs;
    BOOL updateSeq;
} ToyDBQueryOptions;

extern const ToyDBQueryOptions kDefaultToyDBQueryOptions;


/** A ToyCouch database. Acts primarily as a container for named tables (CLCaches). */
@interface ToyDB : NSObject
{
    NSString* _path;
    FMDatabase *_fmdb;
    BOOL _open;
    NSInteger _transactionLevel;
    BOOL _transactionFailed;
}    
        
- (id) initWithPath: (NSString*)path;
- (BOOL) open;
#if SQLITE_VERSION_NUMBER >= 3005000
- (BOOL) openWithFlags:(int)flags;
#endif
- (BOOL) close;

@property (readonly) NSString* path;
@property (readonly) NSString* name;
@property (readonly) BOOL exists;
@property (readonly) int error;

- (void) beginTransaction;
- (void) endTransaction;
@property BOOL transactionFailed;

- (int) compact;

// DOCUMENTS:

+ (BOOL) isValidDocumentID: (NSString*)str;
- (NSString*) generateDocumentID;

@property (readonly) NSUInteger documentCount;
@property (readonly) NSUInteger lastSequence;

- (ToyDocument*) getDocumentWithID: (NSString*)docID;
- (ToyDocument*) getDocumentWithID: (NSString*)docID revisionID: (NSString*)revID;

- (ToyDocument*) putDocument: (ToyDocument*)document
                     withID: (NSString*)docID 
                 revisionID: (NSString*)revID
                     status: (int*)outStatus;
- (ToyDocument*) createDocument: (ToyDocument*)document
                         status: (int*)outStatus;
- (ToyDocument*) deleteDocumentWithID: (NSString*)docID 
                           revisionID: (NSString*)revID
                               status: (int*)outStatus;;

- (NSArray*) changesSinceSequence: (int)lastSequence
                          options: (const ToyDBQueryOptions*)options;

// QUERIES:

- (NSDictionary*) getAllDocs: (const ToyDBQueryOptions*)options;

@end
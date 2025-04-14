//
//  RDEmbeddedFilesViewController.h
//  PDFMaster
//
//  Created by Federico Vellani on 15/06/2021.
//

#import <UIKit/UIKit.h>
#import "RDEmbeddedFilesTableViewCell.h"

@interface RDEmbedFile: NSObject
-(id)init:(NSString *)name :(NSString *)description :(NSString *)path;
@property NSString *name;
@property NSString *description;
@property NSString *path;
@end

@interface RDEmbeddedFilesViewController : UIViewController <UITableViewDelegate, UITableViewDataSource, UIDocumentPickerDelegate, RDEmbeddedFilesTableViewCellDelegate> {
    NSMutableArray *items;
}

@property (strong, nonatomic) RDPDFDoc *doc;
@property (strong, nonatomic) IBOutlet UIView *container;
@property (strong, nonatomic) IBOutlet UIToolbar *toolbar;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *embedLabel;
@property (strong, nonatomic) IBOutlet UITableView *filesTableView;
@property (nonatomic) BOOL readonly;

- (IBAction)dismissView:(id)sender;
- (IBAction)addFiles:(id)sender;

@end

//
//  RDEmbeddedFilesTableViewCell.h
//  PDFMaster
//
//  Created by Federico Vellani on 17/06/2021.
//

#import <UIKit/UIKit.h>

@protocol RDEmbeddedFilesTableViewCellDelegate <NSObject>
- (void)extractAttachmentAtIndexPath:(NSIndexPath *)indexPath;
- (void)deleteAttachmentAtIndexPath:(NSIndexPath *)indexPath;
@end

@interface RDEmbeddedFilesTableViewCell : UITableViewCell

@property (strong, nonatomic) NSIndexPath *indexPath;
@property (weak, nonatomic) id<RDEmbeddedFilesTableViewCellDelegate> delegate;

@property (strong, nonatomic) IBOutlet UILabel *nameLabel;
@property (strong, nonatomic) IBOutlet UILabel *descriptionLabel;

-(IBAction)extractButtonTapped:(id)sender;
-(IBAction)deleteButtonTapped:(id)sender;

@end

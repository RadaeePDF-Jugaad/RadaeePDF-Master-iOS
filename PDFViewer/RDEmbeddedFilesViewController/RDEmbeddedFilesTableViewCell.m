//
//  RDEmbeddedFilesTableViewCell.m
//  PDFMaster
//
//  Created by Federico Vellani on 17/06/2021.
//

#import "RDEmbeddedFilesTableViewCell.h"

@implementation RDEmbeddedFilesTableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
    // Configure the view for the selected state
}

- (void)extractButtonTapped:(id)sender {
    _indexPath = [(UITableView *)self.superview indexPathForCell: self];
    [_delegate extractAttachmentAtIndexPath:_indexPath];
}

- (void)deleteButtonTapped:(id)sender {
    _indexPath = [(UITableView *)self.superview indexPathForCell: self];
    [_delegate deleteAttachmentAtIndexPath:_indexPath];
}

@end

//
//  RDEmbeddedFilesViewController.m
//  PDFMaster
//
//  Created by Federico Vellani on 15/06/2021.
//

#import "RDEmbeddedFilesViewController.h"

@interface RDEmbeddedFilesViewController ()

@end

@implementation RDEmbeddedFilesViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    _filesTableView.delegate = self;
    _filesTableView.dataSource = self;
    _container.backgroundColor = [RDUtils radaeeWhiteColor];
    _toolbar.backgroundColor = [RDUtils radaeeWhiteColor];
    _toolbar.layer.cornerRadius = 10.0f;
    _filesTableView.backgroundColor = [RDUtils radaeeWhiteColor];
    _filesTableView.layer.cornerRadius = 10.0f;
    _embedLabel.title = NSLocalizedString(@"Embedded files", nil);
}

- (void)getItems {

}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [_doc getEmbedFileCount];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"embedFileCell";
    RDEmbeddedFilesTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        // Load the top-level objects from the custom cell XIB.
        NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"RDEmbeddedFilesTableViewCell" owner:self options:nil];
        // Grab a pointer to the first object (presumably the custom cell, as that's all the XIB should contain).
        cell = [topLevelObjects objectAtIndex:0];
    }
    cell.nameLabel.text = [_doc getEmbedFileName:(int)indexPath.row];
    if ([_doc getEmbedFileDesc:(int)indexPath.row].length > 0) {
        cell.descriptionLabel.hidden = NO;
        cell.descriptionLabel.text = [_doc getEmbedFileDesc:(int)indexPath.row];
    }
    cell.delegate = self;
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 50;
}

- (void)extractAttachmentAtIndexPath:(NSIndexPath *)indexPath {
    NSString *documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    NSString *path = [documents stringByAppendingPathComponent:[NSString stringWithFormat:@"%@", [_doc getEmbedFileName:(int)indexPath.row]]];
    NSURL *fileUrl = [NSURL fileURLWithPath:path];
    if ([_doc getEmbedFileData:(int)indexPath.row :path]) {
        UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:@[fileUrl] applicationActivities:nil];
        [activityViewController setCompletionWithItemsHandler:^(NSString *activityType, BOOL completed, NSArray *returnedItems, NSError *activityError) {
            if (completed) {
                [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
            }
        }];
        if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
        {
            [self presentViewController:activityViewController animated:YES completion:nil];
        }
        else
        {
            UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityViewController];
            [popup presentPopoverFromRect:CGRectMake(self.view.frame.size.width/2, self.view.frame.size.height/2, 0, 0) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        }
    }
}

- (void)deleteAttachmentAtIndexPath:(NSIndexPath *)indexPath {
    if(!_readonly)
    {
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:NSLocalizedString(@"Warning", nil) message:NSLocalizedString(@"Are you sure to delete this embedded file?", nil) preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *confirm = [UIAlertAction actionWithTitle:NSLocalizedString(@"Yes", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            [self->_doc delEmbedFile: (int)indexPath.row];
            [self->items removeObjectAtIndex:indexPath.row];
            [self->_doc save];
            [self->_filesTableView reloadData];
        }];
        UIAlertAction *cancel = [UIAlertAction actionWithTitle:NSLocalizedString(@"No", nil) style:UIAlertActionStyleCancel handler:nil];
        [alert addAction:confirm];
        [alert addAction:cancel];
        [self presentViewController:alert animated:YES completion:nil];
    }
    
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (void)addFiles:(id)sender {
    if(!_readonly)
    {
        UIDocumentPickerViewController* documentPicker = [[UIDocumentPickerViewController alloc] initWithDocumentTypes:@[@"public.data"] inMode:UIDocumentPickerModeImport];
        documentPicker.delegate = self;
        [self presentViewController:documentPicker animated:YES completion:nil];
    }
}

- (void)dismissView:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
    if (controller.documentPickerMode == UIDocumentPickerModeImport) {
        int count = [_doc getEmbedFileCount];
        for (NSURL *fileUrl in urls) {
            [_doc newEmbedFile:fileUrl.path];
            //[_doc addEmbedFileData:_doc :fileUrl.path];
        }
        NSString *file = (urls.count == 1) ? @"File" : @"Files";
        if (count < [_doc getEmbedFileCount]) {
            [_doc save];
            [self showBasicAlertControllerWithTitle:NSLocalizedString(@"Success", nil) message:[NSString stringWithFormat:@"%@ %@", file, NSLocalizedString(@"successfully saved", nil)]];
        } else {
            [self showBasicAlertControllerWithTitle:NSLocalizedString(@"Error", nil) message:[NSString stringWithFormat:@"%@ %@",NSLocalizedString(@"Error on adding new embedded", nil), file.lowercaseString]];
        }
    }
}

- (void)showBasicAlertControllerWithTitle:(NSString *)title message:(NSString *)message
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *conform = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self->_filesTableView reloadData];
    }];
    [alert addAction:conform];
    [self presentViewController:alert animated:YES completion:nil];
}

@end

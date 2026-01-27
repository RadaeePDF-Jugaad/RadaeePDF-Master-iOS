# RadaeePDF SDK Master for iOS

<img src="https://www.radaeepdf.com/wp-content/uploads/2024/08/solo_butterly_midres.png" style="width:100px;"> 

RadaeePDF SDK is a powerful, native PDF rendering and manipulation library for iOS applications. Built from true native C++ code, it provides exceptional performance and a comprehensive set of features for working with PDF documents.

## About RadaeePDF

RadaeePDF SDK is designed to solve most developers' needs with regards to PDF rendering and manipulation. The SDK is trusted across industries worldwide including automotive, banking, publishing, healthcare, and more.

### Key Features

- **PDF ISO32000 Compliance** - Full support for the widely-used PDF format standard
- **High Performance** - True native code compiled from C++ sources for optimal speed
- **Annotations** - Create and manage text annotations, highlights, ink annotations, and more
- **Protection & Encryption** - Full AES256 cryptography for document security
- **Text Handling** - Search, extract, and highlight text with ease
- **Form Editing** - Create, read, and write PDF form fields (AcroForms)
- **Digital Signatures** - Sign and verify PDF documents with digital certificates
- **Multiple View Modes** - Single page, continuous scroll, and more
- **Night Mode** - Built-in dark mode support for better readability

## Quick Start - Run Demo

To quickly test the RadaeePDF SDK demo:

1. **Clone the Repository** (skip if already cloned)
   - Open **Xcode**
   - Click on **Clone Git Repository** from the welcome screen (or go to **Source Control** → **Clone** or use `⌘ + Shift + C`)
   - Paste the repository URL:
     ```
     https://github.com/RadaeePDF-Jugaad/RadaeePDF-Master-iOS.git
     ```
   - Click **Clone** and choose a location to save the project
   - If you get an error about the repository already existing, proceed to step 2

2. **Open the Project**
   - Navigate to the cloned folder (or use **File** → **Open** in Xcode)
   - Double-click on `PDFMaster.xcodeproj` to open the project in Xcode

3. **Select Target Device**
   - In the Xcode toolbar, select a target device (iPhone simulator or connected iOS device)
   - For physical devices, ensure your device is connected and trusted

4. **Configure Simulator Settings** (for iOS Simulator only)
   - **Important for Apple Silicon Macs**: If you're running on an Apple Silicon (M1/M2/M3) Mac, you need to install the **Universal** version of the iOS Simulator (which includes Rosetta support) instead of the default Apple Silicon-only version. In Xcode, go to **Settings** → **Platforms** and download the **Universal** simulator runtime for your target iOS version.
   - In Xcode, select your project in the Project Navigator
   - Select the **PDFMaster** target
   - Go to **Build Settings** tab
   - Search for "Excluded Architectures"
   - Under **Excluded Architectures** > **Debug**, add `arm64` for **Any iOS Simulator SDK**
   - This ensures the project excludes arm64 architecture when building for iOS Simulator
   - The project has universal support for simulator devices

5. **Build and Run**
   - Press **⌘ + R** or click the **Play** button (▶) in the toolbar
   - The app will build and launch on your selected device/simulator

## Installation

### Manual Installation

1. Download the RadaeePDF SDK framework from [https://www.radaeepdf.com/](https://www.radaeepdf.com/)
2. Drag the framework into your Xcode project (PDFLib)
3. Add the framework libRDDLib.a to your target's "Build Phases/Link Binary With Libraries"

## Getting Started

### Initialize the Library

Before using RadaeePDF, you need to activate your license. There are two approaches depending on your implementation:

#### Approach 1: Using RadaeePDFPlugin (Recommended for UI-based apps)

This approach uses the `RadaeePDFPlugin` class which provides a convenient wrapper for license activation and PDF viewing.

**Swift**

```swift
import UIKit

class ViewController: UIViewController {
    private var pdfPlugin: RadaeePDFPlugin?

    override func viewDidLoad() {
        super.viewDidLoad()

        // Initialize the PDF plugin
        pdfPlugin = RadaeePDFPlugin.pluginInit()
    }

    func openPDF(path: String) {
        // Activate license before opening PDF
        pdfPlugin?.activateLicense(withSerialKey: "YOUR-LICENSE-KEY-HERE")

        // Open the PDF viewer
        if let pdfViewController = pdfPlugin?.show(path, withPassword: "") as? UIViewController {
            pdfViewController.modalPresentationStyle = .fullScreen
            present(pdfViewController, animated: true)
        }
    }
}
```

**Objective-C**

```objc
#import "RadaeePDFPlugin.h"

@interface ViewController ()
@property (nonatomic, strong) RadaeePDFPlugin *pdfPlugin;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Initialize the PDF plugin
    self.pdfPlugin = [RadaeePDFPlugin pluginInit];
}

- (void)openPDFAtPath:(NSString *)path {
    // Activate license before opening PDF
    [self.pdfPlugin activateLicenseWithSerialKey:@"YOUR-LICENSE-KEY-HERE"];

    // Open the PDF viewer
    UIViewController *pdfViewController = [self.pdfPlugin show:path withPassword:@""];
    if (pdfViewController) {
        pdfViewController.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:pdfViewController animated:YES completion:nil];
    }
}

@end
```

#### Approach 2: Using Global Activation (For AppDelegate initialization)

Use this approach when you want to activate the license once at app startup, typically in your AppDelegate. This is useful when working with PDF documents directly without the viewer component, or when you want to ensure the license is activated before any PDF operations throughout your app.

**Objective-C**

```objc
#import "RDVGlobal.h"

// Declare the external global variable
extern NSString *g_serial;

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Set the license key
    g_serial = @"YOUR-LICENSE-KEY-HERE";

    // Initialize RadaeePDF engine with the license
    [RDVGlobal Init];

    return YES;
}

@end
```

**Swift**

For Swift projects, you need to expose `g_serial` through a bridging header and create an Objective-C helper:

```objc
// In your Bridging Header or a helper file
#import "RDVGlobal.h"

extern NSString *g_serial;

// Helper function for Swift
void RadaeePDF_SetLicense(NSString *key) {
    g_serial = key;
    [RDVGlobal Init];
}
```

```swift
// In AppDelegate.swift
func application(_ application: UIApplication,
    didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {

    // Activate RadaeePDF license at app startup
    RadaeePDF_SetLicense("YOUR-LICENSE-KEY-HERE")

    return true
}
```

**Important Notes:**
- Replace `YOUR-LICENSE-KEY-HERE` with your actual RadaeePDF license key
- The license key is a hexadecimal string provided by RadaeePDF
- For Approach 1: The `activateLicenseWithSerialKey:` method internally sets `g_serial` and calls `[RDVGlobal Init]`
- For Approach 2: You must set `g_serial` before calling `[RDVGlobal Init]`
- The `[RDVGlobal Init]` method calls `Global_active()` to validate the license and initializes fonts, CMaps, and other resources

### Open and Display a PDF

#### Using RadaeePDFPlugin (Recommended)

The `RadaeePDFPlugin` provides several methods to open PDFs with a built-in viewer:

**Swift**

```swift
import UIKit

class ViewController: UIViewController {
    private var pdfPlugin: RadaeePDFPlugin?

    override func viewDidLoad() {
        super.viewDidLoad()
        pdfPlugin = RadaeePDFPlugin.pluginInit()
    }

    // Basic open with password
    func openPDF(path: String, password: String) {
        pdfPlugin?.activateLicense(withSerialKey: "YOUR-LICENSE-KEY")

        if let pdfVC = pdfPlugin?.show(path, withPassword: password) as? UIViewController {
            pdfVC.modalPresentationStyle = .fullScreen
            present(pdfVC, animated: true)
        }
    }

    // Open with options: page, readOnly, autoSave, author
    func openPDFWithOptions(path: String) {
        pdfPlugin?.activateLicense(withSerialKey: "YOUR-LICENSE-KEY")

        if let pdfVC = pdfPlugin?.show(
            path,
            atPage: 0,              // Starting page (0-based)
            withPassword: "",       // Password (empty if none)
            readOnly: false,        // Read-only mode
            autoSave: true,         // Auto-save changes
            author: "User"          // Author for annotations
        ) as? UIViewController {
            pdfVC.modalPresentationStyle = .fullScreen
            present(pdfVC, animated: true)
        }
    }

    // Open from app bundle assets
    func openFromAssets(fileName: String) {
        pdfPlugin?.activateLicense(withSerialKey: "YOUR-LICENSE-KEY")

        if let pdfVC = pdfPlugin?.openFromAssets(fileName, withPassword: "") as? UIViewController {
            pdfVC.modalPresentationStyle = .fullScreen
            present(pdfVC, animated: true)
        }
    }
}
```

**Objective-C**

```objc
#import "RadaeePDFPlugin.h"

@interface ViewController ()
@property (nonatomic, strong) RadaeePDFPlugin *pdfPlugin;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.pdfPlugin = [RadaeePDFPlugin pluginInit];
}

// Basic open with password
- (void)openPDFAtPath:(NSString *)path password:(NSString *)password {
    [self.pdfPlugin activateLicenseWithSerialKey:@"YOUR-LICENSE-KEY"];

    UIViewController *pdfVC = [self.pdfPlugin show:path withPassword:password];
    if (pdfVC) {
        pdfVC.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:pdfVC animated:YES completion:nil];
    }
}

// Open with options
- (void)openPDFWithOptions:(NSString *)path {
    [self.pdfPlugin activateLicenseWithSerialKey:@"YOUR-LICENSE-KEY"];

    UIViewController *pdfVC = [self.pdfPlugin show:path
                                            atPage:0
                                      withPassword:@""
                                          readOnly:NO
                                          autoSave:YES
                                            author:@"User"];
    if (pdfVC) {
        pdfVC.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:pdfVC animated:YES completion:nil];
    }
}

// Open from app bundle assets
- (void)openFromAssets:(NSString *)fileName {
    [self.pdfPlugin activateLicenseWithSerialKey:@"YOUR-LICENSE-KEY"];

    UIViewController *pdfVC = [self.pdfPlugin openFromAssets:fileName withPassword:@""];
    if (pdfVC) {
        pdfVC.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:pdfVC animated:YES completion:nil];
    }
}

@end
```

#### Using RDPDFDoc Directly (Low-level API)

For direct PDF manipulation without the viewer, use `RDPDFDoc`:

**Swift**

```swift
func openPDFDirect(filePath: String, password: String) {
    let doc = RDPDFDoc()
    let result = doc.open(filePath, password)

    switch result {
    case 0:  // Success
        let pageCount = doc.pageCount()
        print("PDF opened successfully. Page count: \(pageCount)")
        // Perform operations...
        doc.close()
    case -1:
        print("Error: Password required")
    case -2:
        print("Error: Unknown encryption")
    case -3:
        print("Error: Damaged or invalid format")
    case -10:
        print("Error: Access denied or invalid file path")
    default:
        print("Error: Unknown error (\(result))")
    }
}
```

**Objective-C**

```objc
- (void)openPDFDirect:(NSString *)filePath password:(NSString *)password {
    RDPDFDoc *doc = [[RDPDFDoc alloc] init];
    int result = [doc open:filePath :password];

    switch (result) {
        case 0:  // Success
            NSLog(@"PDF opened successfully. Page count: %d", [doc pageCount]);
            // Perform operations...
            [doc close];
            break;
        case -1:
            NSLog(@"Error: Password required");
            break;
        case -2:
            NSLog(@"Error: Unknown encryption");
            break;
        case -3:
            NSLog(@"Error: Damaged or invalid format");
            break;
        case -10:
            NSLog(@"Error: Access denied or invalid file path");
            break;
        default:
            NSLog(@"Error: Unknown error (%d)", result);
            break;
    }
}
```

### Open PDF from Memory

**Swift**

```swift
func openPDFFromMemory(data: Data, password: String) {
    let doc = RDPDFDoc()
    let result = data.withUnsafeBytes { bytes -> Int32 in
        return doc.openMem(UnsafeMutableRawPointer(mutating: bytes.baseAddress!),
                          Int32(data.count),
                          password)
    }

    if result == 0 {
        print("PDF opened from memory successfully. Page count: \(doc.pageCount())")
        // Perform operations...
        doc.close()
    }
}
```

**Objective-C**

```objc
- (void)openPDFFromMemory:(NSData *)data password:(NSString *)password {
    RDPDFDoc *doc = [[RDPDFDoc alloc] init];
    int result = [doc openMem:(void *)[data bytes] :(int)[data length] :password];

    if (result == 0) {
        NSLog(@"PDF opened from memory. Page count: %d", [doc pageCount]);
        // Perform operations...
        [doc close];
    }
}
```

## Common Operations

### Get Page Count

**Swift**

```swift
let pageCount = doc.pageCount()
print("Document has \(pageCount) pages")
```

**Objective-C**

```objc
int pageCount = [doc pageCount];
NSLog(@"Document has %d pages", pageCount);
```

### Navigate to a Specific Page

When using `RadaeePDFPlugin`, you can get the current page or navigate programmatically. For programmatic navigation, access the `PDFReaderCtrl` through the plugin's viewer controller and use `PDFGoto:`.

**Swift**

```swift
// Navigate to a specific page using PDFReaderCtrl
// First, get the reader controller from the presented view controller
if let readerCtrl = presentedViewController as? PDFReaderCtrl {
    readerCtrl.pdfGoto(5)  // Go to page 5 (0-based index)
}
```

**Objective-C**

```objc
// Navigate to a specific page using PDFReaderCtrl
// First, get the reader controller from the presented view controller
PDFReaderCtrl *readerCtrl = (PDFReaderCtrl *)self.presentedViewController;
if (readerCtrl) {
    [readerCtrl PDFGoto:5];  // Go to page 5 (0-based index)
}
```

**Using PDFLayoutView directly:**

If you have direct access to the `PDFLayoutView` (for custom implementations):

```objc
// Objective-C
[pdfLayoutView vGoto:5];  // Navigate to page 5
```

```swift
// Swift
pdfLayoutView.vGoto(5)  // Navigate to page 5
```

### Set View Mode

Configure the PDF view mode before opening a document. View modes control how pages are displayed.

**View Mode Values:**
- `0` - Vertical scroll
- `1` - Horizontal scroll (left to right)
- `2` - Curl effect (page flip animation)
- `3` - Single page
- `4` - Single page cover (first page single, others dual in landscape)
- `5` - Reflow
- `6` - Dual page in landscape
- `7` - Dual page with cover (like Acrobat)
- `8` - Dual page without cover

**Swift**

```swift
// Set view mode before opening PDF
pdfPlugin?.setReaderViewMode(0)  // Vertical scroll

// Open PDF after setting view mode
pdfPlugin?.activateLicense(withSerialKey: "YOUR-LICENSE-KEY")
if let pdfVC = pdfPlugin?.show(path, withPassword: "") as? UIViewController {
    present(pdfVC, animated: true)
}
```

**Objective-C**

```objc
// Set view mode before opening PDF
[self.pdfPlugin setReaderViewMode:1];  // Horizontal scroll

// Open PDF after setting view mode
[self.pdfPlugin activateLicenseWithSerialKey:@"YOUR-LICENSE-KEY"];
UIViewController *pdfVC = [self.pdfPlugin show:path withPassword:@""];
if (pdfVC) {
    [self presentViewController:pdfVC animated:YES completion:nil];
}
```

### Enable Night Mode (Dark Mode)

Enable dark mode for better readability in low-light conditions.

**Swift**

```swift
// Enable dark mode globally before opening PDF
GLOBAL.g_dark_mode = true

// Or using RDVGlobal
let global = pdfPlugin?.getGlobal() as? RDVGlobal
global?.g_dark_mode = true
```

**Objective-C**

```objc
// Enable dark mode globally before opening PDF
GLOBAL.g_dark_mode = YES;

// Or get global settings from plugin
RDVGlobal *global = [self.pdfPlugin getGlobal];
global.g_dark_mode = YES;
```

### Customizing Reader Appearance

Configure colors and appearance settings:

**Swift**

```swift
// Set thumbnail bar background color
pdfPlugin?.setThumbnailBGColor(0xFF333333)

// Set reader background color
pdfPlugin?.setReaderBGColor(0xFFE0E0E0)

// Set thumbnail height
pdfPlugin?.setThumbHeight(80.0)

// Enable/disable first page as cover
pdfPlugin?.setFirstPageCover(true)

// Set double-tap zoom mode
pdfPlugin?.setDoubleTapZoomMode(2)

// Enable immersive mode
pdfPlugin?.setImmersive(true)
```

**Objective-C**

```objc
// Set thumbnail bar background color
[self.pdfPlugin setThumbnailBGColor:0xFF333333];

// Set reader background color
[self.pdfPlugin setReaderBGColor:0xFFE0E0E0];

// Set thumbnail height
[self.pdfPlugin setThumbHeight:80.0];

// Enable/disable first page as cover
[self.pdfPlugin setFirstPageCover:YES];

// Set double-tap zoom mode (0, 1, or 2)
[self.pdfPlugin setDoubleTapZoomMode:2];

// Enable immersive mode (hides UI elements)
[self.pdfPlugin setImmersive:YES];
```

### Text Highlighting (Professional License)

Add text markup annotations (highlight, underline, strikeout, squiggly):

**Markup Types:**
- `0` - Highlight
- `1` - Underline
- `2` - Strikeout
- `4` - Squiggly

**Swift**

```swift
func addMarkupAnnotation(to doc: RDPDFDoc, pageIndex: Int,
                         startCharIndex: Int, endCharIndex: Int,
                         markupType: Int) -> Bool {
    guard let page = doc.page(Int32(pageIndex)) else { return false }

    page.objsStart(false)

    // Get markup color based on type
    var color: UInt32
    switch markupType {
    case 0: color = GLOBAL.g_annot_highlight_clr  // Highlight (yellow)
    case 1: color = GLOBAL.g_annot_underline_clr  // Underline (blue)
    case 2: color = GLOBAL.g_annot_strikeout_clr  // Strikeout (red)
    case 4: color = GLOBAL.g_annot_squiggly_clr   // Squiggly (green)
    default: color = 0xFFFFFF00
    }

    // Add markup annotation between character indices
    page.addAnnotMarkup(Int32(startCharIndex), Int32(endCharIndex), Int32(markupType), Int32(color))

    doc.save()
    page.close()
    return true
}
```

**Objective-C**

```objc
- (BOOL)addMarkupAnnotationTo:(RDPDFDoc *)doc
                    pageIndex:(int)pageIndex
               startCharIndex:(int)startIndex
                 endCharIndex:(int)endIndex
                   markupType:(int)type {
    RDPDFPage *page = [doc page:pageIndex];
    if (!page) return NO;

    [page objsStart:NO];

    // Get markup color based on type
    int color;
    switch (type) {
        case 0: color = GLOBAL.g_annot_highlight_clr; break;  // Highlight
        case 1: color = GLOBAL.g_annot_underline_clr; break;  // Underline
        case 2: color = GLOBAL.g_annot_strikeout_clr; break;  // Strikeout
        case 4: color = GLOBAL.g_annot_squiggly_clr; break;   // Squiggly
        default: color = 0xFFFFFF00; break;
    }

    // Add markup annotation between character indices
    [page addAnnotMarkup:startIndex :endIndex :type :color];

    [doc save];
    [page close];
    return YES;
}
```

### Remove Annotation

**Swift**

```swift
func removeAnnotation(from doc: RDPDFDoc, pageIndex: Int, annotIndex: Int) -> Bool {
    guard let page = doc.page(Int32(pageIndex)) else { return false }

    page.objsStart(false)

    if let annot = page.annotAtIndex(Int32(annotIndex)) {
        annot.removeFromPage()
        doc.save()
        page.close()
        return true
    }

    page.close()
    return false
}
```

**Objective-C**

```objc
- (BOOL)removeAnnotationFrom:(RDPDFDoc *)doc pageIndex:(int)pageIndex annotIndex:(int)annotIndex {
    RDPDFPage *page = [doc page:pageIndex];
    if (!page) return NO;

    [page objsStart:NO];

    RDPDFAnnot *annot = [page annotAtIndex:annotIndex];
    if (annot) {
        [annot removeFromPage];
        [doc save];
        [page close];
        return YES;
    }

    [page close];
    return NO;
}
```

### Save Document

**Swift**

```swift
// Save changes to the same file
doc.save()

// Save to a new file
let success = doc.saveAs("/path/to/newfile.pdf", false)  // false = remove security settings
```

**Objective-C**

```objc
// Save changes to the same file
[doc save];

// Save to a new file
BOOL remSecurity = NO;  // Keep security settings
[doc saveAs:@"/path/to/newfile.pdf" :remSecurity];
```

### Encrypt Document

Encrypt a PDF with password protection:

**Permission Flags (combine with OR):**
As defined in PDF Reference 1.7:
- `0x4` (4) - bit 3: Print the document
- `0x8` (8) - bit 4: Modify document contents
- `0x10` (16) - bit 5: Extract text or images

For additional permission bits, refer to the PDF Reference 1.7 specification.

**Encryption Method:**
The `method` parameter is reserved. Pass `0` for default encryption.

**Swift**

```swift
func encryptDocument(outputPath: String,
                     userPassword: String,
                     ownerPassword: String,
                     permission: Int,
                     method: Int) -> Bool {
    let idString = "RadaeePDF"  // Document ID

    return pdfPlugin?.encryptDocAs(
        outputPath,
        userPwd: userPassword,
        ownerPwd: ownerPassword,
        permission: Int32(permission),
        method: Int32(method),
        idString: idString
    ) ?? false
}

// Usage example: Create a password-protected PDF with print and extract permissions
let success = encryptDocument(
    outputPath: "/path/to/encrypted.pdf",
    userPassword: "user123",      // Password to open the document
    ownerPassword: "owner456",    // Password for full access
    permission: 4 | 16,           // Print + Extract text/images
    method: 0                     // Default encryption
)
```

**Objective-C**

```objc
- (BOOL)encryptDocument:(RDPDFDoc *)doc
             outputPath:(NSString *)outputPath
            userPassword:(NSString *)userPwd
           ownerPassword:(NSString *)ownerPwd
              permission:(int)permission
                  method:(int)method {
    NSString *idString = @"RadaeePDF";  // Document ID

    return [self.pdfPlugin encryptDocAs:outputPath
                                userPwd:userPwd
                               ownerPwd:ownerPwd
                             permission:permission
                                 method:method
                               idString:idString];
}

// Usage example
BOOL success = [self encryptDocument:doc
                          outputPath:@"/path/to/encrypted.pdf"
                        userPassword:@"user123"
                       ownerPassword:@"owner456"
                          permission:4 | 16   // Print + Extract text/images
                              method:0];      // Default encryption
```

### Extract Text from PDF

Use `objsStart:` to load text objects, then `objsString:` to extract text. The `objsCount` property returns the total character count.

**Swift**

```swift
func extractText(from doc: RDPDFDoc) -> String {
    var text = ""
    let pageCount = doc.pageCount()

    for i in 0..<pageCount {
        if let page = doc.page(Int32(i)) {
            // Load text objects (false = left-to-right, true = right-to-left)
            page.objsStart(false)

            // Extract all text from page
            let charCount = page.objsCount()
            if let pageText = page.objsString(0, Int32(charCount)) {
                text += "Page \(i + 1):\n\(pageText)\n\n"
            }
            page.close()
        }
    }

    return text
}
```

**Objective-C**

```objc
- (NSString *)extractTextFromPDF:(NSString *)filePath {
    RDPDFDoc *doc = [[RDPDFDoc alloc] init];
    NSMutableString *text = [[NSMutableString alloc] init];

    if ([doc open:filePath :@""] == 0) {  // 0 = success
        for (int i = 0; i < [doc pageCount]; i++) {
            RDPDFPage *page = [doc page:i];

            // Load text objects (NO = left-to-right)
            [page objsStart:NO];

            // Extract all text from page
            int charCount = [page objsCount];
            NSString *pageText = [page objsString:0 :charCount];
            if (pageText) {
                [text appendFormat:@"Page %d:\n%@\n\n", i + 1, pageText];
            }
            [page close];
        }
        [doc close];
    }

    return text;
}
```

### Search Text in PDF

Use `find:` to create a search session. The `RDPDFFinder` object provides match count and character indices.

**Swift**

```swift
func searchText(in doc: RDPDFDoc, searchTerm: String, matchCase: Bool, wholeWord: Bool) -> [(page: Int, startIndex: Int, endIndex: Int)] {
    var results: [(page: Int, startIndex: Int, endIndex: Int)] = []
    let pageCount = doc.pageCount()

    for i in 0..<pageCount {
        if let page = doc.page(Int32(i)) {
            page.objsStart(false)

            // Create finder session
            if let finder = page.find(searchTerm, matchCase, wholeWord) {
                let matchCount = finder.count()

                for j in 0..<matchCount {
                    let startIdx = finder.objsIndex(Int32(j))
                    let endIdx = finder.objsEnd(Int32(j))
                    results.append((page: Int(i), startIndex: Int(startIdx), endIndex: Int(endIdx)))
                }
            }
            page.close()
        }
    }

    return results
}
```

**Objective-C**

```objc
- (NSArray *)searchText:(NSString *)searchTerm inDoc:(RDPDFDoc *)doc {
    NSMutableArray *results = [NSMutableArray array];

    for (int i = 0; i < [doc pageCount]; i++) {
        RDPDFPage *page = [doc page:i];
        [page objsStart:NO];

        // Create finder session (matchCase: YES, wholeWord: NO)
        RDPDFFinder *finder = [page find:searchTerm :YES :NO];
        if (finder) {
            int matchCount = [finder count];

            for (int j = 0; j < matchCount; j++) {
                int startIdx = [finder objsIndex:j];
                int endIdx = [finder objsEnd:j];

                [results addObject:@{
                    @"page": @(i),
                    @"startIndex": @(startIdx),
                    @"endIndex": @(endIdx)
                }];
            }
        }
        [page close];
    }

    return results;
}
```

### Add Sticky Note Annotation

Use `addAnnotNote:` to add a sticky note at a specific point. Access the annotation via `annotAtIndex:` to set properties.

**Swift**

```swift
func addStickyNote(to doc: RDPDFDoc, pageIndex: Int, x: Float, y: Float,
                   subject: String, content: String) -> Bool {
    guard let page = doc.page(Int32(pageIndex)) else { return false }

    page.objsStart(false)

    // Create point in PDF coordinates
    var point = PDF_POINT(x: x, y: y)

    if page.addAnnotNote(&point) {
        // Get the newly added annotation (last one)
        let annotCount = page.annotCount()
        if let annot = page.annotAtIndex(annotCount - 1) {
            annot.setPopupSubject(subject)
            annot.setPopupText(content)
        }
        doc.save()
        page.close()
        return true
    }

    page.close()
    return false
}
```

**Objective-C**

```objc
- (BOOL)addStickyNoteTo:(RDPDFDoc *)doc pageIndex:(int)pageIndex
                      x:(float)x y:(float)y
                subject:(NSString *)subject content:(NSString *)content {
    RDPDFPage *page = [doc page:pageIndex];
    if (!page) return NO;

    [page objsStart:NO];

    // Create point in PDF coordinates
    PDF_POINT point;
    point.x = x;
    point.y = y;

    if ([page addAnnotNote:&point]) {
        // Get the newly added annotation (last one)
        int annotCount = [page annotCount];
        RDPDFAnnot *annot = [page annotAtIndex:annotCount - 1];
        if (annot) {
            [annot setPopupSubject:subject];
            [annot setPopupText:content];
        }
        [doc save];
        [page close];
        return YES;
    }

    [page close];
    return NO;
}
```

### Add Ink Annotation (Handwriting)

Use `RDPDFInk` to capture stroke data, then `addAnnotInk:` to add it to the page.

**Swift**

```swift
func addInkAnnotation(to doc: RDPDFDoc, pageIndex: Int) -> Bool {
    guard let page = doc.page(Int32(pageIndex)) else { return false }

    page.objsStart(false)

    // Create ink with line width and color (0xAARRGGBB format)
    guard let ink = RDPDFInk(2.0, 0xFF0000FF) else {  // Blue color, 2pt width
        page.close()
        return false
    }

    // Draw a stroke
    ink.onDown(100, 100)  // Touch down
    ink.onMove(150, 120)  // Move
    ink.onMove(200, 100)  // Move
    ink.onUp(200, 100)    // Touch up

    if page.addAnnotInk(ink) {
        doc.save()
        page.close()
        return true
    }

    page.close()
    return false
}
```

**Objective-C**

```objc
- (BOOL)addInkAnnotationTo:(RDPDFDoc *)doc pageIndex:(int)pageIndex {
    RDPDFPage *page = [doc page:pageIndex];
    if (!page) return NO;

    [page objsStart:NO];

    // Create ink with line width and color (0xAARRGGBB format)
    RDPDFInk *ink = [[RDPDFInk alloc] init:2.0 :0xFF0000FF];  // Blue, 2pt width

    // Draw a stroke
    [ink onDown:100 :100];  // Touch down
    [ink onMove:150 :120];  // Move
    [ink onMove:200 :100];  // Move
    [ink onUp:200 :100];    // Touch up

    if ([page addAnnotInk:ink]) {
        [doc save];
        [page close];
        return YES;
    }

    [page close];
    return NO;
}
```

### Add Rectangle Annotation

Use `addAnnotRect:` to add a rectangle annotation. Parameters include the rectangle bounds, line width, stroke color, and fill color in `0xAARRGGBB` format.

**Swift**

```swift
func addRectAnnotation(to doc: RDPDFDoc, pageIndex: Int,
                       left: Float, top: Float, right: Float, bottom: Float,
                       lineWidth: Float, strokeColor: UInt32, fillColor: UInt32) -> Bool {
    guard let page = doc.page(Int32(pageIndex)) else { return false }

    page.objsStart(false)

    // Rectangle coordinates in PDF coordinates
    var rect = PDF_RECT()
    rect.left = left
    rect.top = top
    rect.right = right
    rect.bottom = bottom

    // Add rectangle: rect, lineWidth, strokeColor, fillColor (0xAARRGGBB)
    // fillColor alpha=0 means no fill
    if page.addAnnotRect(&rect, lineWidth, Int32(bitPattern: strokeColor), Int32(bitPattern: fillColor)) {
        doc.save()
        page.close()
        return true
    }

    page.close()
    return false
}

// Usage example
let success = addRectAnnotation(
    to: doc,
    pageIndex: 0,
    left: 100, top: 200, right: 300, bottom: 100,
    lineWidth: 2.0,
    strokeColor: 0xFFFF0000,  // Red stroke
    fillColor: 0x00000000     // No fill (alpha = 0)
)
```

**Objective-C**

```objc
- (BOOL)addRectAnnotationTo:(RDPDFDoc *)doc pageIndex:(int)pageIndex {
    RDPDFPage *page = [doc page:pageIndex];
    if (!page) return NO;

    [page objsStart:NO];

    // Rectangle coordinates (left, top, right, bottom) in PDF coordinates
    PDF_RECT rect;
    rect.left = 100;
    rect.top = 200;
    rect.right = 300;
    rect.bottom = 100;

    // Add rectangle: rect, lineWidth, strokeColor, fillColor (0xAARRGGBB)
    // fillColor alpha=0 means no fill
    if ([page addAnnotRect:&rect :2.0 :0xFFFF0000 :0x00000000]) {  // Red stroke, no fill
        [doc save];
        [page close];
        return YES;
    }

    [page close];
    return NO;
}
```

### Fill Form Fields

Form fields are accessed as annotations on each page. Use `annotCount` and `annotAtIndex:` to iterate through annotations, then check `fieldType` to identify form fields.

**Form Field Types (from `fieldType`):**
- `0` - Unknown (not a form field)
- `1` - Button field (push button, checkbox, radio button)
- `2` - Text field
- `3` - Choice field (list box, combo box)
- `4` - Signature field

**Check/Radio Status (from `getCheckStatus`):**
- `0` - Checkbox unchecked
- `1` - Checkbox checked
- `2` - Radio button unchecked
- `3` - Radio button checked

**Swift**

```swift
func fillFormFields(in doc: RDPDFDoc) {
    let pageCount = doc.pageCount()

    for pageIndex in 0..<pageCount {
        guard let page = doc.page(Int32(pageIndex)) else { continue }
        page.objsStart(false)

        let annotCount = page.annotCount()
        for i in 0..<annotCount {
            guard let annot = page.annotAtIndex(Int32(i)) else { continue }

            let fieldType = annot.fieldType()

            switch fieldType {
            case 1:  // Button field
                let checkStatus = annot.getCheckStatus()
                if checkStatus == 0 || checkStatus == 1 {
                    // Checkbox
                    annot.setCheckValue(true)
                } else if checkStatus == 2 || checkStatus == 3 {
                    // Radio button
                    annot.setRadio()
                }
            case 2:  // Text field
                annot.setEditText("Sample Value")
            case 3:  // Choice field (list/combo box)
                annot.setComboSel(0)  // Select first item
            case 4:  // Signature field
                break
            default:
                break
            }
        }
        page.close()
    }

    doc.save()
}
```

**Objective-C**

```objc
- (void)fillFormFields:(RDPDFDoc *)doc {
    int pageCount = [doc pageCount];

    for (int pageIndex = 0; pageIndex < pageCount; pageIndex++) {
        RDPDFPage *page = [doc page:pageIndex];
        if (!page) continue;

        [page objsStart:NO];

        int annotCount = [page annotCount];
        for (int i = 0; i < annotCount; i++) {
            RDPDFAnnot *annot = [page annotAtIndex:i];
            if (!annot) continue;

            int fieldType = [annot fieldType];

            switch (fieldType) {
                case 1: {  // Button field
                    int checkStatus = [annot getCheckStatus];
                    if (checkStatus == 0 || checkStatus == 1) {
                        // Checkbox
                        [annot setCheckValue:YES];
                    } else if (checkStatus == 2 || checkStatus == 3) {
                        // Radio button
                        [annot setRadio];
                    }
                    break;
                }
                case 2:  // Text field
                    [annot setEditText:@"Sample Value"];
                    break;
                case 3:  // Choice field (list/combo box)
                    [annot setComboSel:0];  // Select first item
                    break;
                case 4:  // Signature field
                    break;
            }
        }
        [page close];
    }

    [doc save];
}
```

## SwiftUI Integration

For SwiftUI apps, wrap the `RadaeePDFPlugin` viewer in a `UIViewControllerRepresentable`:

```swift
import SwiftUI

struct PDFViewerRepresentable: UIViewControllerRepresentable {
    let pdfPath: String
    let password: String
    let licenseKey: String

    func makeUIViewController(context: Context) -> UIViewController {
        let plugin = RadaeePDFPlugin.pluginInit()
        plugin?.activateLicense(withSerialKey: licenseKey)

        if let pdfVC = plugin?.show(pdfPath, withPassword: password) as? UIViewController {
            return pdfVC
        }

        // Return empty view controller if PDF fails to open
        return UIViewController()
    }

    func updateUIViewController(_ uiViewController: UIViewController, context: Context) {
        // Update if needed
    }
}

struct ContentView: View {
    var body: some View {
        PDFViewerRepresentable(
            pdfPath: "/path/to/document.pdf",
            password: "",
            licenseKey: "YOUR-LICENSE-KEY"
        )
        .edgesIgnoringSafeArea(.all)
    }
}
```

For presenting the PDF viewer as a sheet or full-screen cover:

```swift
struct DocumentListView: View {
    @State private var showPDF = false
    @State private var selectedPDFPath: String?

    var body: some View {
        List {
            Button("Open Document") {
                selectedPDFPath = "/path/to/document.pdf"
                showPDF = true
            }
        }
        .fullScreenCover(isPresented: $showPDF) {
            if let path = selectedPDFPath {
                PDFViewerRepresentable(
                    pdfPath: path,
                    password: "",
                    licenseKey: "YOUR-LICENSE-KEY"
                )
                .edgesIgnoringSafeArea(.all)
            }
        }
    }
}
```

## License Levels

RadaeePDF offers different license levels with varying features:

Visit [https://www.radaeepdf.com/](https://www.radaeepdf.com/) for detailed licensing information.

## Documentation

For complete API documentation and advanced features, visit:
- [RadaeePDF Support Portal](https://support.radaeepdf.com/)
- [Wiki](https://github.com/RadaeePDF-Jugaad/RadaeePDF-Master-iOS/wiki)

## Support

For technical support and questions:
- Email: support@radaeepdf.com
- Website: [https://www.radaeepdf.com/](https://www.radaeepdf.com/)

## License

This SDK is commercial software. Please ensure you have a valid license before using it in production applications.

---

© 2026 RadaeePDF. All rights reserved.

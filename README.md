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
   - **Important for Apple Silicon Macs**: If you're running on an Apple Silicon (M1/M2/M3) Mac, you need the **Universal** version of the iOS Simulator runtime (which includes x86_64/Rosetta support) instead of the default arm64-only version. This option is **not** available from Xcode's UI — **Settings** → **Platforms** only lets you download the default arm64 runtime. You need to install it from the terminal instead:
     1. If an iOS simulator runtime is already installed, delete it first (**Xcode** → **Settings** → **Platforms**, or via `xcrun simctl runtime list` / `delete`)
     2. Run the following command in Terminal:
        ```
        xcodebuild -downloadPlatform iOS -architectureVariant universal
        ```
     3. Wait for the download to finish, then relaunch Xcode
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
